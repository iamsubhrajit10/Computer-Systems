/*
* Team Members: Subhrajit, Pratyush
*/

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <math.h>
#include <time.h>

/* page size for the current working system */
static size_t page_size;

/* N: size of the page access tracking structure */
static int64_t N;

/* align_down - rounds a value down to an alignment
 * @x: the value
 * @a: the alignment (must be power of 2)
 *
 * Returns an aligned value.
*/
#define align_down(x, a) ((x) & ~((typeof(x))(a) - 1))

#define AS_LIMIT (1 << 25) // Maximum limit on virtual memory bytes
#define MAX_SQRTS (1 << 27) // Maximum limit on sqrt table entries
static double *sqrts;

/* enumartor created for page_replacement_policy*/
enum Policy{
  FIFO, //0
  LRU,  //1
  CLOCK //2
};
/*page_replacement_policy value: 0 for fifo, 1 for lru, 2 for clock*/
enum Policy page_replacement_policy;

const char* policyNames[] = {
    "FIFO",
    "LRU",
    "CLOCK"
};
/* Define a page frame struct*/
typedef struct {
  int64_t last_accessed_time;
  int64_t clock_reference_bit;
  uintptr_t page_addr;
  _Bool page_valid;
} page_frame_t;

/* defined at runtime with value provided by cmdline arg */
page_frame_t *page_access_history;

/* counts the number of page faults occured so far*/
static int64_t page_fault_count;

/* counts the page replacement needed so far*/
static int64_t page_replace_count;
static int64_t page_access_count;
/* contains the maximum elements possible for a page */
int64_t max_ele_per_page;

struct timespec ts;

static int64_t timestamp() {
    clock_gettime(CLOCK_MONOTONIC, &ts);  
    return (int64_t)(ts.tv_sec) * (int64_t)1000000000 + (int64_t)(ts.tv_nsec);
}

/*initializing the page tracking structure*/
static void 
initialize_page_access_history(){
  page_fault_count=0;
  page_replace_count=0;
  page_access_count=0;
  for(int64_t i=0;i<N;i++){
    page_access_history[i].last_accessed_time=timestamp();
    page_access_history[i].clock_reference_bit=-1;
    page_access_history[i].page_addr=0;
    page_access_history[i].page_valid=0;
  }
}

/*to print64_t the page tracking history*/
static void
printPageTracking(){
    printf("Page No, Page address, Start position, Ending Position\n");
    for(int64_t i=0;i<N;i++){
    uintptr_t start_addr = (uintptr_t)sqrts;
    uintptr_t page_addr = page_access_history[i].page_addr;
    unsigned long start_entry_pos = (page_addr-start_addr)/sizeof(double);
    unsigned long entries_per_page = page_size/sizeof(double);
    printf("Page %ld: ",i);
    printf("0x%lx, %ld, %ld\n", 
           page_addr,start_entry_pos, start_entry_pos + entries_per_page - 1);
  }
  printf("\n");
}
/*FIFO page replacement algorithm
* Returns the oldest page index in page_access_history in a circular manner.
*/ 
static 
int64_t fifo_replace_page() {
  return (page_replace_count++) % N;
}

/*LRU page replacement algorithm
* Returns the index of the LRU page in page_access_history.
*/ 
int64_t lru_replace_page() {
    if(page_replace_count<N)
      return page_replace_count++;
    page_replace_count++;
    // Initialize the LRU page index.
    int64_t lru_page_index = 0;
  // Iterate over the cache and find the page with the oldest last accessed time.
  for (int64_t i = 1; i < N; i++) {
    if (page_access_history[i].last_accessed_time < page_access_history[lru_page_index].last_accessed_time) {
      lru_page_index = i;
    }
  }
  return lru_page_index;
}

/* victim page number stored for clock replacement*/
static int64_t victim_page_number;

