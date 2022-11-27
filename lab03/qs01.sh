#!/bin/bash


NUMBER_OF_PARAMETERS="${#}"
if ! [[ "${NUMBER_OF_PARAMETERS}" -eq 2 ]] || ! [[ ${1} =~ ^[+-]?[0-9]+([.][0-9]+)?$ ]] || ! [[ ${2} =~ ^[+-]?[0-9]+([.][0-9]+)?$ ]]
then
    echo "Usage: ${0} integer1 integer2"
    exit 1
fi

a=$(($1 + $2))
echo $a

if [ $1 -gt $2 ]
then
        echo $1
else
        echo $2
fi

exit 0
