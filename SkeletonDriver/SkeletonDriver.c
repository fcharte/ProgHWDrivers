#include<linux/module.h>
#include<linux/kernel.h>

static int __init init_driver(void) {
    printk(KERN_INFO "Skeleton driver loaded\n");
    return 0;
}

static void __exit exit_driver(void) {
    printk(KERN_INFO "Skeleton driver unloaded\n");
}

MODULE_LICENSE("GPL");

module_init(init_driver)
module_exit(exit_driver)

