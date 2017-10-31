#include <linux/types.h>
#include <linux/sched.h>
#include <linux/err.h>
#include <linux/signal.h>
#include <linux/fdtable.h>
#include <linux/list.h>

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