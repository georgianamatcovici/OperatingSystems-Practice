#!/bin/bash
if [ $# -eq 0 ]
then
read -p "Dati litera: " litera
else
litera=$1
fi
#Comentariu
lista=$(grep :202: /home/georgiana/etc-passwd_sanitized.txt | grep ^$litera | cut -d: -f1)

for user in $lista
do
dir_home_html=~$user/html
if [ -r $dir_home_html/index.html -o -r $dir_html_user/index.htm -o -r $dir_html/index.php ]
then
echo "$user are pagina web"
fi
#Comentariu 2
#nbvfcd
done

