gcc -o loader.out task1_logger_comm_log.c
gcc -o log_loader.out cpu_mem_logger.c
rm cpulog0
rm cpulog1
rm cpulog2
rm cpulog3
sudo ./log_loader.out $1 $2 0 0 &
sudo ./log_loader.out $1 $2 1 1 &
sudo ./log_loader.out $1 $2 2 2 &
sudo ./log_loader.out $1 $2 3 3 & 
