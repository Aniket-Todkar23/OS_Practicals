#!/bin/bash

# Simple Address Book Program
# Format: ID;Name;Phone

FILE="address.txt"

# Create file if not exists
if [ ! -f "$FILE" ]; then
    touch "$FILE"
fi

# Function 1: Search Address Book
search() {
    echo ""
    echo "=== SEARCH ADDRESS BOOK ==="
    
    if [ ! -s "$FILE" ]; then
        echo "Address book is empty!"
        return
    fi
    
    read -p "Enter ID to search: " search_id
    
    # Search for the ID
    result=$(grep "^$search_id;" "$FILE")
    
    if [ -z "$result" ]; then
        echo "No record found with ID: $search_id"
    else
        echo "Found:"
        echo "$result" | while IFS=';' read id name phone; do
            echo "ID: $id"
            echo "Name: $name"
            echo "Phone: $phone"
        done
    fi
}

# Function 2: Add Entry
add() {
    echo ""
    echo "=== ADD NEW ENTRY ==="
    
    read -p "Enter ID: " id
    read -p "Enter Name: " name
    read -p "Enter Phone: " phone
    
    # Check if ID already exists
    if grep -q "^$id;" "$FILE"; then
        echo "Error: ID already exists!"
        return
    fi
    
    # Add to file (format: ID;Name;Phone)
    echo "$id;$name;$phone" >> "$FILE"
    echo "Record added successfully!"
}

# Function 3: Remove Entry
remove() {
    echo ""
    echo "=== REMOVE ENTRY ==="
    
    if [ ! -s "$FILE" ]; then
        echo "Address book is empty!"
        return
    fi
    
    read -p "Enter ID to remove: " id
    
    # Check if ID exists
    if ! grep -q "^$id;" "$FILE"; then
        echo "No record found with ID: $id"
        return
    fi
    
    # Remove the line with that ID
    grep -v "^$id;" "$FILE" > temp.txt
    mv temp.txt "$FILE"
    echo "Record removed successfully!"
}

# Function 4: Quit
quit() {
    echo ""
    echo "Goodbye!"
    exit 0
}

# Main Menu Loop
while true; do
    echo ""
    echo "=============================="
    echo "     ADDRESS BOOK MENU"
    echo "=============================="
    echo "1. Search Address Book"
    echo "2. Add an address book entry"
    echo "3. Remove an address book entry"
    echo "4. Quit the program"
    echo "=============================="
    read -p "Enter choice [1-4]: " choice
    
    case $choice in
        1) search ;;
        2) add ;;
        3) remove ;;
        4) quit ;;
        *) echo "Invalid choice! Please enter 1-4" ;;
    esac
done
