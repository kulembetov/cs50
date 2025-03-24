def main():
    height = get_height()

    for i in range(height):
        # print spaces for left alignment
        print(" " * (height - i - 1), end="")
        # print left hashes
        print("#" * (i + 1), end="")
        # print gap
        print("  ", end="")
        # print right hashes
        print("#" * (i + 1))


def get_height():
    while True:
        try:
            height = int(input("Height: "))
            if height > 0 and height <= 8:
                return height
        except ValueError:
            print("Invalid input. Please enter a positive integer between 1 and 8.")


if __name__ == "__main__":
    main()
