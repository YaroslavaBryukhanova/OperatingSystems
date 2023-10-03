gcc ex3.c -o ex3
./ex3 3 &
for i in 1 2 3 
do
    pstree $$
    sleep 5
done
./ex3 5 &
for i in 1 2 3 4 5
do
    pstree $$
    sleep 5
done