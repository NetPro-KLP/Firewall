
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/kthread.h>

#include <linux/rwlock.h>
#include <linux/rwlock_types.h>

#include "expired.h"
#include "hook.h"

struct task_struct *hooking; 
struct task_struct *expired;

rwlock_t hash_lock;

int init_modules(void)
{
	rwlock_init(&hash_lock);
	hooking = kthread_run(start_hook, NULL, "hooking");
	expired = kthread_run(start_expired, NULL, "expired time");
	return 0;
}
void cleanup_modules(void)
{
	kthread_stop(hooking);
	kthread_stop(expired);

	exit_hook();
	exit_expired();
}

module_init(init_modules);
module_exit(cleanup_modules);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("jangsoopark");
MODULE_DESCRIPTION("soma firewall kernel module");

EXPORT_SYMBOL(hash_lock);