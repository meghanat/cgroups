#include<stdio.h>
#include<time.h>
#include<sys/time.h>
#include<stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include<fcntl.h>
/**
 * it takes required utilization as input
 * This program loggs the utilizations to the file load.txt
 *
 * */
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
int main(int argc, char *argv[])
{
	//printf("EXEC SUCCESSFULL!!!\n");
	int fd;
	fd = open("load_util.txt",O_RDWR);
	//printf("bp1\n");
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
	float ts = slice*(1-utility)/utility;//Why?
	char wf[100];
	int n=atoi(argv[3]);
	sprintf(wf,"sudo cgclassify -g cpuset:core%d %d\n",n,getpid());//system("cgset -r cpu.shares=1024 group1");
	sprintf(a,"sudo cgclassify -g cpu:group2 %d\n",getpid());
	printf("%s\n",a);
	//system(a);
	//system(wf);
	printf("%s\n",wf);
	float num;
	//FILE *f2;
	//f2 = fopen("load.txt","w");
	int j=0,k=0;
	ts = ts*1000000;

	////////////////////////////////////////////////////////
	int cpu_t = atoi(argv[1]);
	//printf("bp1\n");
	float cpuTime = (float)cpu_t/1000000;
	printf("util : %f\t cputime : %f\n",utility,cpuTime);
	// open output file 
	int id = atoi(argv[2]);
	//open(*id,O_CREAT|0666);
	float total=0;
	t_start = get_wall_time();
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
	if(num>=slice)
		break;
	}
	total += num;
	//end = clock();
	
	//update total cpu time 
	//total += end - start;
	total=0;
	//if(total>=cpuTime)
	//	break;
	usleep(ts);
	end_wall = get_wall_time();
	num = end-start;
	num = num/CLOCKS_PER_SEC;//get seconds
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
	//FILE *op = fopen("wl_resp.txt","a");
	//fprintf(op,"%d, %f\n",id,total_time_spent);
	//fclose(op);
	return 0;
}
