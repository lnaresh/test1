#include<linux/kernel.h> //kernel defitions
#include<linux/version.h>//kernel versions
#include<linux/module.h> //module definitinos

#include<linux/cdev.h>   //character devices files
#include<linux/device.h> //device specific device nodes

#include<linux/fs.h>     //file operations

#include<linux/err.h>    //err ptr

#include <asm/uaccess.h> //data validations from user,kernel vice versa


#include <linux/slab.h> //creation of a memory block

#define DRV_NAME "MY_MEM"
//static inline int register_chrdev(unsigned int major, const char *name,const struct file_operations *fops)


static unsigned int major = 0;
static struct class* class_ptr=NULL;
static unsigned int minor = 0;
		static char *kbuf ;
		static char *data="kernel data";
/*my_dev_struct{
	char* my_dev_name,
	unsigned int major,
	struct file_operations
}*/
	ssize_t  mem_read(struct file *file, char __user *ubuf , size_t sz, loff_t * oft){
		//copy_to_user(to,from,len)
		int t = 0;
		
		t = copy_to_user(ubuf,data,strlen(data));
		if(t){
			printk("\n[KERN_INFO] in %s t=%d:ERROR IN COPY_TO_USER\n",__func__,t);
			return -1;
		}
			printk("[KERN_INFO] in %s  : ubuf = %s \n",__func__,ubuf);
			printk("[KERN_INFO] in %s  : kbuf = %s \n",__func__,kbuf);

	return sz;
	}
        
	ssize_t mem_write (struct file *file, const char __user *ubuf, size_t sz, loff_t * oft){
		int t=0;
		printk("[KERN_INFO] in %s  : ubuf = %s \n",__func__,ubuf);
		kbuf = kzalloc(sz,GFP_KERNEL);
		t=copy_from_user(kbuf,ubuf,sz);
		if(t){
			printk("\n[KERN_INFO] in %s t=%d:ERROR IN COPY_FROM_USER\n",__func__,t);
			return -1;
		}
			printk("[KERN_INFO] in %s  : ubuf = %s \n",__func__,ubuf);
			printk("[KERN_INFO] in %s  : kbuf = %s \n",__func__,kbuf);
		return sz;
	}
        
	int mem_open (struct inode *inode, struct file *file){
		printk("[KERN_INFO] in %s :MODULE OPEN\n",__func__);	
		return 0;
	}
        
	int mem_release (struct inode *inode, struct file *file){
		printk("[KERN_INFO] in %s :MODULE RELEASED\n",__func__);	
		return 0;
	}

struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = mem_open,
	.release = mem_release,
	.read = mem_read,
	.write = mem_write
};

static int mem_cdev_init(void)
{
	unsigned int ret = register_chrdev(0,DRV_NAME,&fops);
	if(ret < 0){
		printk("[KERN_INFO] in %s :error in register_chrdev\n",__func__);
		goto error;
	}
	major = ret;
	class_ptr=class_create(THIS_MODULE,DRV_NAME);	
	//struct device *device_create(struct class *class, struct device *parent,dev_t devt, void *drvdata, const char *fmt, ...)
	device_create(class_ptr,NULL,MKDEV(major,minor),NULL,DRV_NAME);
		printk("[KERN_INFO] in %s : MODULE LOADED\n",__func__);	
	return 0;
error:
	return ret;
}

static void mem_cdev_exit(void)
{
	//static inline void unregister_chrdev(unsigned int major, const char *name)
	device_destroy(class_ptr,MKDEV(major,minor));
	class_destroy(class_ptr);
	unregister_chrdev(major,DRV_NAME);
	printk("[KERN_INFO] in %s : MODULE REMOVED\n",__func__);	
}
module_init(mem_cdev_init);
module_exit(mem_cdev_exit);
MODULE_LICENSE("GPL");

//if(ERR_PTR(class_ptr)){
//	printk("error in class_ptr\n");
//	ret = -1;	
//	goto error1;
//}

//if(ERR_PTR(class_ptr)){
//	printk("error in class_ptr\n");
//	ret = -1;	
//	goto error2;
//}
//error2:
//	class_destroy(class_ptr);
//error1:
//	unregister_chrdev(major,DRV_NAME);
