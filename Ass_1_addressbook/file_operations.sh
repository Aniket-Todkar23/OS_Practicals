#!/bin/bash

# Problem 1.6 - Simple File Operations

# Function a: Find a file
find_file() {
    echo ""
    echo "=== FIND A FILE ==="
    
    read -p "Enter filename to search: " filename
    
    # Use find and check if output is empty
    if [ -z "$(find . -name "$filename" 2>/dev/null)" ]; then
        echo "File '$filename' not found!"
    else
        echo "File '$filename' found at:"
        find . -name "$filename"
    fi
}

# Function b: Check if directory or file
check_type() {
    echo ""
    echo "=== CHECK FILE TYPE ==="
    
    read -p "Enter path: " path
    
    if [ -d "$path" ]; then
        echo "'$path' is a directory"
    elif [ -f "$path" ]; then
        echo "'$path' is a file"
    else
        echo "'$path' is something else (or does not exist)"
    fi
}

# Function c: Convert filenames to uppercase
convert_uppercase() {
    echo ""
    echo "=== CONVERT TO UPPERCASE ==="
    
    read -p "Enter filename(s) (space-separated): " files
    
    for file in $files; do
        if [ ! -f "$file" ]; then
            echo "Error: '$file' does not exist in current directory"
            continue
        fi
        
        # Convert filename to uppercase
        newname=$(echo "$file" | tr '[:lower:]' '[:upper:]')
        
        if [ "$file" = "$newname" ]; then
            echo "'$file' is already uppercase"
        else
            mv "$file" "$newname"
            echo "Renamed: $file -> $newname"
        fi
    done
}

# Function: Exit
quit() {
    echo ""
    echo "Goodbye!"
    exit 0
}

# Main Menu
while true; do
    echo ""
    echo "======================================"
    echo "      FILE OPERATIONS MENU"
    echo "======================================"
    echo "1. Find a file"
    echo "2. Check if directory or file"
    echo "3. Convert filenames to uppercase"
    echo "4. Exit"
    echo "======================================"
    read -p "Enter choice [1-4]: " choice
    
    case $choice in
        1) find_file ;;
        2) check_type ;;
        3) convert_uppercase ;;
        4) quit ;;
        *) echo "Invalid choice!" ;;
    esac
done
