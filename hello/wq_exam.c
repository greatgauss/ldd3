#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>

#include <linux/time.h>
#include <linux/timer.h>
#include <linux/workqueue.h>
#include <asm/atomic.h>


MODULE_LICENSE("Dual BSD/GPL");


static void do_work(struct work_struct *);
//static void linkwatch_event(struct work_struct *dummy);

static DECLARE_WORK(work_exam_A, do_work);

static struct workqueue_struct *my_workqueue;

struct timer_list my_timer;


void test_timer_fn(unsigned long arg)
{
    my_timer.expires += 3 * HZ;

    add_timer(&my_timer);
    if (queue_work(my_workqueue, &work_exam_A) == 0) {
        printk("Timer: add work queue failed\n");
    }
}
void do_work(struct work_struct *arg)
{
    printk("====work queue run====\n");
    dump_stack();
}

int wq_init(void)
{
    init_timer(&my_timer);
    my_timer.function = test_timer_fn;
    my_timer.expires = jiffies + 3 * HZ;
    add_timer(&my_timer);

    my_workqueue = create_singlethread_workqueue("test-wq");

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

