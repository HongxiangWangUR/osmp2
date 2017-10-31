#include "prinfo.h"

int main (int argc, char ** argv) {
	struct prinfo p={0};//initialize struct to zero
	const struct prinfo empty={0};

	printf("please input the pid:\n");
	while(scanf("%d",&p.pid) != EOF ){
		if(syscall(181,&p)<0){
			printf("syscall error, pleae try again:\n");
			continue;
		}
		/* print the result information */
		show_result(&p);
		p=empty;
	};
	return 0;
}