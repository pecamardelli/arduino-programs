#include "../ecu.h"

double microsec(){
	/* get monotonic clock time */
    struct timespec monotime;
	
    clock_gettime(CLOCK_MONOTONIC_RAW, &monotime);
	//printf("\t%lf\t%d\t%lu\n",(double)monotime.tv_sec * 1000000 + (double)monotime.tv_nsec / 1000, monotime.tv_sec, (unsigned long)monotime.tv_nsec);
	return (double)monotime.tv_sec * 1000000 + (double)monotime.tv_nsec / 1000;
}

void* create_shmem(size_t size){
  // Our memory buffer will be readable and writable:
  int protection = PROT_READ | PROT_WRITE;

  // The buffer will be shared (meaning other processes can access it), but
  // anonymous (meaning third-party processes cannot obtain an address for it),
  // so only this process and its children will be able to use it:
  int visibility = MAP_ANONYMOUS | MAP_SHARED;

  // The remaining parameters to `mmap()` are not important for this use case,
  // but the manpage for `mmap` explains their purpose.
  return mmap(NULL, size, protection, visibility, -1, 0);
}