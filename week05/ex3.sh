gcc -pthread ex3.c -o ex3
start=`date +%s%N`
./ex3 10000000 1
end=`date +%s%N`
expr $end - $start >> ex3_res.txt

start=`date +%s%N`
./ex3 10000000 2
end=`date +%s%N`
expr $end - $start >> ex3_res.txt

start=`date +%s%N`
./ex3 10000000 4
end=`date +%s%N`
expr $end - $start >> ex3_res.txt

start=`date +%s%N`
./ex3 10000000 10
end=`date +%s%N`
expr $end - $start >> ex3_res.txt

start=`date +%s%N`
./ex3 10000000 100
end=`date +%s%N`
expr $end - $start >> ex3_res.txt