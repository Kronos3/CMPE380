#!/bin/bash
#----------------------------------------------------------------------------
# Complex menu exercise
#----------------------------------------------------------------------------

# create a simple user menu for specific tasks!
continue=true
while [ $continue ]
 do
    clear
    echo "-------------------------------------"
    echo " Select an option between 1-3 or 4 to exit!"
    echo "-------------------------------------"
    echo "[1] Open nano editor"
    echo "[2] Show files in the current directory"
    echo "[3] gcc a C file"
    echo "[4] Exit"
    echo "======================="
    read choice
    case $choice in
      1) nano ;;
      2) echo "Files in `pwd`" ; ls -la ; echo "Press a key. . ." ; read ;;
      3) echo "Enter a file. . ." ; read cFile ; gcc $cFile ; echo "Press a key. . ." ; read ;;
      4) echo "Press any key to exit..." ; read ; break ;;
      *) echo "Please select choice 1,2,3, or 4 to exit!";
         echo "Press a key. . ." ; read ;;
	esac
done