
static rand_num = 0;

unsigned int rand(void)
{
    return rand_num ++;
}