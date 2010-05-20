#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>


void move(int x, int y)
{
	printf("\x1b[%d;%dH", y, x);
}

void show(char *c, int x, int y)
{
	printf("\x1b[%d;%dH%s", y, x, c);
}

int data[] = {20, 40, 55, 60, 70, 80, 999};
int pos []  ={ 1,  2,  1,  1,  1,  2,   0};
int max = 100;

volatile hit1 = 0;
volatile hit2 = 0;
int single = 0;

void sigio(int sig)
{
	if (sig == SIGIO)
		hit1++;
	else
		hit2++;
	if (single)
		hit2++;
}

int open_device(char *devname, long sig)
{
	struct sigaction sa = { .sa_handler = sigio };
	int fd;

	printf("device file = %s\n", devname);

	fd = open(devname, O_RDONLY);
	if (fd < 0) {
		perror("open");
		return fd;
	}
	
	sigaction(sig, &sa, NULL);

	/* send signal SIGIO on sensor input */
	if (fcntl(fd, F_SETOWN, getpid()) < 0) {
		perror("fcntl");
		exit(1);
	}

	if (fcntl(fd, F_SETSIG, sig) < 0) {
		perror("fcntl");
		exit(1);
	}

	if (fcntl(fd, F_SETFL, O_ASYNC) < 0) {
		perror("fcntl");
		exit(1);
	}
}

int main(int argc, char *argv[])
{
	int i, point = 0;
	int p = 0, q;
	char *devname[2] = {"/dev/yurex0", "/dev/yurex1"};

	i = open_device(devname[0], SIGIO);
	if (i < 0)
		exit(1);
	i = open_device(devname[1], SIGUSR1);
	if (i < 0) {
		printf("single yurex mode.\n");
		single = 1;
		sleep(1);
	}

	system("clear");

	show("[ ]", 19, 20);
	show("[ ]", 49, 20);

	for (i = 0; i < max; i++) {
		for (q = p; data[q] < max; q++) {
			if (data[q] < 0) {
				if (p == q)
					p++;
				continue;
			}
			int y = 20 - (data[q] - i);
			int x = pos[q] * 30 - 10;
			if (y > 26) {
				p = q;
				continue;
			}
			if (y > 1)
				show(" ", x, y-1);
			if (y < 1) {
				break;
			}
			if (y < 25)
				show("o", x, y);
		}
		move(0, 25);
		printf("%d / %d [%d %d]", i, max, p, data[p]);
		fflush(stdout);

		usleep(50000);

		show(hit1 ? "X" : " ", 20, 24);
		show(hit2 ? "X" : " ", 50, 24);
		if (!hit1 && !hit2) {
			move(10, 25);
			printf("\tSCORE : %d", point);
			continue;
		}

		while (hit1 || hit2) {
			int min = 5, m = -1;
			for (q = p; data[q] < max && data[q] < i + 5; q++) {
				if ((!hit1 && pos[q] == 1) ||
				    (!hit2 && pos[q] == 2))
					continue;
				int d = abs(data[q] - i);
				if (d < min) {
					min = d;
					m = q;
				}
			}
			if (m != -1) {
				int y = 20 - (data[m] - i);
				int x = pos[m] * 30 - 10;
				show("@", x, y);
				data[m] = -1;
				point += 5 - min;
				if (pos[m] == 1)
					hit1--;
				else
					hit2--;
			}
			else
				break;
			
		}
		move(10, 25);
		printf("\tSCORE : \a%d", point);

		hit1 = hit2 = 0;
	}

	printf("\n");
	return 0;
}
