
#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/workqueue.h>


/* declarando licença e autor */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Diogo Canut F. P.");

/* HANDLER */

static void handler(unsigned long data)
{

  printk("entrando na função task_handler\n");

  printk("saindo da função task_handler\n");

}


DECLARE_TASKLET(tasklet_handler, handler, 0);

/* WORKQUEUE */

struct workqueue_struct *wq = 0;


static void work_handler(struct work_struct *w)
{
  printk("entrando na função work_handler\n");
  msleep(500);
  printk("saindo da função work_handler\n");
}

static void delayed_work_handler(struct work_struct *w)
{
  printk("entrando na função delayed_work_handler\n");
  msleep(500);
  printk("saindo da função delayed_work_handler\n");
}


static DECLARE_WORK(mykmod_work, work_handler); 
static DECLARE_DELAYED_WORK(mykmod_delayed_work, delayed_work_handler);


static int __init tasklets_workqueues_kthreads_init(void)
{
  printk("função init - chamada da tasklet\n");
  tasklet_schedule(&tasklet_handler);
  printk("função init - terminada a chamada da tasklet\n");


	/* WORKQUEUE */
  	printk("função init - chamada das funções de workqueue\n");
	if (!wq)
	  wq = create_workqueue("mykmod");
	if (wq)
	  queue_work(wq, &mykmod_work);
	  queue_delayed_work(wq, &mykmod_delayed_work, 5000);

  printk("função init - terminada a chamada das funções de workqueue\n");

  return 0;
}


static void __exit tasklets_workqueues_kthreads_exit(void)
{
  tasklet_kill(&tasklet_handler);

  if (wq)
        destroy_workqueue(wq);
  printk("FIM!\n");

}

module_init(tasklets_workqueues_kthreads_init);
module_exit(tasklets_workqueues_kthreads_exit);
