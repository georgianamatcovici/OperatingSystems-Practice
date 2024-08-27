gcc signal2.c -o signal2
echo "" >fis.txt
./signal2 "fis.txt" 2
echo "The content of the file is:" && cat "fis.txt"
