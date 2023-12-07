Instructions
This assignment will make you more familiar with how to manage virtual memory in user programs using the Unix system call interface. For this assignment, you need to modify the code in mmap.c and implement the signal handler for page fault.

The program maintains a very large table of square root values in virtual memory. However, the table is too large to fit in physical RAM (we have set a rlimit on the virtual memory size), hence whenever we access the square root of a number that is not there in the table there is a SEGFAULT. This SEGFAULT is captured using a signal handler set during the initialization phase of the program. Now, the task is to allocate some (you can allocate page_size amount) memory by first calculating the correct offset (within the large square root table) of the page fault. Once the memory is allocated, the square root values should be computed for the table's address range (new memory block) corresponding to page faults that occurred.

Your job is to implement the demand faulting mechanism using a signal handler and UNIX memory mapping system calls. To stay within the physical RAM limit, we suggest using the simple strategy of unmapping the last page whenever a new page is faulted in. This is important because we have a hard limit on the size of the virtual memory of the program.

Compiling and Running Code
To compile mmap.c:
$ gcc mmap.c -lm -o mmap
Which produces a mmap file, which you can run:
$ ./mmap
page_size is 4096
Validating square root table contents...
oops got SIGSEGV at 0x7f6bf7fd7f18

When the process accesses the square root table, the mapping does not exist and the kernel passes control to the signal handler code in handle_sigsegv(). Modify the code in handle_sigsegv() to map in a page at the faulting address, unmap a previous page to stay within the physical memory limit, and initialize the new page with the correct square root values. Use the function calculate_sqrts() to compute the values.

Submission
You need to submit the modified mmap.c file. Add relevant comments along with the code to help us understand it better. 

Testing
The program includes test logic that verifies if the contents of the square root table are correct. When you have completed your task successfully, the process will print “All tests passed!”.