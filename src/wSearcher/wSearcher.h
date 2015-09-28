#ifdef WIN32
#include <stdlib.h>
#include <tchar.h>
#include <winsock2.h>
#ifndef SINGLESEARCH
#include <windows.h>
#include <winsvc.h>
#endif
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#endif

#include <stdio.h>
#include <math.h>
#include <time.h>

#define LISTEN_PORT 4875
#define CONNECTION_NUM		1024

#ifndef WIN32
#define SOCKET int
#define closesocket close
#endif


typedef struct _tcpinfo
{
    SOCKET fd;
    unsigned long ip;
    unsigned short port;
    char buf[1024];
    int buft;
}TcpInfo;

TcpInfo g_tcp[CONNECTION_NUM];

char *get_filename(int id);
char *get_filetitle(int id);
char *get_filetag(int id);
int init_search(char * dd, int bigmem);
int done_search();
void do_query_all(unsigned char * inp);
int get_total();
void start_listen();
int set_nonblocking (SOCKET f);		//设置为非阻塞
int handle_connection(int i);
