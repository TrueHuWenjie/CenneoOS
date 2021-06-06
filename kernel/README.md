# Kernel of the Cenneo OS#

*Copyright 2020 CENNEO Ltd.*

## Description of File and Directory ##


| Directory  |          Full name          |           Description            |
| ---------- | --------------------------- | -------------------------------- |
|    arch    |        Architecture         | 硬件架构层，用来抽象底层硬件细节 |
|    cpp     |         C Plus Plus         |         对C++语言的支持          |
|  drivers   |      Device's drivers       |             设备驱动             |
|     fs     |         File System         |             文件系统             |
|    gui     |  Graphical User Interface   |           图形用户界面           |
|  include   |           include           | C\C++语言的函数、常量、结构声明  |
|    init    |       Initialization        |            初始化代码            |
|    ipc     | Inter Process Communication |            进程间通讯            |
|    kmm     |  Kernel Memory Management   |           内核内存管理           |
|    kvi     |   Kernel Visual Interface   |          内核可视化接口          |
|    lib     |           Library           |          其他功能函数库          |
|    osm     |  Operating System Moniter   |          操作系统监视器          |
|   script   |           Script            |           脚本语言支持           |
| KERNEL.BIN |     Kernel binary file      |   生成的内核二进制机器代码文件   |
|  kernel.o  |     Kernel object file      |        生成的内核目标文件        |
|  Makefile  |       Makefile script       |        Make工具的编译脚本        |

## 简单说明 ##

### 硬件抽象层 ###

硬件抽象层将具体架构的特殊初始化过程、特殊硬件进行初始化，并提供相关接口，使得
内核中上层与底层硬件结构分离，便于移植到新的平台。

### C++语言的支持 ###
