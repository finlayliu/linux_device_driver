#include <module.h>
#include <init.h>
#include <linux/types.h>	/* size_t */

int scullc_major = SCULLC_MAJOR;
int scullc_devs  = SCULLC_DEVS;		/* number of bare scullc devices */


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

	scullc_devices = 
	return result;
}

void scullc_cleanup(void)
{
	
}

module_init(scullc_init);
module_exit(scullc_cleanup);
