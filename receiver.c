#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/un.h>

#define DATASIZE 1024  // size of shared memory segment
#define SOCK_PATH "pippo"

int main()
{
	int shmid;
	int fd,n,rc;
	key_t key;
	char *shm, *s;
	struct sockaddr_un client_sockaddr;


	fd = socket(AF_UNIX, SOCK_STREAM, 0);
	memset(&client_sockaddr, 0, sizeof(struct sockaddr_un));
	client_sockaddr.sun_family = AF_UNIX;
	strcpy(client_sockaddr.sun_path, SOCK_PATH);

	rc= connect(fd, (struct sockaddr *) &client_sockaddr, sizeof(struct sockaddr_un));
	if(rc == -1){
		printf("CONNECT ERROR %d\n", rc);
		close(fd);
		exit(1);
		}
	rc = recv(fd, &shmid, sizeof(shmid), 0);

	printf("Received: %d\n", shmid);

	// attach to the shared memory segment
	shm = shmat(shmid, NULL, 0);
	if (shm == (char *) -1) {
		perror("shmat");
		exit(1);
		}

	// write some data to the shared memory segment
	memset(shm, 0, DATASIZE);

	// print out the data from the shared memory segment

	sleep(3);

	return 0;
}
