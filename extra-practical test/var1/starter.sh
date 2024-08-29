#!/bin/bash
gcc subordinates/worker2.c -o subordinates/worker2
echo "Worker2 compilat"
gcc subordinates/worker1.c -o subordinates/worker1
echo "Worker1 compilat"
gcc coordonator/supervisor.c -o coordonator/supervisor
echo "Supervisor compilat"
./subordinates/worker1 &
sleep 2
./coordonator/supervisor /home/georgiana/test_practic2/var1/operatii.txt
