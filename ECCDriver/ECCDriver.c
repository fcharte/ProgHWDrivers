#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/fs.h>

#define DRIVER_NAME	"ECCDriver"
#define DRIVER_CLASS	"ECCDriverClass"
#define NUM_DEVICES	3

static dev_t major_number = -1;
static struct cdev ECCcdev[NUM_DEVICES];
static struct class *ECCclass = NULL;

static int ECCopen(struct inode *inode, struct file *file) {
   return 0;
}

static int ECCrelease(struct inode *inode, struct file *file) {
   return 0;
}

static const struct file_operations ECC_fops = {
   .owner = THIS_MODULE,
   .open = ECCopen,
   .release = ECCrelease,
};

static int __init init_driver(void) {
    int n_device;
    dev_t id_device;

    if (alloc_chrdev_region(&major_number, 0, NUM_DEVICES, DRIVER_NAME) < 0)
       goto error1;

    /* En este momento el controlador tiene asignado un "major number"
       Podemos consultarlo mirando en /proc/devices */
    pr_info("%s driver assigned %d major number\n", DRIVER_NAME, MAJOR(major_number));

    if((ECCclass = class_create(THIS_MODULE, DRIVER_CLASS)) == NULL)
       goto error2;

    /* En este momento la clase de dispositivo aparece en /sys/class */
    pr_info("/sys/class/%s class driver registered\n", DRIVER_CLASS);

    for (n_device = 0; n_device < NUM_DEVICES; n_device++) {
       cdev_init(&ECCcdev[n_device], &ECC_fops);

       id_device = MKDEV(MAJOR(major_number), MINOR(major_number) + n_device);
       if(cdev_add(&ECCcdev[n_device], id_device, 1) == -1)
          goto error3;

       if(device_create(ECCclass, NULL, id_device, NULL, DRIVER_NAME "%d", n_device) == NULL)
          goto error3;

       pr_info("Device node /dev/%s%d created\n", DRIVER_NAME, n_device);
    }

    /* En este momento en /dev aparecerán los dispositivos ECCDriverN y en /sys/class/ECCDriver también */

    pr_info("ECC driver initialized and loaded\n");
    return 0;

error1:
    pr_err("Major number assignment failed");
    return -1;

error2: 
    pr_err("Class device registering failed");
    unregister_chrdev_region(major_number, 1);
    return -2;

error3:
    pr_err("Device node creation failed");
    class_destroy(ECCclass);
    unregister_chrdev_region(major_number, 1);
    return -3;
}

static void __exit exit_driver(void) {
    int n_device;

    for (n_device = 0; n_device < NUM_DEVICES; n_device++) {
       device_destroy(ECCclass, MKDEV(MAJOR(major_number), MINOR(major_number) + n_device));
       cdev_del(&ECCcdev[n_device]);
    }
    
    if(ECCclass) 
       class_destroy(ECCclass);

    if(major_number != -1)
       unregister_chrdev_region(major_number, 1);

    pr_info("ECC driver unloaded\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Francisco Charte");
MODULE_VERSION("0.1");
MODULE_DESCRIPTION("Skeleton of a full device driver module");

module_init(init_driver)
module_exit(exit_driver)

