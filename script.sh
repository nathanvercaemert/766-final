#!/bin/bash

# Get the path to the program
program_path="/root/766-volume/nhvercae/ast_printer"

# Find all .c files in the current directory and its subdirectories
find . -type f -name "*.c" | while read -r file; do
    # Check if the file exists
    if [ -e "$file" ]; then
        # Execute the program on the file and capture the output
        output=$($program_path "$file")

        # Check if the output is non-empty
        if [ -n "$output" ]; then
            # Print the file name
            echo "Processing file: $file"

            # Print the program's output
            echo "$output"
        fi
    fi
done
