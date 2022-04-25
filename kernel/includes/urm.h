

#ifndef URM_H_
#define URM_H_

typedef struct urm_scheme
{
    char *name;

    unsigned int pid;
    
    struct urm_scheme *next;
} SCHEME;

typedef struct urm_node
{
    char *name;
    
    SCHEME *scheme_list;

    struct urm_node *parent;
    struct urm_node *children;
    struct urm_node *next;
    void *ram_resource;                 // Align 4KB
    unsigned long ram_resource_len;
} URM;

extern URM *urm_root;

URM *urm_locate(URM *root_dir, const char *path);
int urm_delete(URM *path);
URM *urm_create(const char *path, const char *name);
void init_urm(void);

#endif
