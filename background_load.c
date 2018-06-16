#include <stdio.h>
#include <time.h>

int main(int argc, char const *argv[])
{
	int pid;

	while(1){

		pid=fork();
		if(pid==0){

			printf("child\n");
			execl("./bg_job","0.2","100",NULL);
			perror("exec");
		}
		else{

			printf("parent\n" );
			wait();
		}

	}
	return 0;
}