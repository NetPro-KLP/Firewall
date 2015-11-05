
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/kthread.h>

#include "expired.h"
#include "hook.h"

struct task_struct *hooking; 
struct task_struct *expired;

int init_modules(void)
{
	hooking = kthread_run(start_hook, NULL, "hooking");
	expired = kthread_run(start_expired, NULL, "expired time");
	return 0;
}
void cleanup_modules(void)
{
	exit_hook();
	exit_expired();
	kthread_stop(hooking);
	kthread_stop(expired);
}


module_init(init_modules);
module_exit(cleanup_modules);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("jangsoopark");
MODULE_DESCRIPTION("soma firewall kernel module");

