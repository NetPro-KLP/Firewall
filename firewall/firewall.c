
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include "expired.h"
#include "hook.h"

int init_module(void)
{
	kthread_run(init_hook);
	kthread_run(init_expired);
	return 0;
}
void cleanup_modules(void)
{
	kthread_stop(init_hook);
	kthread_stop(init_expired);
}


module_init(init_modules);
module_exit(cleanup_modules);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("jangsoopark");
MODULE_DESCRIPTION("soma firewall kernel module");

