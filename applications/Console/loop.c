// Cenneo OS
// /
// Console

#include <string.h>

#define CONSOLE_PATH_LENGTH     256
#define CONSOLE_INPUT_LENGTH    256

void console_print_path(const char *path)
{
    printf("<%s>:", path);
}

// Loop
void console_loop(void)
{
    unsigned char path[CONSOLE_PATH_LENGTH];
    unsigned char input[CONSOLE_INPUT_LENGTH];
    unsigned int input_len;
    unsigned int input_insert;
    unsigned char key;


    path[0] = '/';
    path[1] = 0x00;
    input_len = 0;
    input_insert = 0;

    kvi_clear();

    printf("Cenneo OS Console\n");
    
    console_print_path(path);

    while (1)
    {
        key = get_key();
        switch (key)
        {
            case '\0':
                break;
            case '\b':
                if (input_insert)
                {
                    input_len --;
                    input_insert --;
                    memmove(&input[input_insert], &input[input_insert + 1], input_len - input_insert);
                }
                break;
            
            case '\r':
                kvi_put_char('\n');
                printf("cmd:%.*s,len:%d\n", input_len, input, input_len);
                input_len = 0;
                input_insert = 0;
                console_print_path(path);
                break;

            default:
                if (input_len < CONSOLE_INPUT_LENGTH)
                {
                    if (input_insert == input_len)
                    {
                        input[input_insert] = key;
                        kvi_put_char(key);
                    }

                    //memmove(&input[input_insert], &input[input_insert + 1], input_len - input_insert);
                    
                    input_insert ++;
                    input_len ++;
                }
                
                break;
        }
    }
}
