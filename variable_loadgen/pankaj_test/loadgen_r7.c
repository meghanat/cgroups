#include<stdio.h>
#include<time.h>
#include<sys/time.h>
#include<stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
/**
 * it takes required utilization as input from environemt variables which must be set before running the program
 * Inputs are checked for validity 
 * Does not work if application requires more than 1 core
 * Sum of CPU and IO utilization must be 1.0
 * */
float slice = 0.3;
double get_wall_time()
{
	struct timeval time;
	if(gettimeofday(&time,NULL))
	{
		return 0;
	}
	return (double)time.tv_sec + (double)time.tv_usec*0.000001;
}
int main(int argc, char *argv[])
{
	int fd;
	int i;
	clock_t start, end;

	double t_start,t_end;
	double total_time_spent;
	double i_slice;

	double start_wall,end_wall;
	double time_spent;
	
	// Variable declaration for io load generation
	double io_util;
	io_util =  atof(getenv("IO_UTIL"));
	double io_slice;
	double num_runs;
	double io_start,io_end,io_t;
	double io_sleep;
	double io_cpu_start;
	double io_cpu_end;
	int io_count=0;
	int io_num=4;
	double io_cpu_total;
	double io_tune_sleep;
	double r;
	int disk_usage = atof(getenv("DISK_USAGE"));

	char *mem_usage;

	int fd1,fd2;
        int buff=1;
        char line[] ="aaa";
        fd2 = open("hugeinp.txt",O_CREAT|O_RDWR|O_SYNC,0667);

        if(fd2==-1)
                perror("open");
	printf("fd2 = %d\n",fd2);
	char a[50];
	char util[8];

	double utility;
	utility = atof(getenv("CPU_UTIL"));
	printf("cpu util : %f\n",utility);
	float ts = slice*(1-utility)/utility;
	sprintf(a,"cgclassify -g cpuset:core1 %d\n",getpid());
	float num;
	int j=0,k=0;
	io_sleep = ts;
	ts = ts*1000000;

	////////////////////////////////////////////////////////
	int cpu_t = 400000000;
	cpu_t = atoi(getenv("CPU_TIME"));
	double cpuTime = (float)cpu_t/1000000;
	printf("util : %f\t cputime : %f\n",utility,cpuTime);
	int id = 1024;
	id = atoi(getenv("MEMORY"));
	double total=0;
	//Input validation
	if(id <0 || utility<0 || io_util<0 || cpu_t<0 || disk_usage<0 )
	{
		perror("Invalid input: All inputs must be positive");
		exit(0);
	}
	if((utility+io_util) > 1.0)
	{
		perror("Invalid input: cpu_util + io_util > 1.0");
		exit(0);
	}
	//Menory simulation
	mem_usage = (char *)malloc(id);
	// Calculation for io simulation
	num_runs = cpuTime / slice;
	printf("num runs : %f\n",num_runs);
	io_sleep = 0.1;
	double temp1 = (1-io_util)*(1-utility); 
	double temp2 = io_util*utility;
	io_tune_sleep = io_sleep * (1-io_util) - (io_util*utility*io_sleep)/(1-utility);
	io_tune_sleep *= 1000000;
	t_start = get_wall_time();
	while(1)
	{
	////////////////////////////////////////////////////
	io_cpu_start = clock();
	io_t = 0;
	///////////////////////////////////////////
        io_start = get_wall_time();
	usleep(io_tune_sleep);
	///////////////////////////////////////////
        while(io_t < io_sleep)
        {
        if(write(fd2,&line,buff)==-1)
		perror("write");
	io_end = get_wall_time();
	io_t = io_end - io_start;
	io_count++;
	if(io_count >= disk_usage)
	{
		if(lseek(fd2,0,SEEK_SET)< 0)
			perror("lseek");
	}
        }
	start = clock();
	slice = io_t * utility / (1 - utility);// in seconds
	io_cpu_end = clock();
	io_cpu_total = io_cpu_end - io_cpu_start;
	total += io_cpu_total/CLOCKS_PER_SEC; // in seconds
	////////////////////////////////////////////////////
	for(j=0;j<10000000;)
	{
		num = 12/1822*4386384348/579849;
		num = 12/1822*4386384348/579849;
		num = 12/1822*4386384348/579849;
	        
	//code for io simulation
	end = clock();
	num = end-start;
	num = num/CLOCKS_PER_SEC; // in seconds
	if(num>=slice) //num - seconds, slice - seconds
		break;
	}
	
	//update total cpu time 
	total += num;
	if(total>=cpuTime)
		break;

}
	if(remove("hugeinp.txt")==-1)
		perror("remove");
	free(mem_usage);
	return 0;
}
