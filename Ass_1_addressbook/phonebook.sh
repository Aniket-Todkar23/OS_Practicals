#!/bin/bash

# Simple Phonebook Program - Problem 1.2
# Format: FirstName	LastName	PhoneNumber (tab-separated)

FILE="phonebook.txt"

# Create file if not exists
if [ ! -f "$FILE" ]; then
    touch "$FILE"
fi

# Function 1: Add a new name
add() {
    echo ""
    echo "=== ADD NEW ENTRY ==="
    
    read -p "Enter First Name: " firstname
    read -p "Enter Last Name: " lastname
    read -p "Enter Phone Number: " phone
    
    # Add to file with tabs as separator
    echo -e "$firstname\t$lastname\t$phone" >> "$FILE"
    echo "Entry added successfully!"
}

# Function 2: Display all matches
display() {
    echo ""
    echo "=== SEARCH PHONEBOOK ==="
    
    if [ ! -s "$FILE" ]; then
        echo "Phonebook is empty!"
        return
    fi
    
    echo "Search by:"
    echo "1. Name (First or Last)"
    echo "2. Phone Number"
    read -p "Enter choice [1-2]: " search_choice
    
    case $search_choice in
        1)
            read -p "Enter Name to search: " search_name
            echo ""
            echo "Search Results:"
            echo "----------------------------------------"
            
            # Search in both first and last name
            result=$(grep -i "$search_name" "$FILE")
            
            if [ -z "$result" ]; then
                echo "No matches found for: $search_name"
            else
                printf "%-15s %-15s %-15s\n" "FirstName" "LastName" "Phone"
                echo "----------------------------------------"
                echo "$result" | while IFS=$'\t' read firstname lastname phone; do
                    printf "%-15s %-15s %-15s\n" "$firstname" "$lastname" "$phone"
                done
            fi
            ;;
        2)
            read -p "Enter Phone Number to search: " search_phone
            echo ""
            echo "Search Results:"
            echo "----------------------------------------"
            
            result=$(grep "$search_phone" "$FILE")
            
            if [ -z "$result" ]; then
                echo "No matches found for: $search_phone"
            else
                printf "%-15s %-15s %-15s\n" "FirstName" "LastName" "Phone"
                echo "----------------------------------------"
                echo "$result" | while IFS=$'\t' read firstname lastname phone; do
                    printf "%-15s %-15s %-15s\n" "$firstname" "$lastname" "$phone"
                done
            fi
            ;;
        *)
            echo "Invalid choice!"
            ;;
    esac
}

# Function 3: Sort by last name
sort_phonebook() {
    echo ""
    echo "=== SORTED PHONEBOOK (By Last Name) ==="
    
    if [ ! -s "$FILE" ]; then
        echo "Phonebook is empty!"
        return
    fi
    
    echo ""
    printf "%-15s %-15s %-15s\n" "FirstName" "LastName" "Phone"
    echo "------------------------------------------------------------"
    
    # Sort by second field (last name) using tab as separator
    sort -t$'\t' -k2 "$FILE" | while IFS=$'\t' read firstname lastname phone; do
        printf "%-15s %-15s %-15s\n" "$firstname" "$lastname" "$phone"
    done
    
    echo ""
    read -p "Do you want to save this sorted order? (y/n): " save_choice
    if [ "$save_choice" = "y" ] || [ "$save_choice" = "Y" ]; then
        sort -t$'\t' -k2 "$FILE" > temp.txt
        mv temp.txt "$FILE"
        echo "Phonebook sorted and saved!"
    fi
}

# Function 4: Delete an entry
delete() {
    echo ""
    echo "=== DELETE ENTRY ==="
    
    if [ ! -s "$FILE" ]; then
        echo "Phonebook is empty!"
        return
    fi
    
    echo "Delete by:"
    echo "1. Phone Number"
    echo "2. Last Name"
    read -p "Enter choice [1-2]: " del_choice
    
    case $del_choice in
        1)
            read -p "Enter Phone Number to delete: " del_phone
            
            if ! grep -q "$del_phone" "$FILE"; then
                echo "No entry found with phone: $del_phone"
                return
            fi
            
            # Show what will be deleted
            echo ""
            echo "Entry to be deleted:"
            grep "$del_phone" "$FILE" | while IFS=$'\t' read firstname lastname phone; do
                echo "Name: $firstname $lastname, Phone: $phone"
            done
            
            read -p "Are you sure? (y/n): " confirm
            if [ "$confirm" = "y" ] || [ "$confirm" = "Y" ]; then
                grep -v "$del_phone" "$FILE" > temp.txt
                mv temp.txt "$FILE"
                echo "Entry deleted successfully!"
            else
                echo "Deletion cancelled."
            fi
            ;;
        2)
            read -p "Enter Last Name to delete: " del_lastname
            
            if ! grep -i "$del_lastname" "$FILE"; then
                echo "No entry found with last name: $del_lastname"
                return
            fi
            
            # Show what will be deleted
            echo ""
            echo "Entries to be deleted:"
            grep -i "$del_lastname" "$FILE" | while IFS=$'\t' read firstname lastname phone; do
                echo "Name: $firstname $lastname, Phone: $phone"
            done
            
            read -p "Are you sure? (y/n): " confirm
            if [ "$confirm" = "y" ] || [ "$confirm" = "Y" ]; then
                grep -iv "$del_lastname" "$FILE" > temp.txt
                mv temp.txt "$FILE"
                echo "Entry deleted successfully!"
            else
                echo "Deletion cancelled."
            fi
            ;;
        *)
            echo "Invalid choice!"
            ;;
    esac
}

# Function 5: Exit
quit() {
    echo ""
    echo "Goodbye!"
    exit 0
}

# Main Menu Loop
while true; do
    echo ""
    echo "======================================="
    echo "         PHONEBOOK MENU"
    echo "======================================="
    echo "1. Add a new name"
    echo "2. Display all matches (Search)"
    echo "3. Sort phonebook by last name"
    echo "4. Delete an entry"
    echo "5. Exit"
    echo "======================================="
    read -p "Enter choice [1-5]: " choice
    
    case $choice in
        1) add ;;
        2) display ;;
        3) sort_phonebook ;;
        4) delete ;;
        5) quit ;;
        *) echo "Invalid choice! Please enter 1-5" ;;
    esac
done
