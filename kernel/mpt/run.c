
#include <lib/string.h>
#include <cfs.h>
#include <lib/elf.h>
#include <kmm.h>
#include <mpt/mpt.h>

/**执行应用程序函数（非POSIX标准）*/
int run_exec(void *arg)
{
	Elf32_Ehdr *elf_header;
	struct exec_file_para *file_para = arg;
	struct process_info *new_process;
	int (*program)(void);
	/**停止调度*/
	disable_schedule();

	/**分配进程结构体*/
	for (new_process = NULL; new_process == NULL; )
		new_process = kmalloc(sizeof(struct process_info), 0);

	/**清空进程结构体*/
	memset(new_process, 0, sizeof(struct process_info));

	paging_pd_fork();

	/**进程赋值*/
	new_process->cr3 = x86_cr3_read();
	new_process->msg_list = NULL;
	new_process->nthread ++;
	new_process->pid = proc_get_pid();

	/**将当前任务与当前进程脱钩*/
	current->info.pptr->nthread --;

	/**将任务升级成进程*/
	current->info.pptr = new_process;

	/**允许调度*/
	enable_schedule();

	file_open(file_para->filename, (void *)0x80048000);

	/**回收结构体*/
	kfree(arg);

	elf_header = (Elf32_Ehdr *)0x80048000;

	program = (int (*)())elf_header->e_entry;

	//x86_cpl_switch(program);

	return program();



	// elf_header->e_ident - 
	// elf_header->e_type - ET_EXEC
	// elf_header->e_machine - EM_386
	
	//if ()

	/**执行应用程序*/
	//elf_header->entry();
}

/**运行任务函数*/
union thread* run(const char *filename, const char *parameters, const int flags)
{
	struct exec_file_para *new_arg;

	/**至少filename字符串指针不能为NULL*/
	if (filename == NULL) return NULL;

	/**分配一个exec_file_para结构体装载执行run_exec线程的参数*/
	for (new_arg = NULL; new_arg == NULL; )
		new_arg = kmalloc(sizeof(struct exec_file_para), 0);

	/**清空exec_file_para结构体防止造成干扰*/
	memset(new_arg, 0, sizeof(struct exec_file_para));

	/**拷贝文件名信息进入exec_file_para中*/
	strncpy(new_arg->filename, filename, 256);

	/**如果有参数则同时拷贝参数*/
	if (new_arg->parameters != NULL)
		strncpy(new_arg->parameters, parameters, 256);

	/**创建新线程运行run_exec*/
	task(&run_exec, new_arg);
}
