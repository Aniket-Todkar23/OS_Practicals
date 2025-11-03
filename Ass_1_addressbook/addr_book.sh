#!/bin/bash
declare -a name
declare -a phone
declare -a email
count=0
showmenu(){
	echo "================================="
	echo "              MENU"
	echo "================================="
	echo "1]View Address book"
	echo "2]Insert a record"
	echo "3]Delete a record"
	echo "4]Modify a record"
	echo "5]Exit"
	echo "================================="
}
ViewBook(){
	if [ $count -eq 0 ]; then
		echo "No record Found"
	else
		printf "%-5s %-20s %-15s %-25s\n" "No." "Name" "Phone" "Email"
		for ((i=0; i<$count; i++)); do
			printf "%-5s %-20s %-15s %-25s\n" "$((i+1))" "${name[$i]}" "${phone[$i]}" "${email[$i]}"
		done
	fi
}
InsertRec(){
	read -p "Enter name: " n
	read -p "Enter Phone: " p
	read -p "Enter Email: " e
	name[$count]=$n
	phone[$count]=$p
	email[$count]=$e
	((count++))
	echo "Record Inserted Successfully"
}
Exit(){
	echo "Goodbye"
	exit 0
}
while true; do
	showmenu
	read -p "Enter Your choice" choice
	case $choice in 
		1)ViewBook ;;
		2)InsertRec ;;
		3)DeleteRec ;;
		4)ModifyRec ;;
		5)Exit ;;
		*)echo "Invalid Choice" ;;
	esac
done
