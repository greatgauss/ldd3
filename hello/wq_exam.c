#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>

#include <linux/time.h>
#include <linux/timer.h>
#include <linux/workqueue.h>
#include <asm/atomic.h>


MODULE_LICENSE("Dual BSD/GPL");


//static void linkwatch_event(struct work_struct *dummy);

static struct delayed_work mywork;
static void work_cb(struct work_struct *);


void work_cb(struct work_struct *arg)
{
    printk("====EAT EAT====\n");
	schedule_delayed_work(&mywork, 3 * HZ);
}

int wq_init(void)
{
    INIT_DELAYED_WORK(&mywork, work_cb);
	schedule_delayed_work(&mywork, 3 * HZ);

    printk("test-wq init success\n");

    return 0;
}
void wq_exit(void)
{
	cancel_delayed_work(&mywork);
    printk("wq exit success\n");
}

module_init(wq_init);
module_exit(wq_exit);



