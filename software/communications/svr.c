
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

#define RSA_SERVER_CERT     "server.crt"
#define RSA_SERVER_KEY          "server.key"

#define RSA_SERVER_CA_CERT "ca.crt"
#define RSA_SERVER_CA_PATH   "sys$common:[syshlp.examples.ssl]"

#define ON   1
#define OFF  0

#define RETURN_NULL(x) if ((x)==NULL) exit(1)
#define RETURN_ERR(err,s) if ((err)==-1) { perror(s); exit(1); }
#define RETURN_SSL(err) if ((err)==-1) { ERR_print_errors_fp(stderr); exit(1); }
void* ssl_svr(void* in);

struct joystat
{
    int x1,y1,x2,y2,lt,rt;
    int buts[11];
    //[A,B,X,Y,LB,RB,back,start,xbox,l3,r3]
    //[0,1,2,3,4 ,5 ,6,  ,7    ,8   ,9 ,10]
};

pthread_mutex_t  data_mux;
struct joystat joy;



void main()
{
    pthread_t svr_thread;
    pthread_mutex_init(&data_mux,NULL);
    pthread_create(&svr_thread, NULL , ssl_svr, NULL);
    while(1) {};
}

void* ssl_svr(void* in)
{
    int     err;
    int     verify_client = OFF; /* To verify a client certificate, set ON */

    int     listen_sock;
    int     sock;
    struct sockaddr_in sa_serv;
    struct sockaddr_in sa_cli;
    size_t client_len;
    char    *str;
    char     buf[4096];
    char outbuf[4096];

    SSL_CTX         *ctx;
    SSL            *ssl;
    SSL_METHOD      *meth;

    X509            *client_cert = NULL;

    short int       s_port = 5555;
    /*----------------------------------------------------------------*/
    /* Load encryption & hashing algorithms for the SSL program */
    SSL_library_init();

    /* Load the error strings for SSL & CRYPTO APIs */
    SSL_load_error_strings();

    /* Create a SSL_METHOD structure (choose a SSL/TLS protocol version) */
    meth = SSLv23_method();

    /* Create a SSL_CTX structure */
    ctx = SSL_CTX_new(meth);

    if (!ctx) {

        ERR_print_errors_fp(stderr);

        exit(1);

    }

    /* Load the server certificate into the SSL_CTX structure */
    if (SSL_CTX_use_certificate_file(ctx, RSA_SERVER_CERT, SSL_FILETYPE_PEM) <= 0) {

        ERR_print_errors_fp(stderr);

        exit(1);

    }

    /* Load the private-key corresponding to the server certificate */
    if (SSL_CTX_use_PrivateKey_file(ctx, RSA_SERVER_KEY, SSL_FILETYPE_PEM) <= 0) {

        ERR_print_errors_fp(stderr);
        exit(1);
    }

    /* Check if the server certificate and private-key matches */
    if (!SSL_CTX_check_private_key(ctx)) {

        fprintf(stderr,"Private key does not match the certificate public key\n");
        exit(1);
    }

    if(verify_client == ON)

    {

        /* Load the RSA CA certificate into the SSL_CTX structure */
        if (!SSL_CTX_load_verify_locations(ctx, RSA_SERVER_CA_CERT, NULL)) {

            ERR_print_errors_fp(stderr);
            exit(1);
        }

        /* Set to require peer (client) certificate verification */
        SSL_CTX_set_verify(ctx,SSL_VERIFY_PEER,NULL);

        /* Set the verification depth to 1 */
        SSL_CTX_set_verify_depth(ctx,1);

    }
    /* ----------------------------------------------- */
    /* Set up a TCP socket */

    listen_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    RETURN_ERR(listen_sock, "socket");
    memset (&sa_serv, '\0', sizeof(sa_serv));
    sa_serv.sin_family      = AF_INET;
    sa_serv.sin_addr.s_addr = INADDR_ANY;
    sa_serv.sin_port        = htons (s_port);          /* Server Port number */
    err = bind(listen_sock, (struct sockaddr*)&sa_serv,sizeof(sa_serv));

    RETURN_ERR(err, "bind");

    /* Wait for an incoming TCP connection. */
    err = listen(listen_sock, 5);

    RETURN_ERR(err, "listen");
    client_len = sizeof(sa_cli);

    /* Socket for a TCP/IP connection is created */
    sock = accept(listen_sock, (struct sockaddr*)&sa_cli, &client_len);

    RETURN_ERR(sock, "accept");
    close (listen_sock);

    printf ("Connection from %lx, port %d\n", sa_cli.sin_addr.s_addr,
            sa_cli.sin_port);

    /* ----------------------------------------------- */
    /* TCP connection is ready. */
    /* A SSL structure is created */
    ssl = SSL_new(ctx);

    RETURN_NULL(ssl);

    /* Assign the socket into the SSL structure (SSL and socket without BIO) */
    SSL_set_fd(ssl, sock);

    /* Perform SSL Handshake on the SSL server */
    err = SSL_accept(ssl);
    printf("\n%d\n",err);
    ERR_print_errors_fp(stderr);
    RETURN_SSL(err);

    /* Informational output (optional) */
    printf("SSL connection using %s\n", SSL_get_cipher (ssl));

    if (verify_client == ON)
    {

        /* Get the client's certificate (optional) */
        client_cert = SSL_get_peer_certificate(ssl);
        if (client_cert != NULL)
        {

            printf ("Client certificate:\n");
            str = X509_NAME_oneline(X509_get_subject_name(client_cert), 0, 0);
            RETURN_NULL(str);
            printf ("\t subject: %s\n", str);
            free (str);
            str = X509_NAME_oneline(X509_get_issuer_name(client_cert), 0, 0);
            RETURN_NULL(str);
            printf ("\t issuer: %s\n", str);
            free (str);
            X509_free(client_cert);
        }

        else

            printf("The SSL client does not have certificate.\n");
    }

    /*------- DATA EXCHANGE - Receive message and send reply. -------*/
    /* Receive data from the SSL client */
    int rx=1;
    while(rx!=0)
    {
        err = SSL_read(ssl, buf, sizeof(buf) - 1);

        RETURN_SSL(err);

        buf[err] = '\0';
        pthread_mutex_lock(&data_mux);
        //sscanf(buf,"cmd %d ax",&rx );
        sscanf(buf,"cmd %d ax [%d,%d] [%d,%d] [%d] [%d] buttons %d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",&rx, &joy.x1, &joy.y1,&joy.x2,&joy.y2,&joy.lt,&joy.rt,&joy.buts[0],&joy.buts[1],&joy.buts[2],&joy.buts[3],&joy.buts[4],&joy.buts[5],&joy.buts[6],&joy.buts[7],&joy.buts[8],&joy.buts[9],&joy.buts[10]);
        pthread_mutex_unlock(&data_mux);
        printf("cmd %d ax [%d,%d] [%d,%d] [%d] [%d] buttons %d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",rx, joy.x1, joy.y1,joy.x2,joy.y2,joy.lt,joy.rt,joy.buts[0],joy.buts[1],joy.buts[2],joy.buts[3],joy.buts[4],joy.buts[5],joy.buts[6],joy.buts[7],joy.buts[8],joy.buts[9],joy.buts[10]);
        printf("scanned %d\n" ,rx);
        printf ("Received %d chars:'%s'\n", err, buf);

        /* Send data to the SSL client */
        sprintf(outbuf,"PUT RETURN DATA HERE LIKE GPS AND AXIS");
        err = SSL_write(ssl, outbuf,strlen(outbuf));

        RETURN_SSL(err);
    }

    /*--------------- SSL closure ---------------*/
    /* Shutdown this side (server) of the connection. */

    err = SSL_shutdown(ssl);

    RETURN_SSL(err);

    /* Terminate communication on a socket */
    err = close(sock);

    RETURN_ERR(err, "close");

    /* Free the SSL structure */
    SSL_free(ssl);

    /* Free the SSL_CTX structure */
    SSL_CTX_free(ctx);

}