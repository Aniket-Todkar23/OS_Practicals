#!/bin/bash

# File Management Program - Problem 1.4

# Function 1: Test if file exists
test_file() {
    echo ""
    echo "=== TEST IF FILE EXISTS ==="
    
    read -p "Enter filename: " filename
    
    if [ -f "$filename" ]; then
        echo "SUCCESS: File '$filename' exists!"
        
        # Show file details
        echo ""
        echo "File Details:"
        echo "  Size: $(du -h "$filename" | cut -f1)"
        echo "  Type: $(file -b "$filename")"
        echo "  Last Modified: $(stat -c %y "$filename" 2>/dev/null || stat -f "%Sm" "$filename" 2>/dev/null)"
    else
        echo "ERROR: File '$filename' does NOT exist!"
    fi
}

# Function 2: Read a file
read_file() {
    echo ""
    echo "=== READ A FILE ==="
    
    read -p "Enter filename to read: " filename
    
    if [ ! -f "$filename" ]; then
        echo "ERROR: File '$filename' does not exist!"
        return
    fi
    
    if [ ! -r "$filename" ]; then
        echo "ERROR: File '$filename' is not readable!"
        return
    fi
    
    echo ""
    echo "--- Contents of '$filename' ---"
    echo ""
    
    # Display file with line numbers
    cat -n "$filename"
    
    echo ""
    echo "--- End of file ---"
}

# Function 3: Delete a file
delete_file() {
    echo ""
    echo "=== DELETE A FILE ==="
    
    read -p "Enter filename to delete: " filename
    
    if [ ! -f "$filename" ]; then
        echo "ERROR: File '$filename' does not exist!"
        return
    fi
    
    # Show confirmation
    echo ""
    echo "WARNING: You are about to delete '$filename'"
    read -p "Are you sure? (y/n): " confirm
    
    if [ "$confirm" = "y" ] || [ "$confirm" = "Y" ]; then
        rm "$filename"
        echo "SUCCESS: File '$filename' deleted!"
    else
        echo "Deletion cancelled."
    fi
}

# Function 4: Display list of files
display_files() {
    echo ""
    echo "=== DISPLAY LIST OF FILES ==="
    echo ""
    
    # Check if current directory has any files
    if [ -z "$(ls -A)" ]; then
        echo "No files found in current directory."
        return
    fi
    
    echo "Files in current directory:"
    echo "--------------------------------------------------------------"
    printf "%-30s %-10s %s\n" "Filename" "Size" "Type"
    echo "--------------------------------------------------------------"
    
    # List all files with details
    for file in *; do
        if [ -f "$file" ]; then
            size=$(du -h "$file" | cut -f1)
            if [ -d "$file" ]; then
                type="Directory"
            elif [ -x "$file" ]; then
                type="Executable"
            else
                type="File"
            fi
            printf "%-30s %-10s %s\n" "$file" "$size" "$type"
        fi
    done
    
    echo ""
    echo "Total files: $(ls -l | grep "^-" | wc -l)"
}

# Function 5: Exit
quit() {
    echo ""
    echo "Exiting File Manager. Goodbye!"
    exit 0
}

# Main Menu Loop
while true; do
    echo ""
    echo "==========================================="
    echo "         FILE MANAGEMENT PROGRAM"
    echo "==========================================="
    echo "1. Test if file exists"
    echo "2. Read a file"
    echo "3. Delete a file"
    echo "4. Display a list of files"
    echo "5. Exit"
    echo "==========================================="
    read -p "Enter choice [1-5]: " choice
    
    case $choice in
        1) test_file ;;
        2) read_file ;;
        3) delete_file ;;
        4) display_files ;;
        5) quit ;;
        *) echo "Invalid choice! Please enter 1-5" ;;
    esac
done
