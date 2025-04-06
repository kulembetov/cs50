import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session, url_for

# configure application
app = Flask(__name__, static_url_path='/static', static_folder='static')

# ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# enable debug mode
app.debug = True

# configure cs50 library to use sqlite database
db = SQL("sqlite:///birthdays.db")


@app.after_request
def after_request(response):
    """ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":
        # validate submission
        name = request.form.get("name")
        month = request.form.get("month")
        day = request.form.get("day")
        
        if not name or not month or not day:
            return redirect("/")
            
        # validate month and day
        try:
            month = int(month)
            day = int(day)
            if month < 1 or month > 12 or day < 1 or day > 31:
                return redirect("/")
        except ValueError:
            return redirect("/")
            
        # add birthday to database
        db.execute("INSERT INTO birthdays (name, month, day) VALUES(?, ?, ?)", name, month, day)
        return redirect("/")

    else:
        # query for all birthdays
        birthdays = db.execute("SELECT * FROM birthdays")
        return render_template("index.html", birthdays=birthdays)


@app.route("/delete", methods=["POST"])
def delete():
    # get birthday id
    id = request.form.get("id")
    if id:
        # delete birthday from database
        db.execute("DELETE FROM birthdays WHERE id = ?", id)
    return redirect("/")


@app.route("/edit", methods=["POST"])
def edit():
    # get form data
    id = request.form.get("id")
    name = request.form.get("name")
    month = request.form.get("month")
    day = request.form.get("day")
    
    if not id or not name or not month or not day:
        return redirect("/")
        
    # validate month and day
    try:
        month = int(month)
        day = int(day)
        if month < 1 or month > 12 or day < 1 or day > 31:
            return redirect("/")
    except ValueError:
        return redirect("/")
        
    # update birthday in database
    db.execute("UPDATE birthdays SET name = ?, month = ?, day = ? WHERE id = ?", name, month, day, id)
    return redirect("/")


if __name__ == "__main__":
    app.run(debug=True, host="0.0.0.0", port=8080)


