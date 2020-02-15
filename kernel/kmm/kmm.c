#define KER_MEM_BYTEMAP_SIZE 128*1024

void init_kmm(void)
{
    unsigned long n;
    /**初始化内核内存字节页图*/
    for (n = 0; n < KER_MEM_BYTEMAP_SIZE; n ++)
    {
    	//ker_mem_bytemap[n] == KER_MEM_BYTEMAP_USED | KER_MEM_BYTEMAP_START | KER_MEM_BYTEMAP_END;
    }
}
