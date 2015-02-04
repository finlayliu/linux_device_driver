#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/types.h>	/* size_t */
#include <linux/slab.h>		/* kmalloc() */
#include <linux/errno.h>	/* error codes */
#include <linux/fs.h>		/* everything... */
#include <linux/kernel.h>	/* printk() */





int scullc_major = SCULLC_MAJOR;
int scullc_devs  = SCULLC_DEVS;		/* number of bare scullc devices */
int scullc_quantum = SCULLC_QUANTUM;
int scullc_qset =    SCULLC_QSET;



struct scullc_dev *scullc_devices; /* allocated in scullc_init */

struct file_operations scullc_fops = {
	.owner =     THIS_MODULE,
	.llseek =    scullc_llseek,
	.read =	     scullc_read,
	.write =     scullc_write,
	.ioctl =     scullc_ioctl,
	.open =	     scullc_open,
	.release =   scullc_release,
	.aio_read =  scullc_aio_read,
	.aio_write = scullc_aio_write,
};


static void scullc_setup_cdev(struct scullc_dev *dev, int index)
{
	int err, devno = MKDEV(scullc_major, index);
    
	cdev_init(&dev->cdev, &scullc_fops);
	dev->cdev.owner = THIS_MODULE;
	dev->cdev.ops = &scullc_fops;
	err = cdev_add (&dev->cdev, devno, 1);
	/* Fail gracefully if need be */
	if (err)
		printk(KERN_NOTICE "Error %d adding scull%d", err, index);
}


int scullc_init(void)
{
	int result, i;
	dev_t dev = MKDEV(scullc_major, 0);

	if (scullc_major)
		result = register_chrdev_region(dev,scullc_devs,"scullc");
	else {
		result =  alloc_chrdev_region(&dev,0,scullc_devs,"scullc");
		scullc_major = MAJOR(dev);
	}
	if (result)
		return result;

	scullc_devices = kmalloc(scullc_devs*sizeof(struct scullc_dev), GFP_KERNEL);
	if (!scullc_devices) {
		result = -ENOMEM;
		goto fail_malloc;
	}
	
	memset(scullc_devices, 0, scullc_devs*sizeof(struct scullc_dev));
	for (i = 0; i < scullc_devs; i++) {
		scullc_devices[i].quantum = scullc_quantum;
		scullc_devices[i].qset = scullc_qset;
		sema_init(&scullc_devices[i].sem, 1);
		scullc_setup_cdev(scullc_devices + i, i);		
	}
		
		
	return result;
}

void scullc_cleanup(void)
{
	
}

module_init(scullc_init);
module_exit(scullc_cleanup);
