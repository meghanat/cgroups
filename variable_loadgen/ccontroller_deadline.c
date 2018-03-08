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

/*
char * tostring(int number)
    {
        int n = log10(number) + 1;
        int i;
      char *numberArray = calloc(n, sizeof(char));
        for ( i = 0; i < n; ++i, number /= 10 )
        {
            numberArray[i] = number % 10;
        }
        return numberArray;
    }
*/

/**
 * it takes required utilization as input
 * This program loggs the utilizations to the file load.txt
 *
 * */
 
int main(int argc,char *argv[])
{
	
	FILE *cpuset_t;
	FILE *f;
	FILE *f1;
	FILE *f2;

	char wf[100];
	char *line = NULL;
	char *utilline = NULL;

	size_t len = 0;
	size_t lencpu = 0;
	size_t lenutil = 0;

	ssize_t read;

	int flag=0;
	int prev=0;

	int start_time;
	float util;
	char *tcpu=NULL;
	int tcput;
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
	sprintf(wf,"sudo cgclassify -g cpuset:job%s %d\n",argv[1],getpid());
	printf("%s\n",wf);
	printf("bp1\n");
	//system(wf);
	f = fopen("starttime.txt", "r");
	f1 = fopen("cputime.txt", "r");
	f2 = fopen("utilization.txt","r");
	//f = fopen("s.txt", "r");
	//f1 = fopen("c.txt", "r");
	//f2 = fopen("u.txt","r");
	
			
	
	int count =0;
	while(count<20)
	{
		if(!flag)
		{
			getline(&line,&len,f);
			getline(&tcpu,&lencpu,f1);
			getline(&utilline,&lenutil,f2);

			tcput = atoi(tcpu);
			util = atof(utilline);

			intr = 0;
			prev = start_time;
			start_time = atoi(line); 	
			//printf("%d\n", start_time);
			flag=1;
		}
		else
		{
			getline(&line,&len,f);
			getline(&tcpu,&lencpu,f1);
			getline(&utilline,&lenutil,f2);

			tcput = atoi(tcpu);
			util = atof(utilline);

			start_time = atoi(line);
			intr = start_time - prev;
			sleep(intr);
			//printf("%d\n",start_time);
		}
		count++;

		char cutil[100];
		char tpu[100];
		char taskid[100];
		sprintf(tpu,"%d",tcput);
		sprintf(cutil,"%f",util);
		sprintf(taskid,"%d",count);
		printf("starttime :%s\t cputime : %s\t utilization : %s\n",line,tpu,cutil);
		
		pid = fork();
		
		if(pid==0)
		{
						
			printf("before exec\n");
			//setuid(getuid());
			/*setuid(u);	
			printf("KKKK %d",getuid());
			printf("lll");	*/		
			if(execl("./task1_log_deadline.out",cutil,tpu,taskid,NULL)==-1)//change
				perror("exec");
			printf("exec failed\n");
			exit(0);
			/*
			char command[50];

   			strcpy(command, "sudo ./task1_log_deadline.out ");
			strcat(command,cutil);
			strcat(" ",tpu);
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

	return 0;
}
