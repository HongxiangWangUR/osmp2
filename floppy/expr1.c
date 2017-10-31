#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(){
	printf("my pid is %d\n",getpid());
	sleep(100);
	return 0;
}