#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/fs.h>

#define DRIVER_NAME	"ECCDriver"
#define DRIVER_CLASS	"ECCDriverClass"

static dev_t major_number = -1;
static struct cdev ECCcdev;
static struct class *ECCclass = NULL;

static int __init init_driver(void) {
    if (alloc_chrdev_region(&major_number, 0, 1, DRIVER_NAME) < 0)
       goto error1;

    // En este momento el controlador tiene asignado un "major number"
    // Podemos consultarlo mirando en /proc/devices

    if((ECCclass = class_create(THIS_MODULE, DRIVER_CLASS)) == NULL)
       goto error2;

    pr_info("ECC driver initialized and loaded\n");
    return 0;

error1:
    pr_err("Major number assignment failed");
    return -1;

error2: 
    pr_err("Class device registering failed");
    return -2;
}

static void __exit exit_driver(void) {
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

