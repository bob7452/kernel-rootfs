#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/string.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Anonymous");
MODULE_DESCRIPTION("Sysfs dynamic file create/remove demo");
MODULE_VERSION("0.1");

static struct kobject *hello_kobj;
static bool extra_created = false;

static char value_str[128] = "default";
static char extra_str[128] = "extra data";

static ssize_t value_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) {
    return sprintf(buf, "%s\n", value_str);
}
static ssize_t value_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count) {
    snprintf(value_str, sizeof(value_str), "%.*s", (int)(count - 1), buf);
    return count;
}
static struct kobj_attribute value_attr = __ATTR(value, 0644, value_show, value_store);

static ssize_t extra_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) {
    return sprintf(buf, "%s\n", extra_str);
}
static ssize_t extra_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count) {
    snprintf(extra_str, sizeof(extra_str), "%.*s", (int)(count - 1), buf);
    return count;
}
static struct kobj_attribute extra_attr = __ATTR(extra, 0644, extra_show, extra_store);

static ssize_t enabled_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) {
    return sprintf(buf, "%d\n", extra_created ? 1 : 0);
}
static ssize_t enabled_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count) {
    int val;
    sscanf(buf, "%d", &val);

    if (val == 1 && !extra_created) {
        if (sysfs_create_file(hello_kobj, &extra_attr.attr) == 0) {
            extra_created = true;
            pr_info("extra file created\n");
        } else {
            pr_err("failed to create extra file\n");
        }
    } else if (val == 0 && extra_created) {
        sysfs_remove_file(hello_kobj, &extra_attr.attr);
        extra_created = false;
        pr_info("extra file removed\n");
    }
    return count;
}
static struct kobj_attribute enabled_attr = __ATTR(enabled, 0644, enabled_show, enabled_store);

static struct attribute *attrs[] = {
    &value_attr.attr,
    &enabled_attr.attr,
    NULL,
};

static struct attribute_group attr_group = {
    .attrs = attrs,
};

static int __init hello_init(void) {
    int ret;

    hello_kobj = kobject_create_and_add("hello_sysfs", kernel_kobj);
    if (!hello_kobj)
        return -ENOMEM;

    ret = sysfs_create_group(hello_kobj, &attr_group);
    if (ret) {
        kobject_put(hello_kobj);
        return ret;
    }

    pr_info("hello_sysfs loaded\n");
    return 0;
}

static void __exit hello_exit(void) {
    if (extra_created)
        sysfs_remove_file(hello_kobj, &extra_attr.attr);

    sysfs_remove_group(hello_kobj, &attr_group);
    kobject_put(hello_kobj);

    pr_info("hello_sysfs unloaded\n");
}

module_init(hello_init);
module_exit(hello_exit);

