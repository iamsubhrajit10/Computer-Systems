Instructions
This assignment will give you practice in the style of programming you will need to be able to do proficiently, especially for the later assignments in the class. The material covered should all be review for you. Some of the skills tested are:



• Explicit memory management, as required in C.
• Creating and manipulating pointer-based data structures.
• Working with strings.
• Enhancing the performance of key operations by storing redundant information in data structures.
• Implementing robust code that operates correctly with invalid arguments, including NULL pointers.




The lab involves implementing a queue, supporting both last-in, first-out (LIFO) and first-in-first-out (FIFO) queueing disciplines. The underlying data structure is a singly-linked list, enhanced to make some of the operations more efficient.

Download the accompanying tar file.
Extract them with "tar --strip=1 -xf cprogramming-handout.tar"
You will only need to modify queue.h and queue.c. Don’t modify any other files! If you do, we will ignore those modifications and your code probably won’t compile.




Your task is to modify the code in queue.h and queue.c to fully implement the following functions.

queue_new: Create a new, empty queue.
queue_free: Free all storage used by a queue.

queue_insert_head: Attempt to insert a new element at the head of the queue (LIFO discipline). queue_insert_tail: Attempt to insert a new element at the tail of the queue (FIFO discipline). queue_remove_head: Attempt to remove the element at the head of the queue.

queue_size: Compute the number of elements in the queue.

queue_reverse: Reorder the list so that the queue elements are reversed in order. This function should not allocate or free any list elements (either directly or via calls to other functions that allocate or free list elements.) Instead, it should rearrange the existing elements.


Two of the functions, queue_insert_tail and queue_size, will require some effort on your part to meet the required performance standards. Naive implementations would require O(n) steps for a queue with n elements. We require that your implementations operate in time O(1), i.e., that the operation will require only a fixed number of steps, regardless of the queue size. You can do this by including other fields in the queue_t data structure and managing these values properly as list elements are inserted, removed and reversed. Please work on finding a solution better than the O(n2) solution for all of the functions.

Your program will be tested on queues with over 1,000,000 elements. You will find that you cannot operate on such long lists using recursive functions, since that would require too much stack space. Instead, you need to use a loop to traverse the elements in a list.

