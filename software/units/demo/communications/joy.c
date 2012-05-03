#include <stdio.h>
#include <stdlib.h>
#include <linux/joystick.h>

int main() {
	int len;
	char msg[50];
	char* device = "/dev/input/js0";
	FILE *f = fopen(device, "rb+");
	
	while(1) {
		read(f, &msg, 50);
		sleep(1);
			printf("%s\n",msg);

	}
	
	return 0;

}