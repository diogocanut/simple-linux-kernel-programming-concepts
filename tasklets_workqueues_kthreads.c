
#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/delay.h>


/* declarando licença e autor */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Diogo Canut F. P.");

static void handler(unsigned long data){

   printk("entrando na função handler\n");
   msleep(5);
   printk("saindo da função handler\n");

}


DECLARE_TASKLET(tasklet_handler, handler, 0);


static int __init tasklets_workqueues_kthreads_init(void){

   printk("inicio da init\n");
   tasklet_schedule(&tasklet_handler);
   printk("final da init\n");


      

    return 0;

}


static void __exit tasklets_workqueues_kthreads_exit(void){
   tasklet_kill(&tasklet_handler);
   printk("Goodbye, tasklet!\n");

}

module_init(tasklets_workqueues_kthreads_init);
module_exit(tasklets_workqueues_kthreads_exit);
