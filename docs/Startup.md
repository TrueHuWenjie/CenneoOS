```mermaid
graph TD;
    POST自检-->CPU加载MBR到0x7c00
    -->MBR将127个扇区的Loader加载到内存0x1000:0x200中
    -->Loader的setup.bin部分初始化-->Loader的boot.bin部分初始化
    -->Loader将FAT32分区中的KERNEL.BIN加载到0x400000开始的内存中
    -->跳转到KERNEL.BIN中_start函数运行
    -->_start函数调用init_arch函数进行硬件抽象层初始化
    -->_start函数调用main函数进行内核初始化
    -->main函数调用其他功能初始化函数
    -->main函数退化为Idle函数;
