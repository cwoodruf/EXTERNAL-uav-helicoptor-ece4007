#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>


int main(int argc, char **argv) {
	int i = 0;
	int fd = -1;
	int addr = 0;
	int reg = 0;
	int times = 0;
	char buf[10];
	struct timeval start,end;
	unsigned int seconds = 0;
	unsigned long long microseconds = 0;
	unsigned int s;
	unsigned int us;

	if(argc < 4) {
		printf("usage: i2c_bench [address] [register] [times]\n");
		return -1;
	}
	addr = atoi(argv[1]);
	reg = atoi(argv[2]);
	times = atoi(argv[3]);

	if((fd = open("/dev/i2c-3",O_RDWR)) < 1) {
		printf("ERROR Opening File\n");
		return -1;
	}

	if(ioctl(fd,I2C_SLAVE,addr)) {
		printf("ERROR Opening Device\n");
		return -1;
	}

	for(i=0;i<times;++i) {
		gettimeofday(&start,NULL);
		buf[0] = reg;
		if(write(fd,buf,1) != 1) {
			printf("ERROR Could not write to device\n");
			return -1;
		}
		if(read(fd,buf,1) < 1) {
			printf("ERROR Could not read from device\n");
			return -1;
		}
		gettimeofday(&end,NULL);

		s = end.tv_sec - start.tv_sec;
		us = (end.tv_usec - start.tv_usec);
		seconds += s;
		microseconds += us;

		printf("READ in %ds %dus\n",s,us);
	}

	while(microseconds > 1000000) {
		seconds += 1;
		microseconds -= 1000000;
	}

	printf("RAN %d READS in %ds %dms\n",times,seconds,(unsigned int)microseconds/1000);

	return 0;
}
