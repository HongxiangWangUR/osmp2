#include "prinfo.h"

char * get_state_info(long state){
	switch(state){
		case TASK_RUNNING:return "process is running";
		case TASK_INTERRUPTIBLE: return "process is interruptible";
		case TASK_UNINTERRUPTIBLE: return "process is uninterruptible";
		case EXIT_DEAD: return "process is dead";
		case EXIT_ZOMBIE: return "process is a zombie";
		case TASK_WAKING: return "process is waking";
		case TASK_PARKED: return "process is parked";
		case TASK_KILLABLE: return "process is killable";
		case TASK_STOPPED: return "process is stopped";
		default: return "unknown";
	}
}

char* get_signal_info(unsigned long signals, char *buf, int size){
	int i=0, len=0;
	char * signames[64]={"SIGHUP","SIGINT","SIGQUIT","SIGILL","SIGTRAP","SIGABRT/SIGIOT",
	"SIGBUS","SIGFPE","SIGKILL","SIGUSR1","SIGSEGV","SIGUSER2","SIGPIPE","SIGALRM","SIGTERM",
	"SIGSTKFLT","SIGCHLD","SIGCONT","SIGSTOP","SIGTSTP","SIGTTIN","SIGTTOU","SIGURG","SIGXCPU",
	"SIGXFSZ","SIGVTALRM","SIGPROF","SIGWINCH","SIGIO/SIGPOLL","SIGPWR","SIGSYS/SIGUNUSED"};

	while(signals>0){
		if( (signals & 1) && signames[i] != NULL ){
			len+=snprintf(buf+len,size-len,"%s, ",signames[i]);
		}
		signals>>=1;
		i++;
	}
	if(i>1){
		buf[len-1]=buf[len-2]='\0';
	}
	return buf;
}


void show_result(struct prinfo * p){
	char buffer[4096]={0};

	printf("the program state is %s\n",get_state_info(p->state));
	printf("the process id is %d\n",p->pid);
	printf("the process parent id is %d\n",p->parent_pid);
	printf("the youngest child child pid is %d\n",p->youngest_child_pid);
	printf("the younger sibling child pid is %d\n",p->younger_sibling_pid);
	printf("the older sibling pid is %d\n",p->older_sibling_pid);
	printf("the process starts time is %lu\n",p->start_time);
	printf("the user mode time is %lu\n",p->user_time);
	printf("the kernel mode time is %lu\n",p->sys_time);
	printf("the total user time of children is %lu\n",p->cutime);
	printf("the total system of children is %lu\n",p->cstime);
	printf("the user id of process owner is %ld\n",p->uid);
	printf("the executed program name is %s\n",p->comm);
	printf("the pending signals are %s \n",get_signal_info(p->signal,buffer,4096));
	printf("the number of open file descriptors is %lu\n",p->num_open_fds);
}