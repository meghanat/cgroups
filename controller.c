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
	
	FILE *cpuset_t;
	FILE *f;
	FILE *f1;
	FILE *f2;
	FILE *f3;
	FILE *f4;
	FILE *f5;

	char wf[100];
	char *line = NULL;
	char *utilline = NULL;
	char *jobId = NULL;
	char *taskId = NULL;

	size_t len = 0;
	size_t lencpu = 0;
	size_t lenutil = 0;
	size_t lensleep = 0;
	size_t lenjobId = 0;
	size_t lentaskId = 0;

	ssize_t read;

	int flag=0;
	int prev=0;

	int start_time;
	float util;
	char *tcpu=NULL;
	double tcput;
	char *sleep_time = NULL;
	//Code to get utilization
	//util = getUtil();
	//Code for getting start tiem
	//start_time = getStartTime();
	int intr;
	int  pid;
	
	/*sprintf(wf,"/cgroup/cpuset/job%s/tasks",argv[1]);
	printf("%s",wf);	
	cpuset_t = fopen(wf,"a");
	int cpid = getpid();
	fprintf(cpuset_t, "%d\n", cpid);
	printf("%s\n",wf);
	*/
	char cpu_grp[100];
	sprintf(wf,"sudo cgclassify -g cpuset:job%s %d\n",argv[1],getpid());
	sprintf(cpu_grp,"sudo cgclassify -g cpu:group1 %d\n",getpid());
	printf("%s\n",cpu_grp);
	system(cpu_grp);
	// printf("%s\n",wf);
	// printf("bp1\n");
	// //system(wf);
	f = fopen("input/starttime.txt", "r");
	f1 = fopen("input/cpu_time.txt", "r");
	f2 = fopen("input/core_request.txt","r");
	f3 = fopen("input/sleep_time.txt","r");
	f4 = fopen("input/job_id.txt","r");
	f5 = fopen("input/task_id.txt","r");
	//f = fopen("s.txt", "r");
	//f1 = fopen("c.txt", "r");
	//f2 = fopen("u.txt","r");
	printf("bp10\n");	
			
	
	int count =0;
	while(count<350)
	{
		if(!flag)
		{
			getline(&line,&len,f);
			getline(&tcpu,&lencpu,f1);
			getline(&utilline,&lenutil,f2);
			getline(&sleep_time,&lensleep,f3);
	printf("bp12\n");	
			getline(&jobId,&lenjobId,f4);
	printf("\nJobID %s",jobId);
			getline(&taskId,&lenjobId,f5);
			tcput = atof(tcpu);
			util = atof(utilline);

			intr = 0;
			prev = start_time;
			start_time = atoi(line)/10; 	
			//printf("%d\n", start_time);
			flag=1;
		}
		else
		{
			getline(&line,&len,f);
			getline(&tcpu,&lencpu,f1);
			getline(&utilline,&lenutil,f2);
	printf("bp13\n");	
			getline(&sleep_time,&lensleep,f3);
	printf("bp13\n");	
			getline(&jobId,&lenjobId,f4);
			getline(&taskId,&lenjobId,f5);
			tcput = atof(tcpu);
			util = atof(utilline);

			start_time = atoi(line)/10;
			intr = start_time - prev;
			sleep(intr);
			//printf("%d\n",start_time);
		}
		count++;
		printf("bp11\n");	

		char core_request[100];
		char cpu_time[100];
		char taskid[100];
		sprintf(cpu_time,"%f",tcput);
		sprintf(core_request,"%f",util);
		sprintf(taskid,"%d",count);
		printf("starttime :%s\t cycles : %s\t utilization : %s sleep_time : %s\n",line,cpu_time,core_request,sleep_time);
		
		pid = fork();
		
		if(pid==0)
		{
						
			printf("before exec\n");
			//setuid(getuid());
			/*setuid(u);	
			printf("KKKK %d",getuid());
			printf("lll");	*/		
			if(execl("./emulate_job",core_request,cpu_time,taskId,sleep_time,jobId,taskId,NULL)==-1)//change
				perror("exec");
			printf("exec failed\n");
			exit(0);
			/*
			char command[50];

   			strcpy(command, "sudo ./task1_log_deadline.out ");
			strcat(command,core_request);
			strcat(" ",cycles);
			strcat(command," ");
			strcat(command,taskid);
   		
							
			system(command);//change*/
		
		//	printf("exec failed\n");
		//	exit(0);
	

			
		}
		else
		{
			
		}
		prev = start_time;
		//printf("%s",line);
	}
	fclose(f);
	fclose(f1);
	fclose(f2);
	fclose(f3);
	fclose(f4);
	fclose(f5);
	return 0;
}