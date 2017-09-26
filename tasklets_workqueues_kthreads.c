
#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/workqueue.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/kthread.h>
#include <asm/topology.h>

/* problema em sleep no timer e tasklet */


/* declarando licenca e autor */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Diogo Canut F. P.");

/* VARIAVEL UTILIZADA PARA TRANSFORMAR JIFFIES EM SEGUNDOS */
static unsigned long onesec;

/* HANDLER */

static void task_handler(unsigned long data)
{
  /* FUNÇAO UTILIZADA PELO HANDLER */
  unsigned long j = jiffies;
  pr_info("---- task_handler %u jiffies\n", (unsigned)j);

}

/* DECLARANDO UMA TASKLET DE NOME TASKLET_HANDLER PARA EXECUTAR A FUNÇAO
TASK_HANDLER, RECEBENDO 0 NO CAMPO DATA */
DECLARE_TASKLET(tasklet_handler, task_handler, 0);

/* WORKQUEUE */

/* PONTEIRO PARA A WORKQUEUE */
struct workqueue_struct *wq = 0;


static void work_handler(struct work_struct *w)
{
  /* FUNÇAO UTILIZADA PELO WORK_HANDLER */
  unsigned long j = jiffies;
  pr_info("---- work_handler %u jiffies\n", (unsigned)j);
  msleep(500);

}


static void delayed_work_handler(struct work_struct *w)
{
  /* FUNÇAO UTILIZADA PELO DELAYED WORK HANDLER */
  unsigned long j = jiffies;
  pr_info("---- delayed_work_handler %u jiffies\n", (unsigned)j);
  msleep(500);

}

/* DECLARANDO O WORKEQUEUE HANDLER E O DELAYED WORKQUEUE HANDLER PARAR EXECUTAR
SUAS RESPECTIVAS FUNÇÕES */
static DECLARE_WORK(workqueue_handler, work_handler); 
static DECLARE_DELAYED_WORK(delayed_workqueue_handler, delayed_work_handler);


/* KERNEL TIMER */


static void t_handler(unsigned long data)
{
  /* FUNÇAO A SER EXECUTADA PELO KERNEL TIMER */
  unsigned long j = jiffies;
  pr_info("---- timer_handler %u jiffies\n", (unsigned)j);

}

/* DEFININDO O TIMER_HANDLER PARA EXECUTAR A FUNÇAO T_HANDLER */
DEFINE_TIMER(timer_handler, t_handler, 0, 0);


/* KTHREAD */

/* PONTEIROS PARA AS KTHREADS */
static struct task_struct *td, *td_cpu;


static int thread_handler(void *data)
{
  /* FUNÇAO EXECUTADA PELA KTHREAD */
  unsigned long j = jiffies;
  pr_info("---- thread_handler %u jiffies\n", (unsigned)j);
  ssleep(5);
  do_exit(0);
  return 0;
}


static int thread_cpu_handler(void *data)
{
  /* FUNÇAO EXECUTADA PELA KTHREAD NA CPU 1 */
  unsigned long j = jiffies;
  pr_info("---- thread_cpu_handler %u jiffies\n", (unsigned)j);
  ssleep(5);
  do_exit(0);
  return 0;
}



/* INIT */

static int __init tasklets_workqueues_kthreads_init(void)
{
  unsigned long j = jiffies;
  /* TRANSFORMANDO JIFFIES EM SEGUNDOS */
  onesec = msecs_to_jiffies(1000 * 1);

  pr_info("----Inicio da funcao init -- %u \n", (unsigned)j);

   /* TASKLET */

  tasklet_schedule(&tasklet_handler);

	
  /* WORKQUEUE */

	if(!wq)
  {
    wq = create_workqueue("workqueue_handler");
	}
  
  if(wq)
  {
    queue_work(wq, &workqueue_handler);
    queue_delayed_work(wq, &delayed_workqueue_handler, 5000);

  }


  /* TIMER */

  mod_timer(&timer_handler, jiffies + (5*onesec));


  /* KTHREAD */


  td = kthread_run(thread_handler, 0, "thread_handler");
  if(td)
  {
    printk("thread criada com sucesso!\n");
  }
  else
  {
    printk("falha na criacao da thread\n");
  }

  /* kthread_bind  !! */

  /* EXECUTANDO KTHREAD NA CPU 1, É UTILIZADA A FUNÇAO CPU_TO_NODE PARA ADEQUAR AO PARAMETRO PEDIDO 
  PELO FUNÇAO KTHREAD */
  td_cpu = kthread_create_on_node(thread_cpu_handler, 0, cpu_to_node(1), "cpu_thread_handler");
  if(td_cpu)
  {
    printk("thread criada com sucesso para executar na cpu 1!\n");
    wake_up_process(td_cpu);
  }
  else
  {
    printk("falha na criacao da thread para executar na cpu 1!\n");
  }



  printk("Funcao init terminada\n");
  return 0;
}


static void __exit tasklets_workqueues_kthreads_exit(void)
{


  tasklet_kill(&tasklet_handler);

  if(wq)
  {
    destroy_workqueue(wq);
  }
  
  del_timer(&timer_handler);



  printk("FIM!\n");

}

module_init(tasklets_workqueues_kthreads_init);
module_exit(tasklets_workqueues_kthreads_exit);
