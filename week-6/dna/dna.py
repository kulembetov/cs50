import csv
import sys


def main():
    # check for correct command-line usage
    if len(sys.argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")
        sys.exit(1)

    # read database file into a variable
    database = []
    with open(sys.argv[1]) as csvfile:
        reader = csv.DictReader(csvfile)
        # get STR names from header
        strs = reader.fieldnames[1:]
        # read each row into database
        for row in reader:
            # convert STR counts from strings to integers
            for str_name in strs:
                row[str_name] = int(row[str_name])
            database.append(row)

    # read DNA sequence file into a variable
    with open(sys.argv[2]) as file:
        sequence = file.read()

    # find longest match of each STR in DNA sequence
    str_counts = {}
    for str_name in strs:
        str_counts[str_name] = longest_match(sequence, str_name)

    # check database for matching profiles
    for person in database:
        match = True
        # check if all STR counts match
        for str_name in strs:
            if person[str_name] != str_counts[str_name]:
                match = False
                break
        if match:
            print(person["name"])
            return

    print("No match")


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):
        # initialize count of consecutive runs
        count = 0

        # check for a subsequence match in a "substring" (a subset of characters) within sequence
        # if a match, move substring to next potential match in sequence
        # continue moving substring and checking for matches until out of consecutive matches
        while True:
            # adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # if there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # if there is no match in the substring
            else:
                break

        # update most consecutive matches found
        longest_run = max(longest_run, count)

    # after checking for runs at each character in sequence, return longest run found
    return longest_run


if __name__ == "__main__":
    main()
