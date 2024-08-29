gcc master/supervisor.c -o master/supervisor
gcc slaves/worker1.c -o slaves/worker1
gcc slaves/worker2.c -o slaves/worker2
./slaves/worker2 &
sleep 3
./master/supervisor /home/georgiana/test_practic2/var3/input.txt
# ./master/supervisor /home/georgiana/test_practic2/var3/input.txt
# ./slaves/worker2
