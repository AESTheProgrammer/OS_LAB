#!/bin/bash
var1=10
var2=20
VAR=12

if [[ $VAR -gt 10 ]]
then
    echo "The variable is greater than 10."
else
    echo "The variable is equal or less than 10."
fi

if [[ $var1 -gt $var2 ]] 
then
        echo "$var1 is greater than $var2" 
else
        echo "$var2 is greater than $var1" 
fi

counter=0
while [ $counter -lt 10 ]
do
        echo the counter is $counter
        let counter=counter+1
done

sum=0
for VARIABLE in 1 2 3 4 5 .. N
do 
    let sum=sum+VARIABLE
done
echo final sum is $sum

for VARIABLE in armin ali hoosein
do 
        echo $VARIABLE
done

for VARIABLE in `ls -a`
do
        echo item: $VARIABLE
done

function foo(){
        for VARIABLE in `ls -a`
        do
                echo item: $VARIABLE
        done
        echo $1 listed directory
        return 239
}

foo armin
echo $?

exit 1
