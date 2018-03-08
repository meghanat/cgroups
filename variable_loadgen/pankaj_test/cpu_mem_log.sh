rm cpulog
rm massif.out.*
gcc -o cmlog cpu_mem_logger.c
./cmlog
rm cmlog
