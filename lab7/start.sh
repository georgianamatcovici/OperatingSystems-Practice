#!/bin/bash

if [ $# -eq 0 ]
then 
read -p "Dati n " n
read -p "dati varianta de program " exec
fi

if [ $# -eq 1 ]
then
n=$1
read -p "dati varianta de program " exec
fi

if [ $# -eq 2 ]
then
n=$1
exec=$2
fi

if ! test -d programs -a -x programs/program1 -a -x programs/program2
then
gcc programs/program1.c -o program1
if [ $? -ne 0 ]
then 
echo "EROARE" >2
exit 2
gcc programs/program2.c -o program2
if [ $? -ne 0 ]
then 
echo "EROARE" >2
exit 2

if ! test -d instructions -a -r $(find instructions -maxdepth 1)

echo "eroare"
exit 3

if ! test -d database 
echo "eroare"
exit 4
fi


if ! file partajat
then

fi