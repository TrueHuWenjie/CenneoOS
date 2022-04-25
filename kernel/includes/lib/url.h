

#ifndef URL_H_
#define URL_H_

#define URL_MAX_LEN 2048

typedef struct url_prase_info
{
    char *scheme;
    char *hostname;
    char *path;
} URL_PRASE;

int url_praser_free(URL_PRASE *p);
URL_PRASE url_praser(char url[]);


#endif
