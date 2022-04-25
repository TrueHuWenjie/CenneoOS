/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Function main
 * Explorer/ipc/msg.c
 * version:Alpha
 * 1/24/2015 2:27 AM
 */

/**
 * 消息机制实现 2015.
 * Explorer消息机制属于进程间通信方法之一
 * 发送消息的进程需要提供消息type，这个变量
 * 是一个32位无符号整数，用于说明消息的类别。
 * 其中0x00000000~0x7fffffff属于系统发送信息，
 *     0x80000000~0xffffffff属于普通进程发送信息。
 */

#include <lib/string.h>
#include <syscalls.h>
#include <kmm.h>
#include <stddef.h>
#include <stdlib.h>
#include <mpt/mpt.h>
#include <mpt/ipc.h>

/**消息内容的最大长度*/
#define MAX_MSG		4096

/**消息结构体*/
struct msg
{
	struct msg *next;								// 指向下一个消息结构体
	struct process_info *sender, *receiver;			// 发送者和接收者
	void *message;									// 消息指针
	size_t size;									// 消息长度
	unsigned int type;								// 消息的类别
};


/**内核级发送消息函数（type允许从0x00000000~0xffffffff）*/
int msg_send(struct process_info *receiver, unsigned int type, void *msg, size_t size)
{
	/**分配新的区域存放消息*/
	if (size > MAX_MSG) error("msg error!");		/**判断消息长度是否超过限制*/
	void *temp_msg;
	temp_msg = kmalloc(size, 0);
	if (temp_msg == NULL) error("msg error!");

	/**分配新的消息结构*/
	struct msg *new_msg;
	new_msg = kmalloc(sizeof(struct msg), 0);
	if (new_msg == NULL) error("msg error!");

	/**拷贝消息*/
	memcpy(temp_msg, msg, size);

	/**填充消息结构*/
	new_msg->sender = current->info.pptr;
	new_msg->receiver = receiver;
	new_msg->message = temp_msg;
	new_msg->size = size;
	new_msg->type = type;

	/**将该消息加入目标任务的消息队列上*/
	struct msg *list_point;
	for (list_point = receiver->msg_list; list_point->next != NULL; list_point = list_point->next);
	list_point->next = new_msg;
	new_msg->next = NULL;

	/**正常返回*/
	return 0;
}

/**消息系统调用子功能定义*/
#define MSG_SEND	1

/**用户级消息发送函数（系统调用）*/
int sys_ipc(struct context context)
{
/**功能调用判断*/
	switch(context.ebx)
	{
		case MSG_SEND	: goto msg_send;
		default			: goto finish;
	}

/**
 * 发送消息处理
 * ebx = MSG_SEND, ecx = size of msg, edx = ptr of msg, esi = type, edi = object
 * 返回值存放在eax中，为处理状态
 */
msg_send:
	/**对参数进行判断*/
	if (context.esi >= 0x80000000) return -1;

	/**调用消息发送函数*/
	context.eax = msg_send((struct process_info *)context.edi, context.esi, (void *)context.edx, context.ecx);

/**完成处理*/
finish:
	/**正常返回*/
	return;
}

/**
 * 接收消息函数
 * 该函数取出消息并从消息队列移除此消息
 * 返回值：1 - 成功；0 - 无消息；-1 - 消息过大
 */
int msg_receive(void *ptr, size_t msg_size)
{
	struct msg *msg_list = current->info.pptr->msg_list;

	/**检查是否存在消息*/
	if (msg_list == NULL) return 0;

	/**判断消息的长度是否超过接收消息的长度限制*/
	if (msg_list->size > msg_size) return -1;

	/**将消息拷贝出来*/
	memcpy(ptr, msg_list->message, msg_list->size);

	/**从当前任务中移除这个消息*/
	current->info.pptr->msg_list = msg_list->next;

	/**回收内存*/
	kfree(msg_list->message);
	kfree(msg_list);

	/**正常返回*/
	return 1;
}
