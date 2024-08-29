#!/bin/bash

month=$(date | cut -d" " -f2)
year=$(date | cut -d" " -f7)


while true
do
read -p "Dati numele utilizatorului, introduceti q cand ati terminat " us
if [ "$us" == "q" ]
then 
break
else
lista=$(last -n 3 "$us" | grep "$year" | grep "$month" )
if [ "$lista" == "" ]
then
echo "Utilizatorul $us nu s-a conectat niciodată!"
else
echo $lista
fi 
fi
done

