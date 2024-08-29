gcc manager/supervisor.c -o manager/supervisor
gcc workers/worker1.c -o workers/worker1
gcc workers/worker2.c -o workers/worker2
./workers/worker2 &
sleep 3
./manager/supervisor perechi.txt
