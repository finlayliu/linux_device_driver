/******************************************************************
	> File Name: helloworld.c
	> Author: finlay
	> Mail: liufy.thu@gmail.com 
	> Created Time: Sun 20 Jul 2014 11:24:28 HKT
******************************************************************/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <asm/thread_info.h>
#include <asm-generic/delay.h>
#include <linux/kthread.h>
#include <linux/kfifo.h>
#include <linux/idr.h>
#include <linux/interrupt.h>

#define SIZE_4K 0x2000
#define	PARENT_CHILDREN_PID
static void print_taskinfo(struct task_struct *task)
{
	printk(KERN_ALERT "PID:%d\n", task->pid);
	printk(KERN_ALERT "state:%ld\n", task->state);
}

static int list_parent_task(struct task_struct *task)
{
	
	print_taskinfo(task);
	if (task->pid > 1)
		list_parent_task(task->parent);			
	return 0;
}

static int list_child_task(struct task_struct *task, struct list_head *list)
{
	struct task_struct *task1;
	struct pid *p1;
	p1 = find_get_pid(1);
	task1 = get_pid_task(p1, PIDTYPE_PGID);
	print_taskinfo(task1);

	list_for_each(list, &task1->children)
	{
		task = list_entry(list, struct task_struct, sibling);
		print_taskinfo(task);
	}
	return 0;
}

static void print_task(void)
{
	printk(KERN_ALERT "sizeof task_struct is %ld\n", sizeof(struct task_struct));
	printk(KERN_ALERT "sizeof thread_info is %ld\n", sizeof(struct thread_info));
}

static void test_task(void)
{
	struct task_struct *test_task;
	struct list_head *list;
	printk(KERN_ALERT "I bear a charmed life!\n");

	print_task();
	
	test_task = current_thread_info()->task;
	print_taskinfo(test_task);
	set_task_state(test_task, TASK_INTERRUPTIBLE);
	printk(KERN_ALERT "after set:\n");
	print_taskinfo(test_task);
	
	set_task_state(test_task, TASK_RUNNING);
	printk(KERN_ALERT "after set:\n");
	print_taskinfo(test_task);
#ifdef PARENT_CHILDREN_PID
	printk(KERN_ALERT "parent pid:\n");
	list_parent_task(current);
	printk(KERN_ALERT "children pid:\n");
	list_child_task(test_task, list);
#endif	
}

//2014-9-1
static void test_task1(void)
{
	struct task_struct *task;
	for (task = current; task != &init_task; task = task->parent)
		print_taskinfo(task);
}

static void test_task2(void)
{
	struct task_struct *task;
	for_each_process(task)
		printk("%s[%d]\n", task->comm, task->pid);
	printk("current:%s[%d]\n", current->comm, current->pid);
}
struct testtest {
	union {
		int a[2];
		struct te {
			unsigned int ffc:1;
			unsigned int ffd:3;
			unsigned int ffe:2;
			unsigned int fff:2;
			
			unsigned int ffg:3;
			unsigned int ffh:1;
			
			unsigned int ffi:4;
			unsigned int ffj:16;
			
			unsigned int ffk:8;
			unsigned int ffl:12;
			unsigned int ffm:10;
			unsigned int ffn:2;
		} st;
	};
	int bbb;
};


static int test_threadfn(void *data)
{
	char *k = data;
	printk("test_threadfn enter!\n");
	printk("%s\n", k);
	
	struct kfifo fifo;
	unsigned int i, val;
	int ret;
	ret = kfifo_alloc(&fifo, SIZE_4K, GFP_KERNEL);
	
	if (ret)
		return ret;

	/* enqueue [0, 32) to the kfifo named ??fifo?бе */
	for (i = 0; i < 32; i++) {
		kfifo_in(&fifo, &i, sizeof(i));
	print_hex_dump(KERN_DEBUG, "func0:", DUMP_PREFIX_ADDRESS,
	       16, 4, fifo.kfifo.data, 4, true);
	}

	print_hex_dump(KERN_DEBUG, "func:", DUMP_PREFIX_ADDRESS,
	       16, 4, fifo.kfifo.data, 32*4, true);
	
	ret = kfifo_out_peek(&fifo, &val, sizeof(val));
	if (ret != sizeof(val))
		return -EINVAL;
	printk(KERN_INFO "0x%08x\n", val); /* should print 0 */

	while (kfifo_avail(&fifo)) {
		unsigned int val;
		int ret;
		
		/* ... read it, one integer at a time */
		ret = kfifo_out(&fifo, &val, sizeof(val));
		if (ret != sizeof(val))
			return -EINVAL;
		printk(KERN_INFO "0x%08x\n", val);
	}

	print_hex_dump(KERN_DEBUG, "func:", DUMP_PREFIX_ADDRESS,
       16, 4, fifo.kfifo.data, 32*4, true);
	
	return 0;
	
}

static void test_kthread(void)
{
	char *a = "yes,I'm in";
	struct task_struct *k;
	k =	kthread_create(test_threadfn, a, "test_finlay");
	printk("wake up process!\n");
	wake_up_process(k);
}

static int test_map(void)
{
	struct idr idr_huh;
	char *ptr = "hello moto!";
	char *ptr0 = "ok google!";
	char *ptr1;
	int id, id0, ret;
	
	idr_init(&idr_huh);

	do {
		if (!idr_pre_get(&idr_huh, GFP_KERNEL))
			return -ENOSPC;
		ret = idr_get_new(&idr_huh, ptr, &id);
	} while (ret == -EAGAIN);

	do {
		if (!idr_pre_get(&idr_huh, GFP_KERNEL))
			return -ENOSPC;
		ret = idr_get_new(&idr_huh, ptr0, &id0);
	} while (ret == -EAGAIN);

	printk(KERN_ALERT "%s, id is %d, ptr is 0x%p,ptr:%s\n", __func__, id, ptr, ptr);
	printk(KERN_ALERT "%s, id0 is %d, ptr0 is 0x%p,ptr0:%s\n", __func__, id0, ptr0, ptr0);
	
	ptr1 = idr_find(&idr_huh, id);
	printk(KERN_ALERT "%s, ptr1 addr:0x%p, ptr1:%s\n", __func__, ptr1, ptr1);

	ptr1 = idr_find(&idr_huh, id0);
	printk(KERN_ALERT "%s, ptr0 addr:0x%p, ptr0:%s\n", __func__, ptr0, ptr0);
	return 0;
}

static void task1(unsigned long addr)
{
	//char * test_addr = (char *)addr;
	printk(KERN_ALERT"%s:I'm here!!\n", __func__);
	//printk(KERN_INFO"%s:her name is %s\n", __func__, test_addr);
}
static void test_tasklet(void)
{
	struct tasklet_struct task_test;
	printk(KERN_ALERT "%s:begin!!\n", __func__);
	//DECLARE_TASKLET(task_test, task1, (unsigned long)nm);
	tasklet_init(&task_test, task1, 0);
	//udelay(1000);
	//printk(KERN_ALERT "delay\n");
	tasklet_schedule(&task_test);
	printk(KERN_ALERT "%s:end!!\n", __func__);
}

static int hello_init(void)
{	
	test_kthread();	
	test_map();
	test_task();
	test_tasklet();
	test_task1();
	test_task2();

	printk(KERN_ALERT "hello,enter!\n");

	return 0;
}

static void hello_exit(void)
{
	printk(KERN_ALERT "Out, out, brief candle!\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shakespeare");
MODULE_DESCRIPTION("A Hello, World Module");
