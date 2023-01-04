#!/bin/bash


NUMBER_OF_PARAMETERS="${#}"
if ! [[ "${NUMBER_OF_PARAMETERS}" -eq 2 ]] || ! [[ ${1} =~ ^[+-]?[0-9]+([.][0-9]+)?$ ]] || ! [[ ${2} =~ ^[+-]?[0-9]+([.][0-9]+)?$ ]]
then
    echo "Usage: ${0} integer1 integer2"
    exit 1
fi

a=$(($1 + $2))
echo "sum: $a"

if [ $1 -gt $2 ]
then
        echo "$1 is greater than $2" 
elif [ $2 -gt $1 ]
then
        echo "$2 is greater than $1"
else
	echo "$1 equals $2"
fi

exit 0
