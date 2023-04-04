#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>

#define DATASIZE 1024  // size of shared memory segment
#define SOCK_PATH "pippo"

int main()
{
	int shmid;
	int client_sock, server_sock,rc, len;
	key_t key;
	char *shm, *s;
	struct sockaddr_un server_sockaddr;
	struct sockaddr_un client_sockaddr;

	printf("create server socket\n");
	server_sock = socket(AF_UNIX, SOCK_STREAM, 0);
	if (server_sock == -1){
		printf("SOCKET ERROR:\n");
		exit(1);
		}
	memset(&server_sockaddr, 0, sizeof(struct sockaddr_un));
	server_sockaddr.sun_family = AF_UNIX;
	strcpy(server_sockaddr.sun_path, SOCK_PATH);
	len = sizeof(server_sockaddr);

	printf("bind server socket\n");
	rc = bind(server_sock, (struct sockaddr *) &server_sockaddr, len);
	if (rc == -1){
		printf("BIND ERROR: \n");
		close(server_sock);
		exit(1);
		}


	printf("create key 4 shmem sysV\n");
	// generate a key for the shared memory segment
	key = ftok(".", 's');
	if (key == -1) {
		perror("ftok");
		exit(1);
		}

	printf("get shmem\n");
	// create the shared memory segment
	shmid = shmget(key, DATASIZE, IPC_CREAT | 0666);
	if (shmid == -1) {
		perror("shmget");
		exit(1);
		}

	printf("shmid = %d\n", shmid);

	// attach to the shared memory segment
	shm = shmat(shmid, NULL, 0);
	if (shm == (char *) -1) {
		perror("shmat");
		exit(1);
		}

	// write some data to the shared memory segment
	strcpy(shm, "Hello, world!");
	// print out the data from the shared memory segment
	for (s = shm; *s != '\0'; s++) putchar(*s);
	putchar('\n');

	rc = listen(server_sock, 5);
	if (rc == -1){ 
 		printf("LISTEN ERROR: \n");
		close(server_sock);
		exit(1);
		}
	printf("socket listening...\n");

	client_sock = accept(server_sock, NULL, NULL);
	if (client_sock == -1){
 		printf("ACCEPT ERROR: \n");
		close(server_sock);
		close(client_sock);
		exit(1);
		}
	printf("send [%d] => unixsk\n", shmid);
	rc = send(client_sock, &shmid, sizeof(shmid), 0);
	printf("Message sent\nwaiting...\n");

	while (*shm=='H') sleep(1);

	// detach from the shared memory segment
	if (shmdt(shm) == -1) {
		perror("shmdt");
		exit(1);
		}

	// delete the shared memory segment
	if (shmctl(shmid, IPC_RMID, NULL) == -1) {
		perror("shmctl");
		exit(1);
		}
	unlink(SOCK_PATH);

	return 0;
}
