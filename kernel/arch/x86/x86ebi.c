// Cenneo OS
// /kernel/arch/x86/x86ebi.c
// Explorer Boot Information on x86 Architecture

#include "include/x86types.h"
#include "include/x86ebi.h"
#include <lib/mem.h>

struct boot_info ebi;

void x86ebi_store(void)
{
    memcpy(&ebi, boot_info_ptr, sizeof(ebi));
}
