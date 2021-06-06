Copyright 2016 Explorer developers.
探索者内核引导程序的保留扇区引导程序部分
Reserved Sector boot section of Explorer kernel loader program.

本引导程序所使用的许可证类型尚不明朗。

功能描述：初步初始化硬件设备、文件系统，读取
引导配置文件，提供可视化界面供用户选择或输出
引导状态信息，最终加载目标所需内核。

本级结构：略

本目录下存在的文件或文件夹：
\Makefile：提供Boot的编译、调试描述。
\README：本文件。用于描述、说明Boot部分。

设计思路及实现原理：
主引导程序（MBR）将保留扇区引导程序加载到内存后，
由保留扇区引导程序进行设备、内存、处理器的初始
化以及其他相关的初始化并提供可视化操作界面，最
后加载存在于分区中的引导配置文件，由引导配置文
件说明内核存在位置，最后将内核加载到指定内存。

可视化界面（Visual Interface）：
1.提供一个具有标题区域、选择或信息输出区域、提
示区域组成的人机交互界面，最终效果需要如本目录
下output.jpg和select.jpg。其中该界面的各种元素，
如字体颜色、各区域百分比大小、各区域颜色、高亮
显示的选择的背景颜色都标记在../configure.h中。
2.可视化界面中的元素分为页和区域。其中部分区域
附属在某些页中。在这里，页分为输出页、选择页，
独立（不属于页中）的区域有标题区域、注意区域，
非独立区域有介绍区域（选择页中）、选择区域（选
择页中），输出页和选择页相互独立，即同一时间内
只能有其中一个页显示在屏幕上。而针对于界面的任
何区域的重编辑，都需要进行重绘工作，其中对于页
和非独立区域的重编辑工作中，需要经过判断目标页
是否是当前的活动页，若是，才需要进行重绘工作。
3.由于输出页、介绍区域、选择区域有重绘的需求，
所以需要对其数据进行储存。

开发者提示：VI.c中所提供的printk函数并没有对参
数数量有限制，若有非常规的输出调用，有可能会出
现堆栈溢出问题。

储存器（Storage）：
1.对计算机的外部储存器管理，并提供有限功能的统
一规范的接口。

开发者提示：进行新型外部储存器的接口开发时，务
必保证至少下列功能的完整实现：读取/写入、初始化
操作。