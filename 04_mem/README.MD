# 4. Memory Management
## 4.1. Debugging (`gdb`)
```
g++ -g main.cpp
```
Then
```
gdb a.out
```
[GDB cheat sheet](./GDB_Cheat_Sheet.pdf) gives you more detail about debugging using gdb.
[Source](https://darkdust.net/files/GDB%20Cheat%20Sheet.pdf)

## 4.2. Types of Computer Memory

![Memory Hierarchy](./images/mem_hierarchy.png)

## 4.3. Cache Memory
System architecture diagram showing caches, ALU (arithmetic logic unit), main memory, 
and the buses connected each component:

![System Architecture Diagram Showing Cache Levels](./images/sys_arch_diag_cache.png)

The concept of L1 and L2 (and even L3) cache is further illustrated by the following 
figure, which shows a multi-core CPU and its interplay with L1, L2 and L3 caches:

![L1, L2, L3 Cache](./images/l1_l2_l3_cache.png)

In the above figure:
* L1d and L1i represent memory space for data and instructions respectively.
* Size
  * L1: 16 to 64 kBytes
  * L2: at or below 2 megabytes
* **Level 3 Caches** is shared among all cores of a multicore processor. With the L3 cache, 
  the [cache coherence](https://en.wikipedia.org/wiki/Cache_coherence) protocol of multicore 
  processors can work much faster. This protocol compares the caches of all cores to maintain
  data consistency so that all processors have access to the same data at the same time. 
  The L3 cache therefore has less the function of a cache, but is intended to simplify and 
  accelerate the cache coherence protocol and the data exchange between the cores.

Command to know cache information on linux:
```
lscpu | grep cache
```

## 4.3.1. Temporal and Spatial Locality
* **Temporal Locality** means that address ranges that are accessed are likely to be used 
  again in the near future. In the course of time, the same memory address is accessed relatively 
  frequently (e.g. in a loop). This property can be used at all levels of the memory hierarchy to 
  keep memory areas accessible as quickly as possible.
* **Spatial Locality** means that after an access to an address range, the next access to an 
  address in the immediate vicinity is highly probable (e.g. in arrays). In the course of time, 
  memory addresses that are very close to each other are accessed again multiple times. This 
  can be exploited by moving the adjacent address areas upwards into the next hierarchy level 
  during a memory access.

## 4.4. Virtual Memory
### 4.4.1. Problems with Physical Mermory
* The idea of virtual memory stems back from a (not so long ago) time, when the random access 
  memory (RAM) of most computers was severely limited. Programers needed to treat memory as a 
  precious resource and use it most efficiently. Also, they wanted to be able to run programs 
  even if there was not enough RAM available.
* **Holes in address space**: If several programs are started one after the other and then 
  shortly afterwards some of these are terminated again, it must be ensured that the freed-up 
  space in between the remaining programs does not remain unused. If memory becomes too fragmented, 
  it might not be possible to allocate a large block of memory due to a large-enough free contiguous 
  block not being available any more.
* **Programs writing over each other**: If several programs are allowed to access the same memory 
  address, they will overwrite each others' data at this location. In some cases, this might even 
  lead to one program reading sensitive information (e.g. bank account info) that was written by 
  another program. This problem is of particular concern when writing concurrent programs which 
  run several threads at the same time.

The basic idea of virtual memory is to separate the addresses a program may use from the addresses 
in physical computer memory. By using a mapping function, an access to (virtual) program memory 
can be redirected to a real address which is guaranteed to be protected from other programs.

![Virtual Memory Idea](./images/virtual_mem.png)

With virtual memory, the RAM acts as a cache for the virtual memory space which resides on 
secondary storage devices.

1) In a nutshell, virtual memory guarantees us a fixed-size address space which is largely independent 
of the system configuration. Also, 2) the **OS** guarantees that the virtual address spaces of different 
programs do not interfere with each other.

* A **memory page** is a number of directly successive memory locations in virtual memory defined by the 
  computer architecture and by the operating system. The computer memory is divided into memory pages of 
  equal size. The use of memory pages enables the operating system to perform virtual memory management. 
  The entire working memory is divided into tiles and each address in this computer architecture is interpreted 
  by the *Memory Management Unit (MMU)* as a logical address and converted into a physical address.
* A **memory frame** is mostly identical to the concept of a memory page with the key difference being 
  its location in the physical main memory instead of the virtual memory.

  ![Memory Page and Frame](./images/mem_page_frame.png)

  As can be seen, both processes have their own virtual memory space. Some of the pages are mapped to 
  frames in the physical memory and some are not. If process 1 needs to use memory in the memory page 
  that starts at address 0x1000, a page fault will occur if the required data is not there. The memory 
  page will then be mapped to a vacant memory frame in physical memory. Also, note that the virtual 
  memory addresses are not the same as the physical addresses. The first memory page of process 1, 
  which starts at the virtual address 0x0000, is mapped to a memory frame that starts at the physical 
  address 0x2000.

  In summary, virtual memory management is performed by the operating system and programmers do usually 
  not interfere with this process. The major benefit is a unique perspective on a chunk of memory for 
  each program that is only limited in its size by the architecture of the system (32 bit, 64 bit) and 
  by the available physical memory, including the hard disk.