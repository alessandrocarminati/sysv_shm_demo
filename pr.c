#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main() {
	int fd = shm_open("/myshm", O_RDWR, S_IRUSR | S_IWUSR);
	void *ptr = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	printf("%s\n", (char*)ptr);
	munmap(ptr, 4096);
	close(fd);
	shm_unlink("/myshm");
	return 0;
}
