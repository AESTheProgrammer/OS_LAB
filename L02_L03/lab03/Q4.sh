read -p "Enter file name: " file
read -p "Enter starting line number: " x
read -p "Enter stopping line number: " y

head -n $y $file | tail -n $(($y - $x + 1))

