#ifndef USER_H
#define USER_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <unistd.h>

#ifdef __VMS
#include <types.h>
#include <socket.h>
#include <in.h>
#include <inet.h>

#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include <openssl/crypto.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include <pthread.h>
#include <sys/time.h>


#define RSA_SERVER_CERT		"communications/server.crt"
#define RSA_SERVER_KEY		"communications/server.key"
#define RSA_SERVER_CA_CERT	"communications/ca.crt"
#define RSA_SERVER_CA_PATH	"sys$common:[syshlp.example.ssl]"
#define ON	1
#define OFF 0


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


typedef struct _TController {
	int x1;
	int y1;
	int x2;
	int y2;
	int lt;
	int rt;
	//A,B,X,Y,LB,RB,Back,Start,Xbox,L3,R3
	int buttons[11];
}TController;


namespace SSL_SERVER {

	TController tXbox;
	pthread_mutex_t data_mutex;
	char outbuf[4096];

	void (*logger)(const char *) = NULL;
	void (*error)(void) = NULL;
	bool CONNECTION = false;

	void *ssl_srv(void *in);

	void server_init() {
		pthread_mutex_init(&data_mutex,NULL);
	}

	void server_start() {
		pthread_t srv_thread;
		pthread_create(&srv_thread,NULL,ssl_srv,NULL);
	}

	bool server_isConnected() {
		return CONNECTION;
	}

	void *ssl_srv(void *in) {
		int err;
		int verify_client = OFF;
		int listen_sock;
		int sock;
		struct sockaddr_in sa_serv;
		struct sockaddr_in sa_cli;
		size_t client_len;
		char *str;
		char buf[4096];

		SSL_CTX *ctx;
		SSL *ssl;
		SSL_METHOD const *meth;
		X509 *client_cert = NULL;

		short int s_port = 5555;

		SSL_library_init();
		SSL_load_error_strings();
		meth = SSLv23_method();
		ctx = SSL_CTX_new(meth);

		if(!ctx) {
			ERR_print_errors_fp(stderr);
			if(logger != NULL) {
				logger("COMM ERROR - [SSL] Could Not Create Context");
			}
			if(error != NULL) {
				error();
			}
			return NULL;
		}

		if(SSL_CTX_use_certificate_file(ctx,RSA_SERVER_CERT,SSL_FILETYPE_PEM) <= 0) {
			ERR_print_errors_fp(stderr);
			if(logger != NULL) {
				logger("COMM ERROR - [SSL] Could Not Use Certificate");
			}
			if(error != NULL) {
				error();
			}
			return NULL;
		}

		if(SSL_CTX_use_PrivateKey_file(ctx,RSA_SERVER_KEY, SSL_FILETYPE_PEM) <= 0) {
			ERR_print_errors_fp(stderr);
			if(logger != NULL) {
				logger("COMM ERROR - [SSL] Could Not Use Private Key");
			}
			if(error != NULL) {
				error();
			}
			return NULL;
		}

		if(!SSL_CTX_check_private_key(ctx)) {
			ERR_print_errors_fp(stderr);
			if(logger != NULL) {
				logger("COMM ERROR - [SSL] Could Not Check Private Key");
			}
			if(error != NULL) {
				error();
			}
			return NULL;
		}

		if(verify_client == ON) {
			if(!SSL_CTX_load_verify_locations(ctx,RSA_SERVER_CA_CERT,NULL)) {
				ERR_print_errors_fp(stderr);
				if(logger != NULL) {
					logger("COMM ERROR - [SSL] Could Not Verify Locations");
				}
				if(error != NULL) {
					error();
				}
				return NULL;
			}

			SSL_CTX_set_verify(ctx,SSL_VERIFY_PEER,NULL);
			SSL_CTX_set_verify_depth(ctx,1);
		}

		if((listen_sock = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP)) < 0) {
			if(logger != NULL) {
				logger("COMM ERROR - [TCP] Could Not Open Socket");
			}
			if(error != NULL) {
				error();
			}
			return NULL;
		}
		memset(&sa_serv,'\0',sizeof(sa_serv));
		sa_serv.sin_family = AF_INET;
		sa_serv.sin_addr.s_addr = INADDR_ANY;
		sa_serv.sin_port = htons(s_port);
		if(bind(listen_sock,(struct sockaddr*)&sa_serv,sizeof(sa_serv)) < 0) {
			if(logger != NULL) {
				logger("COMM ERROR - [TCP] Could Not Bind Port");
			}
			if(error != NULL) {
				error();
			}
			return NULL;
		}
		if(listen(listen_sock,5) < 0) {
			if(logger != NULL) {
				logger("COMM ERROR - [TCP] Could Not Listen to Port");
			}
			if(error != NULL) {
				error();
			}
			return NULL;
		}
		client_len = sizeof(sa_cli);
		if((sock = accept(listen_sock,(struct sockaddr *)&sa_cli,&client_len))) {
			if(logger != NULL) {
				logger("COMM ERROR - [TCP] Could Not Accept Client");
			}
			if(error != NULL) {
				error();
			}
			return NULL;
		}
		close(listen_sock);

