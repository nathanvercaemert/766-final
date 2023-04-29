#!/bin/bash

# Check if a directory is provided
if [ -z "$1" ]; then
    echo "Usage: $0 <directory>"
    exit 1
fi

directory="$1"

# Find all .c files in the specified directory and its subdirectories
# Then, search for 'scanf' in each file using grep, and print the file path and line number
find "$directory" -type f -name "*.c" -exec grep -nH '!= EOF' {} \;
