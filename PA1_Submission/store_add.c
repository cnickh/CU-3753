#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/uaccess.h>



SYSCALL_DEFINE3(store_add, int, a, int, b, int*, ptr){

	int c = -1;

	c = a + b;
	put_user(c,ptr);

	printk(KERN_ALERT "This is the result of the summation: %d\n", c);
	printk(KERN_ALERT "a: %d\n", a);
	printk(KERN_ALERT "b: %d\n", b);

	return 0;

}
