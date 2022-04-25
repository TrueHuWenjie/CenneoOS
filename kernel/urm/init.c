

#include <urm.h>
#include <stddef.h>

URM *urm_root;

void init_urm(void)
{
    // Create Localhost Node
    urm_root = kmalloc(sizeof(URM));

    urm_root->name = "/";
    urm_root->parent = NULL;
    urm_root->next = NULL;
    urm_root->children = NULL;

    // Create Kernel Resources
}
