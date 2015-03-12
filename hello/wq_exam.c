#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>

#include <linux/time.h>
#include <linux/timer.h>
#include <linux/workqueue.h>
#include <asm/atomic.h>


MODULE_LICENSE("Dual BSD/GPL");


//static void linkwatch_event(struct work_struct *dummy);

static void work_cb(struct work_struct *);

static DECLARE_WORK(mywork, work_cb);


static struct workqueue_struct *my_workqueue;

struct timer_list my_timer;

void test_timer_fn(unsigned long arg)
{
    my_timer.expires += 3 * HZ;

    add_timer(&my_timer);
    queue_work(my_workqueue, &mywork);
}
void work_cb(struct work_struct *arg)
{
    printk("====EAT EAT====\n");
    dump_stack();
}

int wq_init(void)
{
    init_timer(&my_timer);
    my_timer.function = test_timer_fn;
    my_timer.expires = jiffies + 3 * HZ;
    add_timer(&my_timer);

    my_workqueue = 
        //create_singlethread_workqueue("test-wq");
        //alloc_workqueue("test-wq", WQ_UNBOUND | WQ_MEM_RECLAIM, 1);
        __alloc_workqueue_key("test-wq", WQ_UNBOUND | WQ_MEM_RECLAIM, 1, NULL, NULL);
    printk("test-wq init success\n");

    return 0;
}
void wq_exit(void)
{
    del_timer(&my_timer);

    destroy_workqueue(my_workqueue);
    printk("wq exit success\n");
}
module_init(wq_init);
module_exit(wq_exit);



