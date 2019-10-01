#!/bin/bash

# Check correct number of arguments
if [ $# -ne 1 ];then
  printf "USAGE: $0 [server port]\n"
  exit
fi
PORT=$1
ALL_CORRECT=true

# function to compare message files
function compare {
  if diff -q test_message.txt test_output.txt; then
    printf "\nSUCCESS: Message received matches message sent!\n"
  else
    ALL_CORRECT=false
    diff test_message.txt test_output.txt
  fi
  printf "________________________________________\n\n"
}

function compare_2 {
  if diff -q small_input.txt test_output.txt; then
    printf "\nSUCCESS: Message received matches message sent!\n"
  else
    ALL_CORRECT=false
    diff small_input.txt test_output.txt
  fi
  printf "________________________________________\n\n"
}

function test {
  # printf "Testing 1234!\n" > test_message.txt
  # $2 $3 > test_output.txt &
  # sleep 1
  # $1 127.0.0.1 $3 < test_message.txt
  # sleep 1
  # kill $!
  # sleep 1
  # compare
  # rm -f test_output.txt

  # printf "Testing 1234!\n" > test_message.txt
  $2 $3 > test_output.txt &
  sleep 1
  $1 127.0.0.1 $3 < small_input.txt
  sleep 1
  kill $!
  sleep 1
  compare_2
  rm -f test_output.txt
}

# Print header
printf "\nTESTING CLIENT/SERVER IMPLEMENTATIONS\n"
printf "_________________________________________\n\n"

###############################################
# Run tests
###############################################
printf "\nTesting C client -> C server\n\n"
test ./client-c ./server-c $PORT

printf "Testing Python client -> Python server\n\n"
test "python client-python.py" "python server-python.py" $PORT

printf "\nTesting Python client -> C server\n\n"
test "python client-python.py" ./server-c $PORT

printf "\nTesting C client -> Python server\n\n"
test ./client-c "python server-python.py" $PORT


###############################################
# Results message
###############################################
if [ $ALL_CORRECT == true ]; then
  printf "\SUCCESS: You passed all the tests here!\n"
  printf "However, these are not the only tests we will use for grading.\n"
  printf "Double check the client/server specifications in the assignment "
  printf "before submitting.\n"
else
  printf "At least 1 of the above tests didn't pass.\n"
  printf "Check the client/server specifications and debugging tips in the assignment.\n"
fi
