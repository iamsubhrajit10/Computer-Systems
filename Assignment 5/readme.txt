We will explore page replacement policies in this assignment. This assignment will be in continuation to previous assignment on memory-mapped files and has to be done with the same group members as the previous assignment.

We will vary page access workload and select the best page replacement policy for each of them. The submission for this assignment will include the mmap.c file with the various page replacement strategies implemented, and a detailed report explaining each workload pattern and why a particular policy would work best in that case.

In the current implementation, we have this workload in the test_sqrt_region function:

for (i = 0; i < 500000; i++)
{
    if (i % 2 == 0)
    {
        pos = rand() % (MAX_SQRTS - 1);
    }
    else
    {
        pos += 1;
    }
    calculate_sqrts(&correct_sqrt, pos, 1);
    if (sqrts[pos] != correct_sqrt) 
    .... rest of the code...
}
Now, create new workloads, with the following code replacing the above one in a new function, say test_sqrt_region_1


for (i = 0; i < 50000000; i++)
{
    pos = i % (MAX_SQRTS - 1);
    calculate_sqrts(&correct_sqrt, pos, 1);
    if (sqrts[pos] != correct_sqrt) 
    .... rest of the code...
}
Create a third workload, with the following code replacing the above one in a new function, say test_sqrt_region_1000

for (i = 0; i < 50000000; i = i + 1000)
{
  pos = i % (MAX_SQRTS - 1);
  calculate_sqrts(&correct_sqrt, pos, 1);
  if (sqrts[pos] != correct_sqrt)
  .... rest of the code...
}
Given these three workloads, your task is to implement and find the best page replacement policy for each one of them, i.e.:

You should consider the following page replacement algorithms for comparison purposes - FIFO, LRU, and Clock algorithm.
Identify the type of workload and reason out using an example of which page replacement policy would work best here.
What is the tradeoff/overhead of the proposed policy with respect to other possible page replacement policies(compared with two other policies)?
You may consider a memory size of N pages for each of the algorithms, i.e., you will maintain a cache of N pages at any given time that will store the square root values of a certain range of numbers. You should pass a parameter to the program that will set the slot size accordingly. For example, if you do the following: ./mmap 16, the slot size (N) will be set to 16. 
Instead of unmapping a page every time you handle a segfault, you will use the respective page replacement algorithm to decide which page to unmap (victim page) out of the N pages.
As part of the output, you should print the information of all N pages, i.e., which position ranges are stored in those pages.
For evaluation purposes, we will run your program across diferent workloads (such as the one mentioned above) and different slot sizes to verify if your clock algorithm works fine.
Upload the compressed folder with the .c file and the .pdf write-up for evaluation.