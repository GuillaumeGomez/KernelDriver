#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
static int major = 0; module_param(major, int, 0);
static int nb_dev = 1; module_param(nb_dev, int, 0);

/*
struct mydriver_dev {
	struct cdev cdev;
} mydriver_dev;
*/
/*
static void setup_cdev(struct cdev *m_dev, int minor, struct file_operations *m_fops)
{
	int err, dev = MKDEV(major, minor);
 
	cdev_init(m_dev, m_fops);
	m_dev->owner = THIS_MODULE;
	m_dev->ops = m_fops;
	err = cdev_add(m_dev, devno, 1);
 
	if (err)
		printk(KERN_NOTICE "Error adding cdev: %d\n", err);
}
*/

static int allocate_dev_number(void) {
	int dev, result;
	if (major) {
		dev = MKDEV(major, 0);
		result = register_chrdev_region(dev, nb_dev, "mydriver");
	} else {
		result = alloc_chrdev_region(&dev, 0, nb_dev, "mydriver");
		major = MAJOR(dev);
	}
	if (result < 0) {
		printk(KERN_WARNING "mydriver: can't get major %d\n", major);
	}
	return result;
}

static int __init mydriver_init(void)
{
	allocate_dev_number();
	printk("mydriver: init with major=%d nb_dev=%d\n", major, nb_dev);
	return 0;
}

static void __exit mydriver_exit(void)
{
	int dev = MKDEV(major, 0);
	unregister_chrdev_region(dev, nb_dev);
	printk ("mydriver: exit\n");
	return;
}

/*
struct file_operations mydriver_fops = {
	.owner = THIS_MODULE,
	.read = mydriver_read,
	.write = mydriver__write,
	.open = mydriver_open,
	.release = mydriver_release
};
*/
module_init(mydriver_init);
module_exit(mydriver_exit);

MODULE_LICENSE("GPL");