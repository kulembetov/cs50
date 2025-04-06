import os
from dotenv import load_dotenv

# load environment variables
load_dotenv()

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# configure application
app = Flask(__name__)

# ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"

# set secret key for session management
app.secret_key = (
    "your_secret_key_here"  # replace with a secure secret key in production
)

Session(app)

# custom filter
app.jinja_env.filters["usd"] = usd

# configure cs50 library to use sqlite database
db = SQL("sqlite:///finance.db")

# ensure database tables exist
db.execute(
    """
CREATE TABLE IF NOT EXISTS users (
    id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
    username TEXT NOT NULL,
    hash TEXT NOT NULL,
    cash NUMERIC NOT NULL DEFAULT 10000.00
)
"""
)

db.execute("CREATE UNIQUE INDEX IF NOT EXISTS username ON users (username)")

db.execute(
    """
CREATE TABLE IF NOT EXISTS transactions (
    id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
    user_id INTEGER NOT NULL,
    symbol TEXT NOT NULL,
    shares INTEGER NOT NULL,
    price NUMERIC NOT NULL,
    type TEXT NOT NULL,
    timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (user_id) REFERENCES users(id)
)
"""
)


# add current year to all templates
@app.context_processor
def inject_year():
    from datetime import datetime

    return dict(year=datetime.now().year)


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """show portfolio of stocks"""

    # get user's stocks and shares
    stocks = db.execute(
        """
        SELECT symbol, SUM(shares) as total_shares
        FROM transactions
        WHERE user_id = ?
        GROUP BY symbol
        HAVING total_shares > 0
    """,
        session["user_id"],
    )

    # get user's cash balance
    rows = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
    cash = rows[0]["cash"]

    # initialize grand total (cash + stocks value)
    grand_total = cash

    # get current price for each stock and calculate total values
    for stock in stocks:
        quote = lookup(stock["symbol"])
        stock["name"] = quote["name"]
        stock["price"] = quote["price"]
        stock["shares"] = stock["total_shares"]
        stock["total"] = stock["price"] * stock["shares"]
        grand_total += stock["total"]

    # render portfolio
    return render_template(
        "index.html", stocks=stocks, cash=cash, grand_total=grand_total
    )


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """buy shares of stock"""

    # user reached route via post (as by submitting a form via post)
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        # ensure symbol was submitted
        if not symbol:
            return apology("must provide symbol", 400)

        # ensure number of shares was submitted
        if not shares:
            return apology("must provide number of shares", 400)

        # ensure number of shares is positive integer
        try:
            shares = int(shares)
            if shares <= 0:
                return apology("number of shares must be positive", 400)
        except ValueError:
            return apology("number of shares must be positive integer", 400)

        # look up quote
        quote = lookup(symbol)

        # ensure symbol exists
        if quote is None:
            return apology("invalid symbol", 400)

        # calculate total cost
        price = quote["price"]
        total_cost = price * shares

        # get user's cash balance
        rows = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        cash = rows[0]["cash"]

        # check if user can afford the purchase
        if cash < total_cost:
            return apology("can't afford", 400)

        # update user's cash balance
        db.execute(
            "UPDATE users SET cash = ? WHERE id = ?",
            cash - total_cost,
            session["user_id"],
        )

        # record the transaction
        db.execute(
            "INSERT INTO transactions (user_id, symbol, shares, price, type) VALUES (?, ?, ?, ?, ?)",
            session["user_id"],
            quote["symbol"],
            shares,
            price,
            "buy",
        )

        # redirect user to home page
        flash(f"bought {shares} shares of {quote['symbol']} at {usd(price)}")
        return redirect("/")

    # user reached route via get (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """show history of transactions"""

    # query database for all transactions
    transactions = db.execute(
        """
        SELECT symbol, shares, price, type, timestamp
        FROM transactions
        WHERE user_id = ?
        ORDER BY timestamp DESC
    """,
        session["user_id"],
    )

    # render history page
    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """log user in"""

    # forget any user_id
    session.clear()

    # user reached route via post (as by submitting a form via post)
    if request.method == "POST":
        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect("/")

    # user reached route via get (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """log user out"""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """get stock quote"""

    # user reached route via post (as by submitting a form via post)
    if request.method == "POST":
        symbol = request.form.get("symbol")

        # ensure symbol was submitted
        if not symbol:
            return apology("must provide symbol", 400)

        # look up quote
        quote = lookup(symbol)

        # ensure symbol exists
        if quote is None:
            return apology("invalid symbol", 400)

        # display quote
        return render_template("quoted.html", quote=quote)

    # user reached route via get (as by clicking a link or via redirect)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """register user"""

    # forget any user_id
    session.clear()

    # user reached route via post (as by submitting a form via post)
    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # ensure username was submitted
        if not username:
            return apology("must provide username", 400)

        # ensure password was submitted
        if not password:
            return apology("must provide password", 400)

        # ensure confirmation password was submitted
        if not confirmation:
            return apology("must confirm password", 400)

        # ensure passwords match
        if password != confirmation:
            return apology("passwords must match", 400)

        # check if username already exists
        rows = db.execute("SELECT * FROM users WHERE username = ?", username)
        if len(rows) > 0:
            return apology("username already exists", 400)

        # insert new user
        user_id = db.execute(
            "INSERT INTO users (username, hash) VALUES (?, ?)",
            username,
            generate_password_hash(password),
        )

        # check if insert was successful
        if not user_id:
            return apology("registration error", 400)

        # log in the user immediately
        session["user_id"] = user_id
        flash("welcome! your account has been created successfully")
        return redirect("/")

    # user reached route via get (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """sell shares of stock"""

    # get user's stocks
    stocks = db.execute(
        """
        SELECT symbol, SUM(shares) as total_shares
        FROM transactions
        WHERE user_id = ?
        GROUP BY symbol
        HAVING total_shares > 0
    """,
        session["user_id"],
    )

    # user reached route via post (as by submitting a form via post)
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        # ensure symbol was submitted
        if not symbol:
            return apology("must select a symbol", 400)

        # ensure number of shares was submitted
        if not shares:
            return apology("must provide number of shares", 400)

        # ensure number of shares is positive integer
        try:
            shares = int(shares)
            if shares <= 0:
                return apology("number of shares must be positive", 400)
        except ValueError:
            return apology("number of shares must be positive integer", 400)

        # check if user owns enough shares
        user_shares = 0
        for stock in stocks:
            if stock["symbol"] == symbol:
                user_shares = stock["total_shares"]
                break

        if user_shares < shares:
            return apology("not enough shares", 400)

        # look up stock price
        quote = lookup(symbol)
        if quote is None:
            return apology("invalid symbol", 400)

        # calculate sale proceeds
        price = quote["price"]
        proceeds = price * shares

        # update user's cash
        db.execute(
            "UPDATE users SET cash = cash + ? WHERE id = ?",
            proceeds,
            session["user_id"],
        )

        # record the sale
        db.execute(
            "INSERT INTO transactions (user_id, symbol, shares, price, type) VALUES (?, ?, ?, ?, ?)",
            session["user_id"],
            symbol,
            -shares,  # negative for sales
            price,
            "sell",
        )

        # success message
        flash(f"sold {shares} shares of {symbol} at {usd(price)}")
        return redirect("/")

    # user reached route via get (as by clicking a link or via redirect)
    else:
        return render_template("sell.html", stocks=stocks)


if __name__ == "__main__":
    # ensure responses aren't cached in debug mode
    if app.config["DEBUG"]:

        @app.after_request
        def after_request(response):
            response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
            response.headers["Expires"] = 0
            response.headers["Pragma"] = "no-cache"
            return response

    # set api key
    if not os.environ.get("API_KEY"):
        raise RuntimeError("API_KEY not set")

    # run the app
    app.run(host="0.0.0.0", port=8080, debug=True)