/* Clock page replacement algorithm */
int64_t clock_replace_page() {
  if(page_replace_count<N)
    return page_replace_count++;
  page_replace_count++;
  // Find the next victim page number using the Clock algorithm.
  while (1) {
    if (page_access_history[victim_page_number].clock_reference_bit == 0) {
      // This page is a candidate to be replaced.
      break;
    }

    // Set the reference bit to 0.
    page_access_history[victim_page_number].clock_reference_bit = 0;

    // Increment the victim page number.
    victim_page_number = (victim_page_number + 1) % N;
  }

  return victim_page_number;
}

/* checks whether page_addr has entry in the page_access_history 
* returns the index if it is there, otherwise returns -1
*/
static int
getPageIndex(uintptr_t page_addr, double ele, int64_t pos){
  for (int64_t i=0;i<N;i++){
    if(page_access_history[i].page_addr 
      && (page_access_history[i].page_addr == page_addr)
      && page_access_history[i].page_valid){
      return i;
    }
  }
  return -1;
}

void update_page_ref_history(int64_t pos) {
    /* address of the currently accessed page*/
    uintptr_t page_addr = (uintptr_t)&sqrts[pos];
    /* base address the currently accessed page*/
    page_addr = align_down(page_addr,page_size);
    double *ele=(double *)page_addr;
    int64_t i;
    page_access_count++;

    /* starting pos of the current page  */
    int64_t start_pos=(pos-pos%max_ele_per_page);

    if((i=getPageIndex(page_addr,ele[0],start_pos))>-1){
        /* updating accessed time and reference bit */
        page_access_history[i].last_accessed_time=timestamp();
        page_access_history[i].clock_reference_bit=1; 
    }
}

/* Use this helper function as an oracle for square root values. */
static void
calculate_sqrts(double *sqrt_pos, int64_t start, int64_t nr)
{
  int64_t i;
  for (i = 0; i < nr; i++)
    sqrt_pos[i] = sqrt((double)(start + i));
}

/* page fault handler */
static void handle_sigsegv(int sig, siginfo_t *si, void *ctx) {
    
    page_fault_count++;
    uintptr_t fault_addr = (uintptr_t)si->si_addr;
    uintptr_t sqrt_addr= (uintptr_t )(&(sqrts[0]));

    /* base address of the faulty page */
    uintptr_t base_addr = align_down(fault_addr, page_size);

    /* starting index of the values to be computed for sqrt next */
    int64_t start_index = (base_addr - (uintptr_t)sqrts) / sizeof(double);

    /* replacement victim page number in page_access_history */
    int64_t replacement_page_number;
    void *replacement_page=NULL;

      switch (page_replacement_policy)
      {
      case FIFO:
        replacement_page_number=fifo_replace_page();
        break;

      case LRU:
        replacement_page_number=lru_replace_page();
        break;

      case CLOCK:
        replacement_page_number=clock_replace_page();
        break;

      default:
        replacement_page_number=fifo_replace_page();
        break;
      }
    /* replacement page_address of the replacement page in the page_access_history */
    replacement_page=(void *)page_access_history[replacement_page_number].page_addr;
  
    /* to allocate new page, trying to unmap the last page */
    if (replacement_page&& munmap(replacement_page, page_size)<0){
      perror("munmap failed");
      exit(EXIT_FAILURE);
    }

    // trying to map the new page from the base address of the faulty page
    uintptr_t *new_page;
    if ((new_page = mmap((void *)base_addr, page_size,
      PROT_READ | PROT_WRITE,
      MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)) < 0) {
        printf("mmap fails\n");
        exit(EXIT_FAILURE);
    }

    // now, computing the sqrts for this new page
    calculate_sqrts((double *)new_page, start_index, max_ele_per_page);

    /*updating the victim page entry in the page_access_history*/
    page_access_history[replacement_page_number].page_addr=(uintptr_t)new_page;
    page_access_history[replacement_page_number].clock_reference_bit=0;

    page_access_history[replacement_page_number].last_accessed_time=timestamp();
    page_access_history[replacement_page_number].page_valid=1;
}

