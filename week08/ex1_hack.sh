gcc ex1.c -o ex1
./ex1 &
sleep 1
pid=$(cat /tmp/ex1.pid)

address=$(cat /proc/$pid/maps | grep heap | awk '{print $1}')

beg=$(echo $address | cut -d'-' -f1)
end=$(echo $address | cut -d'-' -f2)

beg="0x$beg"
end="0x$end"

echo "Memory address: $address"

pass=$(sudo xxd -s $beg -l $end /proc/$pid/mem | grep "pass:")
pass=$(echo $pass | cut -d':' -f3 | cut -c1-8)

echo "Password: $pass"

sudo kill -9 $pid