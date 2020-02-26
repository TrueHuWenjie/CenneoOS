# Cenneo Opreating System #
# 玄鸟操作系统 #

*版权所有 2020 玄鸟股份有限公司*

## 简单概要 ##

玄鸟操作系统是一款自主研发设计、跨平台多任务图形化操作系统，版权、著作权归属玄鸟股份有限公司所有。

在下面的详细介绍中，以***[.../cenneo-os-project/](./ "cenneo-os-project")***目录，即
包含这个README.md文件的目录作为本项目的根目录。

## 性能要求 ##
处理器：Intel 80386及以上

内存：9MB或更高

## 版本控制 ##

整个玄鸟操作系统项目（包含引导程序、内核程序、自主工具、系统镜像和说明文档）的代码、镜像文件和二
进制执行文件都托管在玄鸟股份有限公司的项目托管站点
[http://git.cenneo.com](http://git.cenneo.com "玄鸟股份有限公司项目托管站点")，
克隆项目地址如下：

Clone with SSH


    git@git.cenneo.com:HuWenjie/cenneo-os-project.git

Clone with HTTP

    http://git.cenneo.com/HuWenjie/cenneo-os-project.git

## 引导程序 ##

引导程序的根目录位于
***[.../cenneo-os-project/loader/](./loader "cenneo-os-loader")***
，了解更多关于引导程序的信息请参考
***[.../cenneo-os-project/docs/loader/loader.md](./docs/loader/loader.md
"cenneo-os-docs-loader")***
。

## 内核程序 ##

内核程序的根目录位于
***[.../cenneo-os-project/kernel/](./kernel "cenneo-os-kernel")
***。

## 自主工具 ##
鬼鸟文件系统阅读器（GhostBird Filesystem Reader, gfr），是一款为了在Windows和Linux上兼
容具有自主文件系统格式的磁盘、软盘及虚拟介质的工具，通过它我们可以实现对上述介质的扇区、文件系统
读写，为开发调试工作提供了便利，gfr子项目的根目录位于
***[.../cenneo-os-project/tools/gfr](./tools/gfr "GhostBird Filesystem Reader")
***。

## 系统镜像 ##


## 说明文档 ##

## 开发调试 ##
将项目文件从版本控制系统上克隆下来后，进入根目录
***[.../cenneo-os-project/](./ "cenneo-os-project")***。

首先先解压虚拟磁盘映像，使用如下命令：

    make prepare

在终端中输入如下命令开始编译、生成

## 许可协议 ##
