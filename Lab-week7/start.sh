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

if ! test -d executables -a -x executables/program1 -a -x executables/program2
then
gcc executables/program1.c -o program1
fi

if [ $? -ne 0 ]
then 
echo "EROARE" >2
exit 2
gcc executables/program2.c -o program2
fi

if [ $? -ne 0 ]
then 
echo "EROARE" >2
exit 2
fi

if ! test -d instructions -a -r $(find instructions -maxdepth 1)
then 
echo "eroare"
exit 3
fi

if ! test -d database 
then
echo "eroare"
exit 4
fi

