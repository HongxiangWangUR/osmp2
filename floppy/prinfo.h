#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>

/* constant define goes here */
#define TASK_RUNNING 0
#define TASK_INTERRUPTIBLE 1
#define TASK_UNINTERRUPTIBLE 2
#define __TASK_STOPPED 4
#define __TASK_TRACED 8
#define EXIT_DEAD 16
#define EXIT_ZOMBIE 32
#define TASK_DEAD 64
#define TASK_WAKEKILL 128
#define TASK_WAKING 256
#define TASK_PARKED 512
#define TASK_STATE_MAX 1024
#define TASK_KILLABLE (TASK_WAKEKILL | TASK_UNINTERRUPTIBLE)
#define TASK_STOPPED (TASK_WAKEKILL | __TASK_STOPPED)

struct prinfo {
	long state;
	pid_t pid;
	pid_t parent_pid;
	pid_t youngest_child_pid;
	pid_t younger_sibling_pid;
	pid_t older_sibling_pid;
	unsigned long start_time;
	unsigned long user_time;
	unsigned long sys_time;
	unsigned long cutime;
	unsigned long cstime;
	long uid;
	char comm[16];
	unsigned long signal;
	unsigned long num_open_fds;
};

//function declerations goes here
void show_result(struct prinfo * p);
char * get_state_info(long state);
char* get_signal_info(unsigned long signals, char *buf, int size);