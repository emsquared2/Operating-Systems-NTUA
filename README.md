# **Operating Systems - NTUA**
Solutions to the assignments of the course "Operating Systems" of the School of Electrical and Computer Engineering at the National Technical University of Athens during Spring Semester 2022.

# **Lab Exercises**

## **Lab 1**

In this lab, we covered the following topics:
- An introduction to the Linux environment and a brief overview of the file system. 
- Creating and linking object files. 
- Merging two files into a third using read and write system calls.

## **Lab 2**
In this lab on process management and communication, we covered the following topics:
- Creating a process tree using the fork system call.
- Creating an arbitrary process tree based on an input file description.
- Interprocess communication using signals (SIGSTOP/SIGCONT) to display the messages of the arbitrary process tree in depth (DFS).
- Parallel computation of a numerical expression presented in the form of an arbitrary process tree. The expression consists of addition and multiplication operations at the nodes and numerical values at the leaves. To implement this, we used UNIX pipes for interprocess communication.

## **Lab 3**
In this lab we used common synchronization mechanisms to solve synchronization problems in multi-threaded applications based on the POSIX threads standard. We covered:
- Synchronization: a comparison between mutexes and atomic operations.
- Parallel calculation of the Mandelbrot set by progressively increasing number of threads and synchronizing them using semaphores.
- Implementing a synchronization scheme to solve a synchronization problem.

## **Lab 4**
This lab was divided into two parts. In the first part, we explored system calls and basic mechanisms for virtual memory management. In the second part, we modified the Mandelbrot set calculation program from the previous lab (lab3) to use processes instead of threads (pthreads) for parallel calculation of the set. This task was implemented with and without semaphores. The lab covered the following topics:
- System calls and basic mechanisms of the OS for virtual memory management.
- Semaphores over shared memory. 
- Virtual memory utilization to share resources between processes.
- Implementation of an interprocess synchronization scheme.

# **Contributors**
- [@emsquared2](https://github.com/emsquared2)
- [@johnskop7](https://github.com/johnskop7)