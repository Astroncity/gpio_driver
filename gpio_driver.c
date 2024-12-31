#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/slab.h>

#define MAX_BUF_SIZE 4
#define BCM_GPIO_BASE 0x7E000000

static struct proc_dir_entry *proc = NULL;
static char buffer[MAX_BUF_SIZE];

ssize_t gpio_read(struct file *file, char __user *user, size_t count, loff_t *ppos) {
    copy_to_user(user, "Hello from GPIO Driver\n", 23);
    return 23;
}

ssize_t gpio_write(struct file *file, const char __user *buf, size_t size, loff_t *ppos) {
    memset(buffer, 0, MAX_BUF_SIZE);

    if (size > MAX_BUF_SIZE) {
        printk(KERN_WARNING "Buffer size is too large\n");
        size = MAX_BUF_SIZE;
    }

    copy_from_user(buffer, buf, size);
    printk(KERN_INFO "Buffer: %s\n", buffer);
    memset(buffer, 0, MAX_BUF_SIZE);
    return size;
}

static const struct proc_ops gpio_fops = {
    .proc_read = gpio_read,
    .proc_write = gpio_write,
};

static int __init gpio_driver_init(void) {
    printk(KERN_INFO "GPIO Driver Initialized\n");
    proc = proc_create("gpio_driver", 0666, NULL, &gpio_fops);
    return 0;
}

static void __exit gpio_driver_exit(void) {
    printk(KERN_INFO "GPIO Driver Exited\n");
    proc_remove(proc);
}

module_init(gpio_driver_init);
module_exit(gpio_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Astro");
MODULE_DESCRIPTION("GPIO Driver");
MODULE_VERSION("1.0");
