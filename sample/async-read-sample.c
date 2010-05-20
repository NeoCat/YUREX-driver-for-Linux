#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

int fd;

void sigio(int sig)
{
	int len;
	char buf[16];

	/* read current value and write it to stdout */
	lseek(fd, 0, 0);
	len = read(fd, buf, sizeof(buf));
	if (len < 0) {
		perror("read");
		exit(1);
	}
	write(1, buf, len);
}

int main(int argc, char *argv[])
{
	int len;
	char *devname = "/dev/yurex0";
	struct sigaction sa = { .sa_handler = sigio };

	if (argc > 2)
		devname = argv[1];

	printf("device file = %s\n", devname);

	fd = open(devname, O_RDONLY);
	if (fd < 0) {
		perror("open");
		exit(1);
	}
	
	sigaction(SIGIO, &sa, NULL);

	/* send signal SIGIO on sensor input */
	if (fcntl(fd, F_SETOWN, getpid()) < 0) {
		perror("fcntl");
		exit(1);
	}

	if (fcntl(fd, F_SETFL, O_ASYNC) < 0) {
		perror("fcntl");
		exit(1);
	}

	for (;;) {
		/* sleep forever */
		select(0, NULL, NULL, NULL, NULL); 
	}
	return 0;
}
