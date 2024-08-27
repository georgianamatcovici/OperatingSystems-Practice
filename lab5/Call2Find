#!/bin/bash

if [ $# -eq 0 ]
then
read -p "Dati numele directorului de start " dir
[ -z $dir ] && dir="."
else
dir=$1
fi

if [ ! -d $dir -o ! -r $dir ]
then
echo "Eroare"
exit 1
fi

lista=$(find $dir -type f -name "*.sh")

for script in $lista
do
echo $script
grep -n '#' $script | grep -v "$#" | grep -v " #!"

done

