#include<stdio.h>
#include<time.h>
#include<sys/time.h>
#include<stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include<fcntl.h>
#include<linux/sched.h>
 #include <string.h>
 #include <time.h>
 #include <linux/unistd.h>
 #include <linux/kernel.h>
 #include <linux/types.h>
 #include <sys/syscall.h>
 #include <pthread.h>
// implements algorithm independent of utilization, we sleep instead of performing IO operations because of the intuition that it will interfere with the experiment with CPU contention 

// input parameters : cpu_utilization, cpu_time, taskID, IO time, jobId 

float slice = 0.05;
double get_wall_time()
{
	struct timeval time;
	if(gettimeofday(&time,NULL))
	{
		return 0;
	}
	return (double)time.tv_sec + (double)time.tv_usec*0.000001;
}
#define gettid() syscall(__NR_gettid)

 #define SCHED_DEADLINE	6

 /* XXX use the proper syscall numbers */
 #ifdef __x86_64__
 #define __NR_sched_setattr		314
 #define __NR_sched_getattr		315
 #endif

 #ifdef __i386__
 #define __NR_sched_setattr		351
 #define __NR_sched_getattr		352
 #endif

 #ifdef __arm__
 #define __NR_sched_setattr		380
 #define __NR_sched_getattr		381
 #endif

 static volatile int done;

 struct sched_attr {
	__u32 size;

	__u32 sched_policy;
	__u64 sched_flags;

	/* SCHED_NORMAL, SCHED_BATCH */
	__s32 sched_nice;

	/* SCHED_FIFO, SCHED_RR */
	__u32 sched_priority;

	/* SCHED_DEADLINE (nsec) */
	__u64 sched_runtime;
	__u64 sched_deadline;
	__u64 sched_period;
 };

 int sched_setattr(pid_t pid,
		  const struct sched_attr *attr,
		  unsigned int flags)
 {
	return syscall(__NR_sched_setattr, pid, attr, flags);
 }

 int sched_getattr(pid_t pid,
		  struct sched_attr *attr,
		  unsigned int size,
		  unsigned int flags)
 {
	return syscall(__NR_sched_getattr, pid, attr, size, flags);
 }

void printSchedType()
{
        int schedType;

        schedType = sched_getscheduler(getpid());
        switch(schedType)
        {
                case SCHED_FIFO:
                printf("Pthread Policy is SCHED_FIFO : %d \n",schedType);
                break;
                case SCHED_OTHER:
                printf("Pthread Policy is SCHED_OTHER : %d \n",schedType);
                break;
                case SCHED_RR:
                printf("Pthread Policy is SCHED_RR : %d \n",schedType);
                break;
               	case SCHED_DEADLINE:
                printf("Pthread Policy is SCHED_DEADLINE : %d \n",schedType);
                break;
                default:
                printf("Pthread Policy is UNKNOWN : %d \n",schedType);

        }
}

int schedule_job(float runtime, float period, float deadline){
	/////////////////////////////////////////////////////
		struct sched_attr attr;
		unsigned int flags = 0;

		attr.size = sizeof(attr);
		attr.sched_flags = 0;
		attr.sched_nice = 0;
		attr.sched_priority = 0;
		
		attr.sched_policy = SCHED_DEADLINE;
		attr.sched_runtime = runtime*1000*1000;
		attr.sched_period = period*1000*1000;
		attr.sched_deadline=deadline*1000*1000;		
		return sched_setattr(getpid(), &attr, flags);



}
 
int main(int argc, char *argv[])
{
	
		
	float runtime=atof(argv[1]);
	float deadline=atof(argv[7]);
	float period=deadline*4/3;
	int ret;
	int jobid=atoi(argv[4]);
	
	do{
		usleep(1000000);
		ret=schedule_job(runtime,period,deadline);
		if(ret<0){
			perror("sched_setattr");
			printf("Error: Job Id: %d\n",jobid );
		}

	}
	while(ret<0);
	printf("\nStarting: Time:%lu, Start Time: %d, Job Id: %d, Period: %f, Runtime: %f, Ratio: %f\n",(unsigned long)time(NULL),atoi(argv[6]),jobid,period,runtime,runtime/period);

	int i;
	clock_t start, end;

	double t_start,t_end;
	double total_time_spent;

	double start_wall,end_wall;
	double time_spent;

	char a[50];
	char util[8];
	float utility = atof(argv[0]);

	//printf("%d\n", utility);
	
	//////////////////////////////////////////////////////////

	//SLEEP TIME CALCULATION -  part of this code must be shifted to the ccontroller program, especially the code involving IO speed of the machine

	//float ts = slice*(1-utility)/utility;
	int id = atoi(argv[2]);

	float ts;
	int buff=10;
	
	float num;
	
	int j=0,k=0;
	ts = ts*1000000;

	////////////////////////////////////////////////////////
	double cpu_t = atof(argv[1]);
	
	double cpuTime = cpu_t;
	double slice_cput = cpuTime/((double)deadline/slice);
	
	//printf("util : %f\t cputime : %f\n",utility,cpuTime);
	
	float total=0;
	t_start = get_wall_time();


	double total_sleep_time = atof(argv[3]);
	
	double slice_sleep = total_sleep_time / ((double)deadline/slice);
	//printf("slice_sleep = %f\n",slice_sleep);
	//printf("utilization : %f\n",slice/(slice+slice_sleep));
	slice_sleep *= 1000000;

	// printf("%s\n",argv[4] );
	while(1)
	{
		start_wall = get_wall_time();
		start = clock();
		for(j=0;j<10000000;)
		{
			num = 12/1822*4386384348/579849;
			num = 12/1822*4386384348/579849;
			num = 12/1822*4386384348/579849;
				
		
			end = clock();
			num = end-start;
			num = num/CLOCKS_PER_SEC;
			if(num>=slice_cput)
				break;
		}
		total += num;
			
		
		if(total>=cpuTime)
			break;
		usleep(slice_sleep);
		end_wall = get_wall_time();
		num = end-start;
		num = num/CLOCKS_PER_SEC;
		
		time_spent = end_wall - start_wall;
		
		sprintf(util,"%f %f\n",(float)num/(time_spent),total);
		
	}
//	printf("\nJob Id:%d, cpu time: %f, actual util:%s, input util:%f \n",jobid,cpuTime,util,runtime/period);
//	system("ps -eo stat,pid,user,command | egrep \"^STAT|^D|^R\"");

	t_end = get_wall_time();
	total_time_spent = t_end - t_start;
	printf("\n Ending: Time:%lu, Start Time: %d,End Time: %f, Job Id: %d, Input Period: %f, Input Runtime: %f, Input Ratio: %f, Actual Runtime: %f, Actual Period: %f, Actual Ratio: %f\n",(unsigned long)time(NULL), atoi(argv[6]),t_end,jobid,period,runtime,runtime/period,total,total_time_spent, total/total_time_spent);
	FILE *op = fopen("wl_resp.txt","a");
	fprintf(op,"%s, %d, %f\n",argv[4],id,total_time_spent);
	fclose(op);
	return 0;
}
