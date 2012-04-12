#ifndef USER_H
#define USER_H

#include <sys/time.h>
#include <sys/socket.h>

class User {

	private:
		fd_set fdset;
		struct timeval timeout;
		int rc;
		char val;

	public:

		User();
		User(int s, int us);
		char get_input();

};

User::User() {
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
}

User::User(int s, int us) {
	timeout.tv_sec = s;
	timeout.tv_usec = us;
}

char User::get_input() {
	FD_ZERO(&fdset);
	FD_SET(0,&fdset);

	rc = select(1,&fdset,NULL,NULL,&timeout);
	if(rc > 0) {
		if(FD_ISSET(0,&fdset)) {
			return getchar();
		} 
	}
	return NULL;
}

#endif
