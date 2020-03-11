```mermaid

graph BT;

subgraph Hardware Abstract Layer
    gdt["Global Descriptor Table"];
    chips["Chips(CMOS\Intel 8254\Intel 8259...)"];
    cpu["CPU(Features detection"];
    irq["Interrupt"];
    mmu["Memory Management Unit"];
    io["Input\Output"];
end;

subgraph Kernel
    kmm["Kernel Memory Management"];
    fs["File System"];
    kvi["Kernel Visual Interface"];
    mpt["Multi-task(task\thread\process)"];
    ipc["Inter Process Communication"];
    lib["Library"];
end;

subgraph Drivers
    mse["Mouse"];
    kbd["Keyboard"];
    hdd["Hard Disk"];
    gfx["Graphics Card"];
    bus["Bus(PCI\PCIe\USB\Serial ATA..."];
end;

subgraph Graphical User Interface

end;

```
