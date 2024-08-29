gcc app/supervisor.c -o app/supervisor
gcc app/worker1.c -o app/worker1
gcc app/components/worker2.c -o app/components/worker2
./app/components/worker2 &
./app/worker1 /home/georgiana/test_practic2/var4/input.txt
