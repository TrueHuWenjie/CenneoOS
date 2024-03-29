// Cenneo OS
// /kernel/include/mpt.h
// Multiple process and task support

#ifndef MPT_H_
#define MPT_H_

#include <stddef.h>
#include <urm.h>

extern union thread *current;

struct mpt_queen
{
	union thread *realtime, *high, *normal, *low;
	union thread *idle, *wait, *sleep, *zombie;
};

extern struct mpt_queen mpt_queen;

#define TASK_NAME_LEN 32

/**进程结构体*/
struct process_info
{
	char name[TASK_NAME_LEN];
	unsigned int pid;
	unsigned long nthread;				/**线程数量*/
	unsigned long cr3;					/**进程的页目录表地址*/
	struct msg *msg_list;				/**消息链表*/
};

/**任务0的联合体定义*/
extern union thread task_0;

// state in thread_info represents one of the following conditions:
#define THREAD_INVALID	0			// Invalid
#define THREAD_REALTIME	1			// Realtime
#define THREAD_HIGH		2			// High
#define THREAD_NORMAL	3			// Normal
#define THREAD_LOW		4			// Low
#define THREAD_IDLE		7			// Idle
#define THREAD_WAIT		8			// Idle
#define THREAD_SLEEP	9			// Sleep
#define THREAD_ZOMBIE	10			// Zombie

/**任务信息结构*/
struct thread_info{
	union thread *next, *prev;			/**任务结构体之间组成双向链表*/
	unsigned long stack;				/**堆栈指针*/
	char name[TASK_NAME_LEN];
	unsigned int tid;
	union thread *father;
	union thread *brother;
	union thread *children;
	struct process_info *pptr;		/**进程结构指针*/
	URM *root_dir;
	URM *work_dir;
	unsigned long counter, time_limit;	/**时间片计时和时间片长度*/
	unsigned long runtime;				/**运行时间*/
	unsigned long lastsecond;
	int state;							/**任务的属性*/
};

/**任务联合体的大小*/
#define THREAD_SIZE	8192

/**任务联合体*/
union thread
{
	struct thread_info info;
	int stack[THREAD_SIZE / sizeof(int)];
};



/**队列管理函数*/
void init_queue(union thread *queue);
void add_queue(union thread *queue, union thread *task);

/*位于\arch\x86\kernel\task\init_stack.asm文件中*/
extern unsigned long task_stack_init(unsigned long stack, int (*function)(void *), void * argument);

/**初始化任务函数*/
void init_mpt(void);

/**创建内核任务线程函数*/
union thread* task(int (*function)(), void *argument);

/**执行文件名及参数*/
struct exec_file_para
{
	char filename[256];
	char parameters[256];
};

/**运行任务函数*/
union thread* run(const char *filename, const char *parameters, const int flags);

int proc_name(const char *name);

int task_name(const char *name);

/**获得当前任务的id*/
union thread* get_id(void);

/**获得父任务的id*/
union thread* get_pid(void);

/**任务结束函数*/
void exit(int code);

/**允许调度函数*/
void enable_schedule(void);

/**禁止调度函数*/
void disable_schedule(void);

/**任务切换函数，使用算法选择下个任务并加载*/
void schedule(void);

/**任务挂起函数*/
void sleep(void);

// Wakeup a thread
void wakeup(union thread* id);

/**exit other task*/
#define	SUCCESS		0
#define	FAIL		-1
long do_exit(union thread* id);

#endif
