#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/random.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "SuperRand"
#define CLASS_NAME "super_rand"

static dev_t first;                 // Global variable for the first device number
static struct cdev c_dev;           // Global variable for the character device structure
static struct class *cl;            // Global variable for the device class

static int my_open(struct inode *i, struct file *f)
{
    printk(KERN_INFO "Driver: open()\n");
    return 0;
}
static int my_close(struct inode *i, struct file *f)
{
    printk(KERN_INFO "Driver: close()\n");
    return 0;
}

ssize_t my_read(struct file *f, char __user *buf, size_t len, loff_t *off)
{
    char temp[4] = {'\0'};
    char result[4] = {'\0'};
    int i = 0;
    int len_temp = 0;
    unsigned char random_number;

    get_random_bytes(&random_number, sizeof(char));
    printk(KERN_INFO "%s: Core random number is %u\n", DEVICE_NAME, random_number);
    if (len < 4)
    {
        return -EFAULT;
    }

    if (random_number != 0)
    {
        //Get random_number (in reverse order)
        while (random_number != 0)
        {
            temp[i] = random_number % 10 + '0';
            random_number = random_number / 10;
            i++;
            len_temp++;
        }

        //Reverse temp to result to match actual result
        i -= 1;
        while (i >= 0)
        {
            result[len_temp - 1 - i] = temp[i];
            i -= 1;
        }
        if (copy_to_user(buf, result, 4))
        {
            return -EFAULT;
        }
    }

    return 0;
}

static ssize_t my_write(struct file *f, const char __user *buf, size_t len, loff_t *off)
{
    printk(KERN_INFO "Driver: write()\n");
    return len;
}

static struct file_operations pugs_fops = //C99 syntax
    {
        .owner = THIS_MODULE,
        .open = my_open,
        .release = my_close,
        .read = my_read,
        .write = my_write};

static int __init super_rand_init(void) /* Constructor */
{
    int err; //Error code, negative number if there is an error
    struct device *dev_ret;

    //Register dev
    if ((err = alloc_chrdev_region(&first, 0, 1, DEVICE_NAME)) < 0)
    {
        return err;
    }

    //Create class simple
    if (IS_ERR(cl = class_create(THIS_MODULE, CLASS_NAME)))
    {
        unregister_chrdev_region(first, 1);
        return PTR_ERR(cl);
    }

    //Create device automatically
    if (IS_ERR(dev_ret = device_create(cl, NULL, first, NULL, DEVICE_NAME)))
    {
        class_destroy(cl);
        unregister_chrdev_region(first, 1);
        return PTR_ERR(dev_ret);
    }

    //Init character device
    cdev_init(&c_dev, &pugs_fops);
    if ((err = cdev_add(&c_dev, first, 1)) < 0)
    {
        device_destroy(cl, first);
        class_destroy(cl);
        unregister_chrdev_region(first, 1);
        return err;
    }

    printk(KERN_INFO "%s: device class created correctly\n", DEVICE_NAME);
    return 0;
}

static void __exit super_rand_exit(void) /* Destructor */
{
    cdev_del(&c_dev);
    device_destroy(cl, first);
    class_destroy(cl);
    unregister_chrdev_region(first, 1);
    printk(KERN_INFO "%s: %s unregistered", DEVICE_NAME, CLASS_NAME);
}

module_init(super_rand_init);
module_exit(super_rand_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Huynh Hong An");
MODULE_DESCRIPTION("My First Character Driver");
