# Compile pager.c
gcc pager.c -o pager

# Compile mmu.c
gcc mmu.c -o mmu
sleep 2
# Run pager and mmu in different terminals
pager_pid=$(cat /tmp/ex2/pager_pid)
sleep 2 # wait for the pager process to start
xterm -e ./pager 4 2
xterm -e ./mmu 4 R0 R1 W1 R0 R2 W2 R0 R3 W2 $pager_pid