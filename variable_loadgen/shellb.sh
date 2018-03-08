sudo gcc -o loader.out task1_logger_comm.c
sudo ./loader.out $1 $2 0 0 &
sudo ./loader.out $1 $2 1 1 &
sudo ./loader.out $1 $2 2 2 &
sudo ./loader.out $1 $2 3 3 & 
