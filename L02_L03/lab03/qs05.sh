#!/bin/bash


NUMBER_OF_PARAMETERS="${#}"
if ! [[ "${NUMBER_OF_PARAMETERS}" -eq 1 ]] || ! [[ ${1} =~ ^[+-]?[0-9]+([.][0-9]+)?$ ]]
then
    echo "Usage: ${0} PATTERN_NUMBER"
    exit 1
fi

case ${1} in
    "1")
        cat ./pattern1.txt
        ;;
    "2")
        cat ./pattern2.txt
        ;;
    "3")
        cat ./pattern3.txt
        ;;
    *)
        printf "Please choose a number between 1, 2, and 3"
        ;;
esac 

exit 0
