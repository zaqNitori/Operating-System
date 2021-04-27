#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/types.h>
#include <linux/slab.h>

#define SETMEMORY(person) person = kmalloc(sizeof(*person),GFP_KERNEL);

static LIST_HEAD(birthday_list);

struct birthday
{
	int day;
	int month;
	int year;
	char *name;
	struct list_head list;
};

void set_birthday(struct birthday *person,int day,int month,int year,char name[])
{
	person->day = day;
	person->month = month;
	person->year = year;
	person->name = name;
	INIT_LIST_HEAD(&person->list);
	list_add_tail(&person->list,&birthday_list);
}

static int simple_init(void)
{
	struct birthday *mio,*aqua,*shion,*ina,*fbk;
	struct birthday *ptr;
	char *name;
	printk(KERN_INFO "Birthday Module\n");	

	SETMEMORY(mio);
	SETMEMORY(aqua);
	SETMEMORY(shion);
	SETMEMORY(ina);
	SETMEMORY(fbk);

	name = "mio\0";
	set_birthday(mio,20,8,2000,name);
	name = "aqua\0";
	set_birthday(aqua,11,1,2001,name);
	name = "shion\0";
	set_birthday(shion,22,2,2002,name);
	name = "ina\0";
	set_birthday(ina,3,3,2003,name);
	name = "FBK\0";
	set_birthday(fbk,4,4,2004,name);

	list_for_each_entry(ptr,&birthday_list,list)
	{
		printk(KERN_INFO "%s -> %d/%d/%d\n",
			ptr->name,ptr->year,ptr->month,ptr->day);	
	}

	return 0;
}

static void simple_exit(void)
{
	struct birthday *ptr, *next;
	printk(KERN_INFO "Remove Module\n");
	list_for_each_entry_safe(ptr, next, &birthday_list, list)
	{
		printk(KERN_INFO "removed %s -> %d/%d/%d\n",
			ptr->name,ptr->year,ptr->month,ptr->day);
		list_del(&ptr->list);	
		kfree(ptr);
	}
}

module_init(simple_init);
module_exit(simple_exit);

MODULE_DESCRIPTION("Simple ");
MODULE_AUTHOR("107590026");
MODULE_LICENSE("GPL");
