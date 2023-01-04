#!/bin/bash

function digits_sum() {
	local number=$1
	local res=0
	until [[ $number -eq 0 ]] 
	do
		let res+=number%10
		let number/=10
	done
	echo $res
}

read -p "Enter a number: " number
until [[ "$number" -eq "done" ]]; do
	echo "digits sum is $(digits_sum $number)"
	echo "Reverse $(echo $number | rev)"

	read -p "Enter a number: " number
done

