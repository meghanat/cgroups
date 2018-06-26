#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<math.h>
#include<linux/sched.h>
#include<pthread.h>
#include<time.h>
#include<sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include<linux/sched.h>
#include <time.h>
#include <linux/unistd.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <sys/syscall.h>
#include <pthread.h>

 
int main(int argc,char *argv[])
{
	
	
	int  pid;
	
	
	
	while(1)
	{
		
			
		pid = fork();
		
		if(pid==0)
		{
						
			// printf("child\n");
				
			if(execl("./bg_job",argv[1],argv[2],NULL)==-1)//change
				perror("exec");
			printf("exec failed\n");
			exit(0);
			
		}
		else
		{
			// printf("parent\n" );
			wait();
			
		}
	
	}
	
	return 0;
}
