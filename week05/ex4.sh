gcc -pthread ex4.c -o ex4
start=`date +%s%N`
./ex4 10000000 1
end=`date +%s%N`
expr $end - $start >> ex4_res.txt

start=`date +%s%N`
./ex4 10000000 2
end=`date +%s%N`
expr $end - $start >> ex4_res.txt

start=`date +%s%N`
./ex4 10000000 4
end=`date +%s%N`
expr $end - $start >> ex4_res.txt

start=`date +%s%N`
./ex4 10000000 10
end=`date +%s%N`
expr $end - $start >> ex4_res.txt

start=`date +%s%N`
./ex4 10000000 100
end=`date +%s%N`
expr $end - $start >> ex4_res.txt