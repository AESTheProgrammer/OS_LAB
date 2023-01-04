#!/bin/bash

NUMBER_OF_PARAMETERS="${#}"
if ! [[ "${NUMBER_OF_PARAMETERS}" -eq 3 ]] || ! [[ ${1} =~ ^[+-]?[0-9]+([.][0-9]+)?$ ]] || ! [[ ${3} =~ ^[+-]?[0-9]+([.][0-9]+)?$ ]]
then
    echo "Usage: ${0} operand1 operator operand2"
    exit 1
fi

#echo debug01

case ${2} in
    '+')
        a=$(($1 + $3))
        ;;
    '-')
        a=$(($1 - $3))
        ;;
    '*')
        a=$(($1 * $3))
        ;;
    '/')
        a=$(($1 / $3))
        ;;
    '%')
        a=$(($1 % $3))
        ;;
    '**')
        a=$(($1 ** $3))
        ;;
    '^')
        a=$(($1 ^ $3))
        ;;
    '&')
        a=$(($1 & $3))
        ;;
    '|')
        a=$(($1 | $3))
        ;;
    *)
        echo "Usage: ${0} operand1 operator operand2"
        ;;
esac
echo $a
exit 0
