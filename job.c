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
 
int main(int argc, char *argv[])
{
	printf("BEFORE:%d",getpid());	
		printSchedType();
	      	
		/////////////////////////////////////////////////////
		struct sched_attr attr;
		int x = 0;
		int ret;
		unsigned int flags = 0;
		int P=100;
		float RT = atof(argv[0]) * 1000;
		printf("hhhhhhhhh : %f",RT);
		//printf("deadline thread started [%ld]\n", gettid());

		attr.size = sizeof(attr);
		attr.sched_flags = 0;
		attr.sched_nice = 0;
		attr.sched_priority = 0;
		/* This creates a 10ms/30ms reservation *///nano
		attr.sched_policy = SCHED_DEADLINE; 
		//attr.sched_runtime = RT * 1000 * 1000;
		attr.sched_runtime = RT * 301 * 1000 * 1.001;
		attr.sched_period = attr.sched_deadline = 301 * 1000 * 1000;

		//ret = sched_setattr(getpid(), &attr, flags);
		ret =0;
		printf("#############%d",ret);
		if (ret < 0) {
			done = 0;
			perror("sched_setattr");
			exit(-1);
		}

	/*	while (!done) {
			x++;
		}*/

		//printf("deadline thread dies [%ld]\n", gettid());
		//return NULL;
	 

		///////////////////////////////////////////////////
	
		printf("BEFORE:%d",getpid());	
		printSchedType();
	      	/*
		 struct sched_param rt_param;
		int rt_max_prio = sched_get_priority_max(SCHED_DEADLINE);
		rt_param.sched_priority = rt_max_prio;
		int rc=sched_setscheduler(getpid(),SCHED_DEADLINE, &rt_param);
		*/
	printf("AFTER:%d",getpid());	
		printSchedType();
       
	
	printf("EXEC SUCCESSFULL!!!\n");
	
	
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
	//char io_file_name[20];
	//sprintf(io_file_name,"io_speed%d.txt\n",id);
	float ts;
	int buff=10;
	/*int io_speed_file = open("io_speed.txt",O_CREAT|O_RDWR|O_SYNC,0667);
	char line[] ="aaaaaaaaaa";
	double io_speed_start_tm= get_wall_time();
	for(i=0;i<100;i++)
	if(write(io_speed_file,&line,buff)==-1)
		perror("write");
	double io_speed_end_tm = get_wall_time();
	if(remove("io_speed.txt")==-1)
		perror("remove");

	double io_time_per_byte= (io_speed_end_tm - io_speed_start_tm) / 5000000;

	
	//////////////////////////////////////////////////////////
	*/
	//system("cgset -r cpu.shares=1024 group1");
	//sprintf(a,"sudo cgclassify -g cpu:group1 %d\n",getpid());
	//printf("%s\n",a);
	//system(a);
	float num;
	//FILE *f2;
	//f2 = fopen("load.txt","w");
	int j=0,k=0;
	ts = ts*1000000;

	////////////////////////////////////////////////////////
	double cpu_t = atof(argv[1]);
	//printf("bp1\n");
	//float cpuTime = (float)cpu_t/1000000;
	double cpuTime = cpu_t;
	double slice_cput = cpuTime/((double)300/slice);
	//float cpuTime = (float)cpu_t;
	printf("util : %f\t cputime : %f\n",utility,cpuTime);
	// open output file 
	//open(*id,O_CREAT|0666);
	float total=0;
	t_start = get_wall_time();


	/////////
	/*double disk_usage = atof(argv[4]);
	double total_sleep_time = disk_usage * io_time_per_byte;
	double slice_sleep = total_sleep_time / (cpu_t/slice) ;
	printf("total_sleep_time : %f\n",total_sleep_time);*/
	double total_sleep_time = atof(argv[3]);
	
	double slice_sleep = total_sleep_time / ((double)300/slice);
	printf("slice_sleep = %f\n",slice_sleep);
	printf("utilization : %f\n",slice/(slice+slice_sleep));
	slice_sleep *= 1000000;

	printf("%s\n",argv[4] );
	while(1)
	{
	start_wall = get_wall_time();
	start = clock();
	for(j=0;j<10000000;)
	{
		num = 12/1822*4386384348/579849;
		num = 12/1822*4386384348/579849;
		num = 12/1822*4386384348/579849;
	//	j++;
	//	fprintf(f2,"task1 ping %d",j);
	        
	//end_wall = get_wall_time();
	end = clock();
	num = end-start;
	num = num/CLOCKS_PER_SEC;
	if(num>=slice_cput)
		break;
	}
	total += num;
	//end = clock();
	
	//update total cpu time 
	//total += end - start;
	if(total>=cpuTime)
		break;
	usleep(slice_sleep);
	end_wall = get_wall_time();
	num = end-start;
	num = num/CLOCKS_PER_SEC;
	//if(num==0.1)
	//	break;
	time_spent = end_wall - start_wall;
	//fprintf(f2,"%f\n",(float)num/(time_spent));
	//printf("Wall clock time : %f cpu Time : %f CPU Utilization : %f start : %d end : %d\n",(double)(time_spent),((float)(num)),(float)num/(time_spent),start,end);
	//printf("Wall clock time : %f\tcpu Time : %f\tCPU Utilization : %f\n",(double)(time_spent),((float)(num)),(float)num/(time_spent));
	sprintf(util,"%f %f\n",(float)num/(time_spent),total);
	//printf("%s",util);
	//write(fd,util,9);
	//sleep(1);
	//}
	/*printf("Done - task1 meau\n");
	f = fopen("output1.txt","r");
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	//time_spent = (double)(end-begin);
	fprintf(f,"Execution time t1: %f\nCurrent time t1: %f",time_spent,(double)clock());
	fclose(f);*/
}
	//fclose(f2);
	t_end = get_wall_time();
	total_time_spent = t_end - t_start;
	FILE *op = fopen("wl_resp.txt","a");
	fprintf(op,"%s, %d, %f\n",argv[4],id,total_time_spent);
	fclose(op);
	return 0;
}