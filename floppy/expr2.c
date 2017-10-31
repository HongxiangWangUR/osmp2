#include "prinfo.h"

int main(int argc,char * const * argv){
	struct prinfo p={0};
	p.pid=getpid();

	if(syscall(181,&p)<0){
		printf("error\n");
		exit(1);
	}

	show_result(&p);
	return 0;
}