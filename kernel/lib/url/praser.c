
#include <stddef.h>
#include <lib/string.h>
#include <kmm.h>
#include <lib/url.h>

#define URL_PRASER_SCHEME 1
#define URL_PRASER_ROOT 2
#define URL_PRASER_HOST 3
#define URL_PRASER_PORT 4
#define URL_PRASER_PATH 5
#define URL_PRASER_FILE 6

int url_praser_free(URL_PRASE *p)
{
    kfree(p->hostname);
    kfree(p->path);
    kfree(p->scheme);
    return 0;
}

URL_PRASE url_praser(char *url)
{
    URL_PRASE info;
    int i;
    char *scheme_host_divider;
    char *scheme_path_divider;
    char *p_slash, *p_colon;
    int i_slash = 0, i_colon = 0, i_hostname = 0;

    memset(&info, 0, sizeof(info));

    // Regularing URL
    for (i = 0; ; i ++)
    {
        switch (url[i])
        {
            case '\\':
                url[i] = '/';
            case '/':
                i_slash = i;
                if (i_hostname)
                {
                    info.hostname = strndup(&url[i_hostname], i - i_hostname);
                    i_hostname = 0;
                    info.path = strdup(&url[i]);
                }
                break;
            case ':':
                i_colon = i;
                if (!i_slash)
                {
                    info.scheme = strndup(url, i);
                    if ((url[i + 1] == '/') && (url[i + 2] == '/'))
                    {
                        // There is a hostname
                        i += 3;
                        i_hostname = i;
                    }else{
                        // Without hostname
                        info.path = strdup(&url[i + 1]);
                    }
                }else{
                    if (i_hostname)
                    {
                        info.hostname = strndup(&url[i_hostname], i - i_hostname);
                        i_hostname = 0;
                    }
                }
                break;

            case '\0':
                if (i_hostname)
                    info.hostname = strdup(&url[i_hostname]);
                else if (!info.path) info.path = strdup(url);
                return info;

            default:
                break;
        }
    }

    return info;
}

void url_next_node(const char path[], unsigned int *index, unsigned int *length)
{
    unsigned int new_index, new_length;
    *index += *length;

    if (!*index && !*length)
    {
        new_length = 1;
        goto ret;
    }else{
        new_index ++;
    }

    if (((path[*index] == '\\') || (path[*index] == '/')) && (!*index && !*length))
    {
        
    }

    

    for (int i = *index; ; i ++)
    {
        //printk("%c,", path[i]);
        
        switch (path[i])
        {
            
            case '\\':
            case '/':
            case '\0':
                *length = (i - 1) - *index;
                return;
            default:
                
                break;
        }
    }
    error("url_next_node error!");

ret:
    *index = new_index;
    *length = new_length;
    return;
}
