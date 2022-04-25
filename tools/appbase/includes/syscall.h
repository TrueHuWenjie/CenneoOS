

#ifndef SYSCALL_H_
#define SYSCALL_H_

#define SYSCALL_SERVID_URM 0
#define SYSCALL_SERVID_MM  1
#define SYSCALL_SERVID_MPT 2
#define SYSCALL_SERVID_IPC 3
#define SYSCALL_SERVID_FS  4
#define SYSCALL_SERVID_NET 5
#define SYSCALL_SERVID_GUI 6

#define SYSCALL_SERVID_URM_CALL 0

int syscall(int service_id, int function_id, int arg1, int arg2);

#endif