#include<stdio.h>
#include<time.h>
#include<sys/time.h>
#include<stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include<fcntl.h>
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
	int pid = fork();
	if(pid!=0)
	{
		double start_wall,end_wall;
		double time_spent;
		char pol[100];
		start_wall = get_wall_time();	
		sprintf(pol,"ps -p %d -o %%cpu >> cpulog%s",pid,argv[3]);
		printf("%s\n",pol);
		while(1)
		{
			usleep(500000);
			//sleep(1);
			system(pol);
			end_wall = get_wall_time();
			if(end_wall-start_wall > 300)
			{
				break;
			}
		}
	}
	else if(pid ==0)
	{
		if(execl("./loader.out",argv[1],argv[2],argv[3],argv[4],NULL)==-1)
		//if(execlp("valgrind","valgrind","--tool=massif","./myapp",NULL)==-1)
				perror("valgrind exec");
			printf("exec failed\n");
			exit(0);
	}
	return 0;
}
