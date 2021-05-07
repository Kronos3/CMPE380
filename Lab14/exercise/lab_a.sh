#!/bin/bash
#----------------------------------------------------------------------------
# Simple command exercises
# student copy
#----------------------------------------------------------------------------

# Create a 2 word variable and print it out
DOG_CAT="dog cat"
echo "$DOG_CAT"

# Read a number from the command line
printf "Enter a number: "
read number
echo "Number is $number"


# Use the number in a simple if statement
if [[ $1 -gt 3 ]]; then
  echo "Number is greater than 3"
else
  echo "Number is less or equal to 3"
fi


# Do some math with the number
math_op=$(expr $1 + 2)
echo "$1 + 2 = $math_op"

# Do some math with the number using the alternative method
let math_op+=1
echo "math_op += 1"
echo "math_op == $math_op"

# Count from 0 to 3
let i=1
while [ $i -le 3 ]; do
  echo "i: $i"
  let i+=1
done

# Cycle through a list
for i in one two three; do
  echo "$i"
done


# Verify string comparison functions 
printf "Enter dog (or not): "
read dog_or_not
if [[ "$dog_or_not" == "dog" ]]; then
  echo "Dog was entered!"
else
  echo "Dog was not entered!!!!!"
fi

# Exercise the case statement
printf "Enter number between [1-3]: "
read number
case $number in
  1 | 2 | 3)
    echo "Value $number between 1 and 3"
    ;;

  *)
    echo "Value $number not between 1-3"
    ;;
esac

# Get and print the current directory
echo "Current dir $(pwd)"

