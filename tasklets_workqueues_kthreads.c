
#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <unistd.h>


/* declarando licença e autor */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Diogo Canut F. P.");

static void handler(void);
DECLARE_TASKLET(handler, handler, 0);

static void handler(void){

   printk("entrando na função handler\n");
   tasklet_disable(&handler);
   sleep(5);
   tasklet_enable(&handler);
   printk("saindo da função handler\n");

}


static int __init tasklets_workqueues_kthreads_init(void){

   printk("inicio da init\n");
   tasklet_schedule(&my_tasklet);
   printk("final da init\n");


      

    return 0;

}


static void __exit tasklets_workqueues_kthreads_exit(void){
   tasklet_kill(&my_tasklet);
   printk("Goodbye, tasklet!\n");

}

module_init(tasklets_workqueues_kthreads_init);
module_exit(tasklets_workqueues_kthreads_exit);
