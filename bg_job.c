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




int main(int argc, char const *argv[])
{
	float period=atof(argv[1]);
	float runtime=atof(argv[0])*period;
	printf("exec successful\n");
	printf("util: %s\n",argv[0]);
	printf("period: %s\n",argv[1]);
	printf("runtime %f\n",runtime);

	int job_id;
	srand(time(NULL));
	job_id=rand();

	struct sched_attr attr;
	unsigned int flags = 0;
	attr.size = sizeof(attr);
	attr.sched_flags = 0;
	attr.sched_nice = 0;
	attr.sched_priority = 0;
	attr.sched_policy = SCHED_DEADLINE; 

	attr.sched_deadline =period*1000000;
	attr.sched_period = period*1000000;
	attr.sched_runtime=runtime*1000000;
	int ret = sched_setattr(getpid(), &attr, flags);
	printf("SCHED_DEADLINE RETURN: %d",ret);
	if (ret < 0) {
		done = 0;
		perror("sched_setattr");
		printf("Error!! Job ID: %d",job_id);
		exit(-1);
	}


	float slice = 0.05;
	double start_wall,end_wall;
	clock_t start, end;
	float num;
	double cpu_t = runtime;

	double cpuTime = cpu_t;
	double slice_cput = cpuTime/((double)period/slice);
	double total_sleep_time = (0.95*period)*0.95;
	double slice_sleep = total_sleep_time / ((double)period/slice);
	slice_sleep *= 1000000;
	float total=0;
	double time_spent;
	
	while(1)
	{
		start_wall = get_wall_time();
		start = clock();
		for(int j=0;j<10000000;)
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
		
		// sprintf(util,"%f %f\n",(float)num/(time_spent),total);
		
	}
	printf("BG job ending: %d\n",job_id);

	return 0;
}