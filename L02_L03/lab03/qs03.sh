#!/bin/bash


function reverse() {
    temp=${1}
    let reversed=0
    while [ $temp -ne 0 ]
    do
        reversed=$reversed$((temp%10))
        temp=$((temp/10))
    done
    return $reversed
}


NUMBER_OF_PARAMETERS="${#}"
if ! [[ "${NUMBER_OF_PARAMETERS}" -eq 0 ]]
then
    echo "Usage: ${0}"
    exit 1
fi

sum=0
while [[ true ]]
do
    read -p 'input number: ' user_input
    if [[ ${user_input} == "done" ]]
    then
        break
    elif ! [[ ${user_input} =~ ^[+-]?[0-9]+([.][0-9]+)?$ ]]
    then
        # echo 'Please input a number.'
        continue 
    else
        reverse $user_input
        let sum=sum+$?
    fi
done

echo sum = $sum
exit 0

# 100 has a problem