static void
free_sqrt_region(void)
{
  if (munmap(sqrts, MAX_SQRTS * sizeof(double) + AS_LIMIT) == -1) {
    fprintf(stderr, "Couldn't munmap() region for sqrt table; %s\n", 
            strerror(errno));
    exit(EXIT_FAILURE);
  }

}
static void
setup_sqrt_region(void)
{
  max_ele_per_page=page_size/sizeof(double);
  struct rlimit lim = {AS_LIMIT, AS_LIMIT};
  struct sigaction act;
  page_access_count=0;
  page_replace_count=0;
  // Only mapping to find a safe location for the table.
  sqrts = mmap(NULL, MAX_SQRTS * sizeof(double) + AS_LIMIT, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (sqrts == MAP_FAILED) {
    fprintf(stderr, "Couldn't mmap() region for sqrt table; %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  // Now release the virtual memory to remain under the rlimit.
  if (munmap(sqrts, MAX_SQRTS * sizeof(double) + AS_LIMIT) == -1) {
    fprintf(stderr, "Couldn't munmap() region for sqrt table; %s\n",
            strerror(errno));
    exit(EXIT_FAILURE);
  }

  // Set a soft rlimit on virtual address-space bytes.
  if (setrlimit(RLIMIT_AS, &lim) == -1) {
    fprintf(stderr, "Couldn't set rlimit on RLIMIT_AS; %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  // Register a signal handler to capture SIGSEGV.
  act.sa_sigaction = handle_sigsegv;
  act.sa_flags = SA_SIGINFO;
  sigemptyset(&act.sa_mask);
  if (sigaction(SIGSEGV, &act, NULL) == -1) {
    fprintf(stderr, "Couldn't set up SIGSEGV handler;, %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }
}

static void free_page_access_history()
{
  for(int64_t i=0;i<N;i++){
    if (munmap((void*)page_access_history[i].page_addr,page_size) == -1) {
		  fprintf(stderr, "Couldn't munmap() region for sqrt table; %s\n",
			strerror(errno));
		  exit(EXIT_FAILURE);
	  }
  }
  free(page_access_history);
}


/* workload 1*/
static void
test_sqrt_region(void)
{
  printf("Validating square root table contents...\n");
  srand(0xDEADBEEF);
  int64_t i, pos = rand() % (MAX_SQRTS - 1);
  double correct_sqrt;

  /* policy */
  // page_replacement_policy=FIFO;
  printf("Workload 1, policy used: %s\n",policyNames[page_replacement_policy]);
  page_access_history=(page_frame_t *)malloc(N*sizeof(page_frame_t));
  initialize_page_access_history();
  for (i = 0; i < 500000; i++) {
    if (i % 2 == 0)
      pos = rand() % (MAX_SQRTS - 1);
    else
      pos += 1;

    calculate_sqrts(&correct_sqrt, pos, 1);

    if (sqrts[pos] != correct_sqrt) {
      fprintf(stderr, "Square root is incorrect. Expected %f, got %f.\n",
              correct_sqrt, sqrts[pos]);
      exit(EXIT_FAILURE);
    }
    update_page_ref_history(pos);
  }

  printf("All tests passed!\n");
  // printf("Page faults in 1st workload: %ld\n", page_fault_count);
  printf("Pages in track: \n");
  printPageTracking();
  free_page_access_history();
}

/* workload 2*/
static void
test_sqrt_region_1(void)
{
  printf("Validating square root table contents...\n");
  srand(0xDEADBEEF);
  int64_t i, pos = rand() % (MAX_SQRTS - 1);
  double correct_sqrt;

  /* policy used */
  // page_replacement_policy=LRU;
  printf("Workload 2, policy used: %s\n",policyNames[page_replacement_policy]);
  page_access_history=(page_frame_t *)malloc(N*sizeof(page_frame_t));
  initialize_page_access_history();
  for (i = 0; i < 50000000; i++)
  {
    pos = i % (MAX_SQRTS - 1);
    calculate_sqrts(&correct_sqrt, pos, 1);
    if (sqrts[pos] != correct_sqrt){ 
      fprintf(stderr, "Square root is incorrect. Expected %f, got %f.\n",
              correct_sqrt, sqrts[pos]);
      exit(EXIT_FAILURE);
    }
    update_page_ref_history(pos);
  }
  printf("All tests passed!\n");
  // printf("Page faults in 2nd workload: %ld\n", page_fault_count);
  printf("Pages in track: \n");
  printPageTracking();
  free_page_access_history();
}

/* workload 3*/
static void
test_sqrt_region_1000(void)
{
  printf("Validating square root table contents...\n");
  srand(0xDEADBEEF);
  int64_t i, pos = rand() % (MAX_SQRTS - 1);
  double correct_sqrt;

  /* replacement policy*/
  // page_replacement_policy=CLOCK;
  printf("Workload 3, policy used: %s\n",policyNames[page_replacement_policy]);
  page_access_history=(page_frame_t *)malloc(N*sizeof(page_frame_t));

  initialize_page_access_history();
  for (i = 0; i < 50000000; i = i + 1000)
  {
    pos = i % (MAX_SQRTS - 1);
    calculate_sqrts(&correct_sqrt, pos, 1);
    if (sqrts[pos] != correct_sqrt){ 
      fprintf(stderr, "Square root is incorrect. Expected %f, got %f.\n",
              correct_sqrt, sqrts[pos]);
      exit(EXIT_FAILURE);
    }
    update_page_ref_history(pos);
  }

  printf("All tests passed!\n");
  // printf("Page faults in 3rd workload: %ld\n", page_fault_count);
  printf("Pages in track: \n");
  printPageTracking();
  free_page_access_history();
}

int
main(int argc, char *argv[])
{ 
  if(argc<2){
    printf("Error! Run the code with './mmap <N>'\n'");
    exit(-1);
  }
  N=atoi(argv[1]);
  if(N<1){
    printf("Error! './mmap <N>' N value should be >0 \n");
    exit(-1);
  }
  page_size = sysconf(_SC_PAGESIZE);
  printf("page_size is %ld\n", page_size);
  setup_sqrt_region();
  srand(0xDEADBEEF);
  
  int64_t faults_fifo_orig , faults_fifo_1, faults_fifo_2;
  int64_t faults_lru_orig , faults_lru_1, faults_lru_2;
  int64_t faults_clock_orig , faults_clock_1, faults_clock_2;
  
  page_replacement_policy=FIFO;
  // printf("Testing workload 1...\n");
  test_sqrt_region();
  faults_fifo_orig = page_fault_count;

  // printf("Testing workload 2...\n");
  test_sqrt_region_1();
  faults_fifo_1 = page_fault_count;
  
  // printf("Testing workload 3...\n");
  test_sqrt_region_1000();
  faults_fifo_2 = page_fault_count;
    
  page_replacement_policy=LRU;
  // printf("Testing workload 1...\n");
  test_sqrt_region();
  faults_lru_orig = page_fault_count;

  // printf("Testing workload 2...\n");
  test_sqrt_region_1();
  faults_lru_1 = page_fault_count;
  
  // printf("Testing workload 3...\n");
  test_sqrt_region_1000();
  faults_lru_2 = page_fault_count;
  
  page_replacement_policy=CLOCK;
  // printf("Testing workload 1...\n");
  test_sqrt_region();
  faults_clock_orig = page_fault_count;

  // printf("Testing workload 2...\n");
  test_sqrt_region_1();
  faults_clock_1 = page_fault_count;
  
  // printf("Testing workload 3...\n");
  test_sqrt_region_1000();
  faults_clock_2 = page_fault_count;
  
  printf("\nResults:\n");
  printf("N = %ld\t\t\t|FIFO\t|LRU\t|CLOCK\t|\n", N);
  printf("Workload Original\t|%ld\t|%ld\t|%ld\t|\n",faults_fifo_orig, faults_lru_orig, faults_clock_orig);
  printf("Workload 1\t\t|%ld\t|%ld\t|%ld\t|\n",faults_fifo_1, faults_lru_1, faults_clock_1);
  printf("Workload 2\t\t|%ld\t|%ld\t|%ld\t|\n",faults_fifo_2, faults_lru_2, faults_clock_2);
  
  return 0;
}
