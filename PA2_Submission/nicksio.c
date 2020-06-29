#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <asm/uaccess.h>
#include <linux/slab.h>

#define BUFF_SIZE 1024
MODULE_LICENSE("GPL");

MODULE_AUTHOR("NICK HENRY");

static int fileOpen = 0;         

static char *device_buffer;


int nicksio_open(struct inode *inode, struct file *filp){

    fileOpen++;
    printk("number of open instanceds %d\n", fileOpen);

    return 0;
}

int nicksio_close(struct inode *inode, struct file *filp){

    fileOpen--;
    printk("filed closed, number of open instances: %d \n", fileOpen);

    return 0;
}


ssize_t nicksio_read(struct file *filp, char __user *buffer, size_t length, loff_t *offset){

    printk(KERN_ALERT "Now reading/n");

    int maxlen = BUFF_SIZE - *offset;

    int bytes2read, rbytes;

    if(maxlen > length) {
        bytes2read = length;
    }
    else {
        bytes2read = maxlen;
    }

    if(bytes2read = 0) {
        printk(KERN_ALERT "No bytes read \n");
    }


    rbytes = bytes2read - copy_to_user(buffer, device_buffer + *offset, bytes2read);

    printk(KERN_ALERT "%d bytes read", rbytes);


    *offset += rbytes;


    return rbytes;
}


ssize_t nicksio_write(struct file *filp, const char __user *buffer, size_t length, loff_t *offset){
  
	printk(KERN_ALERT "Now writing to the device\n");

	int maxlen = BUFF_SIZE - *offset;

	int bytes2write, BYTES_WROTE;
	if(maxlen > length){
		bytes2write = length;
	} else {
		bytes2write = maxlen;
	}

	BYTES_WROTE = copy_from_user(device_buffer + *offset, buffer, bytes2write);


	printk(KERN_ALERT "%d bytes written\n", BYTES_WROTE);

	*offset += BYTES_WROTE;

	return BYTES_WROTE;
}

loff_t nicksio_seek (struct file *filp, loff_t offset, int whence){
    printk(KERN_ALERT "Now Seeking...\n");
    loff_t nwoff = 0;
    switch(whence) {
        case 0: 
            nwoff = offset;
            break;
        case 1: 
            nwoff = filp->f_pos + offset;
            break;
        case 2: 
            nwoff = BUFF_SIZE - offset;
            break;
    }
    if(nwoff < 0){
        printk(KERN_ALERT "SEEK FAILED: cannot seek prior to buffer start\n");
    }
    if(nwoff > BUFF_SIZE){
        printk(KERN_ALERT "SEEK FAILED: cannot seek past the buffer end\n");
    }
    filp->f_pos = nwoff;
    return nwoff;
}

struct file_operations nicksio_operations = {
	.owner = THIS_MODULE,
    .open = nicksio_open,
    .read = nicksio_read,
    .write = nicksio_write,
    .llseek = nicksio_seek,
    .release = nicksio_close,
};

int nicksio_init(void){

    printk(KERN_ALERT "Device Driver Loaded\n");

    device_buffer = kmalloc(BUFF_SIZE, GFP_KERNEL);

    register_chrdev(333, "nicksio_dev", &nicksio_operations);
    return 0;          
}

void nicksio_exit(void){
    printk(KERN_ALERT "Device Driver Now Killed\n");

	if(device_buffer){
		kfree(device_buffer);
	}

	unregister_chrdev(333, "nicksio_dev");
}


module_init(nicksio_init);
module_exit(nicksio_exit);