		std::cout << "Connection from: " << 
			sa_cli.sin_addr.s_addr << ", " <<
			sa_cli.sin_port <<
		std::endl;

		if((ssl = SSL_new(ctx)) < 0) {
			if(logger != NULL) {
				logger("COMM ERROR - [SSL] Connection Creation Failed");
			}
			if(error != NULL) {
				error();
			}
			return NULL;
		}

		SSL_set_fd(ssl,sock);
		if(SSL_accept(ssl) < 0) {
			if(logger != NULL) {
				logger("COMM ERROR - [SSL] Failed to Accept Client");
			}
			if(error != NULL) {
				error();
			}
			return NULL;
		}

		std::cout << "SSL Connection using: " << SSL_get_cipher(ssl) << std::endl;

		if(verify_client == ON) {
			client_cert = SSL_get_peer_certificate(ssl);
			if(client_cert != NULL) {
				if((str = X509_NAME_oneline(X509_get_subject_name(client_cert),0,0)) == NULL ) {
					if(logger != NULL) {
						logger("COMM ERROR - [SSL] Failed to Get Subject Name");
					}
					if(error != NULL) {
						error();
					}
					return NULL;	
				}
				free(str);
				X509_free(client_cert);
			} else {
				if(logger != NULL) {
					logger("COMM ERROR - [SSL] Client Does Not Have a Certificate");
				}
				if(error != NULL) {
					error();
				}
				return NULL;
			}
		}

		int rx = 1;
		CONNECTION = true;
		while(rx != 0) {
			if((err = SSL_read(ssl,buf,sizeof(buf)-1)) < 0) {
				if(logger != NULL) {
					logger("COMM ERROR - [SSL] Read Failed");
				}
				if(error != NULL) {
					error();
				}
				CONNECTION = false;
				return NULL;
			}

			buf[err] = '\0';
			pthread_mutex_lock(&data_mutex);
				//Cleaner way of doin this?
				sscanf(buf,"cmd %d ax [%d,%d] [%d,%d] [%d] [%d]	buttons %d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
						&rx, &tXbox.x1, &tXbox.y1,&tXbox.x2,&tXbox.y2,
						&tXbox.lt,&tXbox.rt,&tXbox.buttons[0],
						&tXbox.buttons[1],&tXbox.buttons[2],
						&tXbox.buttons[3],&tXbox.buttons[4],
						&tXbox.buttons[5],&tXbox.buttons[6],
						&tXbox.buttons[7],&tXbox.buttons[8],
						&tXbox.buttons[9],&tXbox.buttons[10]
				);
			pthread_mutex_unlock(&data_mutex);

			if(SSL_write(ssl,outbuf,strlen(outbuf)) < 0 ) {
				if(logger != NULL) {
					logger("COMM ERROR - [SSL] Write Failed");
				}
				if(error != NULL) {
					error();
				}
				CONNECTION = false;
				return NULL;
			}
		}

		CONNECTION = false;
		if(SSL_shutdown(ssl) < 0) {
			if(logger != NULL) {
				logger("COMM ERROR - [SSL] Shutdown Error");
			}
			if(error != NULL) {
				error();
			}
			return NULL;
		}
		if(close(sock) < 0) {
			if(logger != NULL) {
				logger("COMM ERROR - [TCP] Socket Close Error");
			}
			if(error != NULL) {
				error();
			}
			return NULL;
		}

		SSL_free(ssl);
		SSL_CTX_free(ctx); 
		return NULL;
	}
}

#endif
