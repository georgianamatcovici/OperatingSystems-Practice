#!/bin/bash

if [ $# -eq 0 ]
then
read -p "Dati directorul: " dir
else
dir=$1
fi

if [ ! -d $dir ] 
then
echo "Fisierul nu este director "
exit 1
fi

for fis in "$dir"/*.txt
do
nume=$(basename "$fis" .txt)
gzip  -S .zip $fis
done

for fis in  "$dir"/*.sh
do
nume=$(basename "$fis" .sh)
nume_zip="$fis.zip"
zip  $nume_zip $fis
done

