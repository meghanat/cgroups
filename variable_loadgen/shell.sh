gcc -o loader.out task1_logger_comm.c
i=0
for ((i=1;i<=$3;i++))
do
	./loader.out $1 $2 $i $i & 
done
