mkdir ∼/week01
cd ~/week01
find /usr/bin -name "*gcc*" | sort -r | tail -n 5 > ex1.txt
cat ex1.txt