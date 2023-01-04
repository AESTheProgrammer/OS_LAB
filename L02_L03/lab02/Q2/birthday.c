
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/slab.h>

struct birthday {
	int day;
	int month;
	int year;
	struct list_head list;	
};

struct list_head birthday_list;

struct birthday *newBirthDay(int year, int month, int day) {
	struct birthday *bd = kmalloc(sizeof(struct birthday), GFP_KERNEL);
	bd->day = day;
	bd->month = month;
	bd->year = year;
	return bd;
}

int simple_init(void)
{
    printk(KERN_INFO "Loading Module\n");

    INIT_LIST_HEAD(&birthday_list);

    struct birthday *bd = NULL;
    bd = newBirthDay(1, 2, 2001);
	list_add_tail(&bd->list, &birthday_list);
    bd = newBirthDay(3, 4, 2002);
	list_add_tail(&bd->list, &birthday_list);
    bd = newBirthDay(5, 6, 2003);
	list_add_tail(&bd->list, &birthday_list);
    bd = newBirthDay(7, 8, 2004);
	list_add_tail(&bd->list, &birthday_list);
    bd = newBirthDay(9, 10, 2005);
	list_add_tail(&bd->list, &birthday_list);

    struct birthday *ptr;
	list_for_each_entry(ptr, &birthday_list, list) {
		printk(KERN_INFO "OS Module: Day %d.%d.%d\n", ptr->day, ptr->month, ptr->year);
	}

    return 0;
}

void simple_exit(void)
{
    struct birthday *tmp;
	struct list_head *ptr, *next;

	if (list_empty(&birthday_list)) {
		return;
	}

	list_for_each_safe(ptr, next, &birthday_list){
		tmp = list_entry(ptr, struct birthday, list);
		printk(KERN_INFO "OS Module: Removing %d.%d.%d\n", tmp->day, tmp->month, tmp->year);
		list_del(ptr);
		kfree(tmp);
	}

    printk(KERN_INFO "Removing Module\n");
}

module_init(simple_init);
module_exit(simple_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("simple module");
MODULE_AUTHOR("SGG");
