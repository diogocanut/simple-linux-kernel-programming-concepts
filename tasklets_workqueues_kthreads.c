
#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/workqueue.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/kthread.h>


/* problema em sleep no timer e tasklet */


/* declarando licença e autor */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Diogo Canut F. P.");

static unsigned long onesec;

/* HANDLER */

static void handler(unsigned long data)
{

  printk("entrando na função task_handler\n");
  unsigned long j = jiffies;
  pr_info("---- task_handler %u jiffies\n", (unsigned)j);
  printk("saindo da função task_handler\n");

}


DECLARE_TASKLET(tasklet_handler, handler, 0);

/* WORKQUEUE */

struct workqueue_struct *wq = 0;


static void work_handler(struct work_struct *w)
{
  printk("entrando na função work_handler\n");
  unsigned long j = jiffies;
  pr_info("---- work_handler %u jiffies\n", (unsigned)j);
  msleep(500);
  printk("saindo da função work_handler\n");
}

static void delayed_work_handler(struct work_struct *w)
{
  printk("entrando na função delayed_work_handler\n");
  unsigned long j = jiffies;
  pr_info("---- delayed_work_handler %u jiffies\n", (unsigned)j);
  msleep(500);
  printk("saindo da função delayed_work_handler\n");
}


static DECLARE_WORK(mykmod_work, work_handler); 
static DECLARE_DELAYED_WORK(mykmod_delayed_work, delayed_work_handler);


/* KERNEL TIMER */

static void timer_handler(unsigned long data);
DEFINE_TIMER(mytimer, timer_handler, 0, 0);

static void timer_handler(unsigned long data)
{

  printk("entrando na função timer_handler\n");
  
  unsigned long j = jiffies;
  pr_info("---- timer_handler %u jiffies\n", (unsigned)j);
  printk("saindo da função timer_handler\n");

}

/* KTHREAD */

static struct task_struct *td, *td_cpu;

static int thread_handler(void *data)
{
  printk("inicio da função thread_handler\n");
  unsigned long j = jiffies;
  pr_info("---- thread_handler %u jiffies\n", (unsigned)j);
  ssleep(5);
  printk("saindo da função thread_handler\n");
  do_exit(0);
  return 0;
}



/* INIT */

static int __init tasklets_workqueues_kthreads_init(void)
{
  unsigned long j = jiffies;
  onesec = msecs_to_jiffies(1000 * 1);

  pr_info("----Inicio da função init -- %u/%u \n", (unsigned)j, (unsigned)onesec);

   /* TASKLET */

  tasklet_schedule(&tasklet_handler);

  printk("Chamada tasklet realizada\n");
	

  /* WORKQUEUE */

	if(!wq)
  {
    wq = create_workqueue("mykmod");
	}
  
  if(wq)
  {
    queue_work(wq, &mykmod_work);
    queue_delayed_work(wq, &mykmod_delayed_work, 5000);

  }
  

  /* TIMER */

  mod_timer(&mytimer, jiffies + (5*onesec));


  /* KTHREAD */


  td = kthread_run(thread_handler, NULL, "mythread");
  if(td)
    printk("thread criada com sucesso!\n");
  else
    printk("falha na criação da thread\n");

  /* kthread_bind  !! */





  printk("Função init terminada\n");
  return 0;
}


static void __exit tasklets_workqueues_kthreads_exit(void)
{
  tasklet_kill(&tasklet_handler);

  if(wq)
  {
    destroy_workqueue(wq);
  }
  
  del_timer(&mytimer);



  kthread_stop(td);
  printk("Termino da thread\n");

  printk("FIM!\n");

}

module_init(tasklets_workqueues_kthreads_init);
module_exit(tasklets_workqueues_kthreads_exit);
