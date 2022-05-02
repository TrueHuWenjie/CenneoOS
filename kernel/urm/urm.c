
#include <stddef.h>
#include <lib/string.h>
#include <lib/url.h>
#include <kvi.h>
#include <kmm.h>
#include <urm.h>

void urm_print(const char *path)
{
    int i;
    URM *p;
    int level = 0;

    if (path == NULL) return;

    p = urm_locate(urm_root, path);
    
    printk("Tree of Path \"%s\":\n", path);

    while (1)
    {
        if (p)
        {
            for (i = 0; i < (level - 1); i ++)
                printk("|   ");
            if (level)
                printk("+---");
            if (p->name)
                printk("%s\n", p->name);
        }

        if (p->children)
        {
            p = p->children;
            level ++;
        }else if (p->next)
        {
            p = p->next;
        }else if (p->parent)
        {
            for (;;)
            {
                if (level > 0)
                {
                    p = p->parent;
                    level --;
                }else{
                    return;
                }

                if (p->next)
                {
                    p = p->next;
                    break;
                }
            }
            
        }else{
            return;
        }
    }
}

URM *urm_locate(URM *base_dir, const char *path)
{
    int i;
    int i_slash = -1;
    int thelast = 0;
    URM *retval = base_dir;

    for (i = 0; ; i ++)
    {
        switch (path[i])
        {
            case '/':
            process_slash:
                if (i_slash >= 0)
                {
                    retval = retval->children;
                    for (;;)
                    {
                        if (!retval) return NULL;

                        if (strncmp(retval->name, &path[i_slash + 1], (i - 1) - i_slash))
                        {
                            retval = retval->next;
                        }else{
                            break;
                        }
                    }
                        
                }else{
                    // It is the first slash
                    retval = urm_root;
                }

                if (thelast)
                    return retval;

                i_slash = i;
                break;

            case '\0':
                thelast = 1;
                if (i_slash < 0) i_slash = 0;
                if (path[i - 1] == '/') return retval;
                goto process_slash;
            default:
                break;
        }
    }
}

int urm_delete(URM *path)
{
    URM *path_node, *new_node;

    //path_node = urm_locate(path);
    if (!path_node) return -1;


}

URM *urm_create_dir(const char *path, const char *name)
{
    URL_PRASE info;
    URM *path_node, *new_node;

    info = url_praser(path);
    path_node = urm_locate(urm_root, info.path);
    
    if (!path_node) return NULL;
    url_praser_free(&info);

    new_node = kmalloc(sizeof(URM), 0);
    new_node->name = kmalloc(strlen(name), 0);

    new_node->parent = path_node;
    strcpy(new_node->name, name);
    new_node->scheme_list = NULL;
    new_node->children = NULL;
    new_node->ram_resource = NULL;
    new_node->ram_resource_len = 0;

    if (!path_node->children)
    {
        new_node->next = NULL;
        path_node->children = new_node;

    }else{
        new_node->next = path_node->children;
        path_node->children = new_node;
    }

    return new_node;
}
