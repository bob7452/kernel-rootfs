#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TAOHUNG");
MODULE_DESCRIPTION("/proc/hello demo");
MODULE_VERSION("0.1");

#define PROC_NAME "hello"

static int hello_show(struct seq_file *m, void *v) {
    seq_printf(m, "👋 Hello from /proc/%s!\n", PROC_NAME);
    return 0;
}

static int hello_open(struct inode *inode, struct file *file) {
    return single_open(file, hello_show, NULL);
}

static const struct proc_ops hello_fops = {
    .proc_open    = hello_open,
    .proc_read    = seq_read,
    .proc_lseek   = seq_lseek,
    .proc_release = single_release,
};

static int __init hello_init(void) {
    proc_create(PROC_NAME, 0, NULL, &hello_fops);
    pr_info("/proc/%s created\n", PROC_NAME);
    return 0;
}

static void __exit hello_exit(void) {
    remove_proc_entry(PROC_NAME, NULL);
    pr_info("/proc/%s removed\n", PROC_NAME);
}

module_init(hello_init);
module_exit(hello_exit);

