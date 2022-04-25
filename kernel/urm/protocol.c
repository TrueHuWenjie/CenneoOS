#include <lib/string.h>
#include <urm.h>

// Default Protocol
// file: File Protocol - Open file with default way
// read: Read Protocol - Read file
// write: Write Protocol - Write file
// mount

#define SCHEME_FILE

SCHEME *urm_scheme_(URM *path, const char *scheme_name)
{
    if (!strcmp(scheme_name, "file"))
    {
        
    }
}

SCHEME *urm_scheme_register(const char *name)
{

}


urm_scheme_cancel(SCHEME *scheme)
{

}
