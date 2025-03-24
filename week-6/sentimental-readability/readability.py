def main():
    # get text from user
    text = input("Text: ")

    # count letters, words, and sentences
    letters = count_letters(text)
    words = count_words(text)
    sentences = count_sentences(text)

    # calculate coleman-liau index
    L = (letters / words) * 100
    S = (sentences / words) * 100
    index = round(0.0588 * L - 0.296 * S - 15.8)

    # print grade level
    if index >= 16:
        print("Grade 16+")
    elif index < 1:
        print("Before Grade 1")
    else:
        print(f"Grade {index}")


def count_letters(text):
    # count alphabetic characters in text
    return sum(1 for c in text if c.isalpha())


def count_words(text):
    # count spaces and add 1 (words = spaces + 1)
    return len(text.split())


def count_sentences(text):
    # count sentence endings (periods, exclamations, and question marks)
    return sum(1 for c in text if c in [".", "!", "?"])


if __name__ == "__main__":
    main()
