#include <linux/kernel.h>
#include <linux/linkage.h>

asmlinkage long sys_helloworld(void){

printk(KERN_ALERT "This is the very official system test written by nick\n");
return 0;

}
