
#include <cnos_urm.h>
#include <cnos_scheme.h>



// Loop
void console_loop(void);

int main(int argc, char *argv[])
{
    struct scheme_record_args sra;

    // Check Parameters
    // if (argc != 2) return -1;

    sra.level = SCHEME_RECORD_LEVEL_NOTE;
    sra.info = "This is an application!";
    sra.size = 30;
    urm(&sra, "record:/kernel/log");

    // Framebuffer
    
    // Keyboard
    // Run
    
    console_loop();
}

