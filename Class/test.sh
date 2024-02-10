ls -1 /bin | head -n 8
ls -1 /bin | head -n 7 | tail -n 1
stat /bin/ls | head -n 2 | tail -n 1 | awk '{print $2}'