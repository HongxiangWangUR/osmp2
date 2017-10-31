#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/prinfo.h>

/*
 * function decleration goes here
 */
int check_pointer( struct prinfo * pointer);
unsigned long get_fds(unsigned long * array, int size);
unsigned long get_signals(sigset_t * signal);


SYSCALL_DEFINE1(prinfo, struct prinfo *, p){
	int result=0;
	struct task_struct* t=NULL; //this is the pcb
	struct list_head* l=NULL;  //this is the cursor
	struct fdtable* fdt=NULL;

	result=check_pointer(p); //find if it is a valid pointer
	if(result<0){
		return result;
	}

	if(p->pid <=0 ){ //if this is not a valid pid
		result=-EINVAL;
		return result;
	}

	rcu_read_lock(); //read lock the critical section

	/* begin to scan process list */
	for_each_process(t){
		if(task_tgid_vnr(t)==p->pid){//if the pid equals user input
			break;
		}
	}

	/* if we can't find the process */
	if( t==&init_task ){
		result=-ESRCH;
		rcu_read_unlock();
		return result;
	}

	/* get the youngest child and if there is no such process then we set it to 0*/
	p->youngest_child_pid=0;
	list_for_each_prev(l,&t->children){ //traverse the list from back to front
		p->youngest_child_pid=task_tgid_vnr(list_entry(l,struct task_struct,sibling));
		break;
	}

	/* get the younger sibling */
	p->younger_sibling_pid = 0;
	list_for_each(l,&t->sibling){
		p->younger_sibling_pid=task_tgid_vnr(list_entry(l,struct task_struct,sibling));
		break;
	}

	/* get the older sibling */
	p->older_sibling_pid=0;
	list_for_each_prev(l,&t->sibling){
		p->older_sibling_pid=task_tgid_vnr(list_entry(l,struct task_struct, sibling));
		break;
	}
	/* get the start time */
	p->start_time=t->start_time;

	/* get the user time */
	p->user_time=t->utime;

	/* get the system time */
	p->sys_time=t->stime;

	/* get the total user time and total system time of children */
	p->cutime=0;
	p->cstime=0;
	list_for_each(l,&t->children){
		p->cutime += list_entry(l,struct task_struct, sibling)->utime;
		p->cstime += list_entry(l,struct task_struct, sibling)->stime;
	}

	/* get the user id */
	p->uid=rcu_dereference(t->real_cred)->uid.val;

	/* get the program name */
	copy_to_user(p->comm,t->comm, min(sizeof(p->comm),sizeof(t->comm)));
	p->comm[sizeof(p->comm)-1]='\0';

	/* get the number of open file descriptors */
	fdt=rcu_dereference(t->files->fdt);
	p->num_open_fds=get_fds(fdt->open_fds,fdt->max_fds/BITS_PER_LONG);

	/* get the pending signals set */
	p->signal=get_signals(&t->signal->shared_pending.signal);

	/* get the state */
	p->state=t->exit_state != 0?t->exit_state:t->state;

	/* get the parent process id */
	p->parent_pid=task_tgid_vnr(rcu_dereference(t->real_parent));

	/* leave the critical section */
	rcu_read_unlock();
	return result;
}

/*
 * check if the input pointer is valid
 * if it is valid then return 0 and if not return a negative value
 */
int check_pointer( struct prinfo * pointer){

	if(pointer==NULL){ // if the pointer is NULL
		return -EINVAL;
	}
	//find if the pointer is usable
	if(!access_ok(VERIFY_WRITE, pointer, sizeof(*pointer))){
		return -EFAULT;
	}
	return 0;
}

/*
 * Get the number of file descriptors according to the fd table
 */
unsigned long get_fds(unsigned long * array, int size){
	int count=0;
	int i=0;
	for(i=0;i<size;i++){
		int n=array[i];
		while( n>0 ){
			count+= n&1;
			n>>=1;
		}
	}
	return count;
}

/*
 * Get the pending signals
 */
unsigned long get_signals(sigset_t * signal){
	unsigned long result=0;
	int i=0;
	for(i=0;i<_NSIG_WORDS;i++){
		result |= signal->sig[i];
	}
	return result;
}