#!/bin/bash


NUMBER_OF_PARAMETERS="${#}"
if ! [[ "${NUMBER_OF_PARAMETERS}" -eq 3 ]] || ! [[ ${1} =~ ^[+-]?[0-9]+([.][0-9]+)?$ ]] || ! [[ ${2} =~ ^[+-]?[0-9]+([.][0-9]+)?$ ]]
then
    echo "Usage: ${0} START END FILE_NAME"
    exit 1
fi

x=${1}
y=${2}
file_name=${3}

printf "using sed command\n"
var=$(sed -n "${x}, ${y}p" ${file_name})
printf "$var\n"

printf "using read command\n"
i=0
while read -r line; do
    if [[ $i -eq $y ]]; then
        echo -e "$line"
        break
    elif [[ $i -ge $x ]]; then
        (( i++ ))
        echo -e "$line"
    else
        (( i++ ))
    fi
done < "$file_name"

exit 0
