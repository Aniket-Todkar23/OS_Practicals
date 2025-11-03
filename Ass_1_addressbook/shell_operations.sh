#!/bin/bash

# Problem 1.5 - Shell Script Operations

# Function a: Display lines between given line numbers
display_lines() {
    echo ""
    echo "=== DISPLAY LINES FROM FILE ==="
    
    read -p "Enter filename: " filename
    
    # Check if file exists
    if [ ! -f "$filename" ]; then
        echo "Error: File '$filename' does not exist!"
        return
    fi
    
    read -p "Enter starting line number: " start
    read -p "Enter ending line number: " end
    
    # Validate line numbers
    if [ $start -gt $end ]; then
        echo "Error: Starting line must be less than or equal to ending line!"
        return
    fi
    
    echo ""
    echo "Lines $start to $end from '$filename':"
    echo "----------------------------------------"
    
    # Display lines using sed
    sed -n "${start},${end}p" "$filename"
    
    echo "----------------------------------------"
}

# Function b: Delete lines containing a word
delete_lines() {
    echo ""
    echo "=== DELETE LINES CONTAINING A WORD ==="
    
    read -p "Enter word to delete: " word
    read -p "Enter filename(s) (space-separated): " files
    
    # Loop through each file
    for file in $files; do
        if [ ! -f "$file" ]; then
            echo "Warning: File '$file' does not exist. Skipping..."
            continue
        fi
        
        echo ""
        echo "Processing '$file'..."
        
        # Count lines before deletion
        before=$(wc -l < "$file")
        
        # Delete lines containing the word
        sed -i.bak "/$word/d" "$file"
        
        # Count lines after deletion
        after=$(wc -l < "$file")
        deleted=$((before - after))
        
        echo "  Lines deleted: $deleted"
        echo "  Backup saved as: ${file}.bak"
    done
    
    echo ""
    echo "Operation completed!"
}

# Function c: Calculate sum using bc and expr
calculate_sum() {
    echo ""
    echo "=== CALCULATE SUM OF TWO NUMBERS ==="
    
    read -p "Enter first number: " num1
    read -p "Enter second number: " num2
    
    # Check if arguments are valid numbers
    if ! [[ "$num1" =~ ^[0-9]+$ ]] || ! [[ "$num2" =~ ^[0-9]+$ ]]; then
        echo "Error: Please enter valid numbers only!"
        return
    fi
    
    echo ""
    echo "Method 1 - Using 'expr':"
    sum_expr=$(expr $num1 + $num2)
    echo "  $num1 + $num2 = $sum_expr"
    
    echo ""
    echo "Method 2 - Using 'bc':"
    sum_bc=$(echo "$num1 + $num2" | bc)
    echo "  $num1 + $num2 = $sum_bc"
}

# Function: Exit
quit() {
    echo ""
    echo "Goodbye!"
    exit 0
}

# Main Menu Loop
while true; do
    echo ""
    echo "=============================================="
    echo "       SHELL SCRIPT OPERATIONS MENU"
    echo "=============================================="
    echo "1. Display lines from file (between line numbers)"
    echo "2. Delete lines containing a word"
    echo "3. Calculate sum of two numbers (expr & bc)"
    echo "4. Exit"
    echo "=============================================="
    read -p "Enter choice [1-4]: " choice
    
    case $choice in
        1) display_lines ;;
        2) delete_lines ;;
        3) calculate_sum ;;
        4) quit ;;
        *) echo "Invalid choice! Please enter 1-4" ;;
    esac
done
