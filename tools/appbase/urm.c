
#include <syscall.h>

int urm(void *args, const char *url)
{
    return syscall(SYSCALL_SERVID_URM, SYSCALL_SERVID_URM_CALL, (int) args, (int) url);
}

urm_protocol_register()
{

}
