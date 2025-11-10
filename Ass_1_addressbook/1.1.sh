#!/bin/bash
if [ ! -f address.txt ]; then
    touch address.txt
fi

while true;do
    echo "MENU"
    echo "1) Add a Book"
    echo "2) Remove a Book"
    echo "3) Search a Book"
    echo "5) Display Book"
    echo "6) test file"
    echo "7) Delete file"
    echo "4) Exit"
    echo -n "Enter Choice :"
    read choice

    case $choice in 
        1) 
        echo "Enter Book Name :"
        read bname
        echo "Enter Book ID :"
        read bid
        echo "Enter Ph no"
        read ph

        if grep -q "^$bid;" address.txt; then
            echo "Book already exist"
        else 
            echo "$bid;$bname;$ph;" >> address.txt
            echo "Entry added successfully"
        fi
        ;;
        3)
        echo "Enter the book Id :"
        read bid
        result=$(grep -i "^$bid;" address.txt)
        if [[ -n $result ]]; then 
            echo "$result"
        else
            echo "No results found"
        fi
        ;;
        2)
        echo "Enter the book Id :"
        read bid
        if grep -q "^$bid" address.txt; then 
            sed -i "/^$bid;/d" address.txt
            echo "Entry removed successfully"
        else 
            echo "No result Found"
        fi
        ;;
        4)
        echo "Thank You"
        exit 0
        ;;
        5)
        echo "enteries are :"
        cat address.txt
        ;;
        6)
        echo "Enter the filename :"
        read filename 
        if [ -f $filename ]; then
            echo " File exists details below"
            ls -l $filename
        else 
            echo "File does not exist"
        fi
        ;;
        7)
        echo "Enter the filename :"
        read filename 
        if [ -f $filename ]; then
            echo "File Details"
            ls -l $filename
            echo "Do you want to delete file for sure :(y/n)"
            read choice
            if [ "$choice" = "y" ] || [ "$choice" = "Y" ]; then
                rm $filename
                echo "file deleted successfully"
            else 
            echo "Deletion cancelled"
            fi
        else 
        echo " No file found"
        fi
        ;;
        *)
        echo "Invalid Choice"
        ;;
        esac

done