
#define SYSCALL_SERVID_URM_CALL 0
int sys_urm(int service_id, int function_id, void *args, char *url)
{
    printk("ooo sys_urm:%s\n", url);
    switch (function_id)
    {
        case SYSCALL_SERVID_URM_CALL:
            break;
        default: return -1;
    }
}
