// wQuery.c : 定义控制台应用程序的入口点。
//

#include "wQuery.h"
#include "wSearcher.h"

#define INDEX_RANGE	10*1024

char buf[INDEX_RANGE],buf2[1024],buf3[1024];
int buft=0,total,length=0;
#define res_total get_total()

#define TIME_OUT 5			//网络超时函数

time_t startt;

#ifndef WIN32
#define SOCKET int
#endif

int send_line(SOCKET s, char * buf)
{
	int i,j;
	startt = time(0);
	j = strlen(buf);
	while (1) {
		i = send(s,buf,j,0);
		if (i!=-1) break;
		if (time(0)-startt>TIME_OUT)
			return -1;
	}
	return 0;
}

char * read_line(SOCKET s)	//从socket中读入一行，相当于fgets
{
	int j;
	char tmp[1024];
	startt = time(0);
	while (1) {
		for(j=0;j<buft;j++) {
			if (buf3[j]==('\n'^3)||buf3[j]==('\r'^3)) {
				strncpy(buf2, buf3, j);
				buf2[j] = 0;
				strncpy(tmp, buf3+j+1, buft-(j+1));
				strncpy(buf3, tmp, buft-(j+1));
				buft-=(j+1);
				return buf2;
			}
		}
		j = recv(s, buf3+buft, 1024-buft, 0);
		if (time(0)-startt>TIME_OUT) 
			return 0;
		if (j!=-1) buft+=j;
	}
}

char ht[1024*10];

#ifdef WIN32
int strncasecmp(char * s1, char * s2, int len)	//判断两个字符串是否相同，忽略大小写
{
	int i=0;
	while(i<len) {
		if (toupper(s1[i])!=toupper(s2[i])) return 1;
		i++;
	}
	return 0;
}
#endif

char * highlight_content(char * content, int len, char * qn, char * preTag, char * postTag) //将一段文字内容中的关键字高亮
{
    char * buf,out[10*1024],out2[10*1024],tmp[10];
    int i,j,k,l,fsize=0,t=0,p=0,q=0;

	buf = content; fsize = len;
	ht[0] = 0;
	memset(out, 0, fsize);
    for(i=0;i<fsize;i++) {
        if(buf[i]==0x1b) {
            j=i;
            while(!(buf[j]>='a'&&buf[j]<='z'||buf[j]>='A'&&buf[j]<='Z')&&j<fsize) {
                buf[j] = 0;
                j++;
            }
            if(j<fsize)
                buf[j] = 0;
        }
    }
    i=0;
    while(i<strlen(qn)) {
        if(qn[i]>='a'&&qn[i]<='z'||qn[i]>='A'&&qn[i]<='Z') {
            j=i;
            while(qn[j]>='a'&&qn[j]<='z'||qn[j]>='A'&&qn[j]<='Z'||qn[j]>='0'&&qn[j]<='9') j++;
            for(k=0;k<fsize-(j-i)+1;k++)
                if(!strncasecmp(qn+i,buf+k,j-i)&&(k==0||!(buf[k-1]>='a'&&buf[k-1]<='z'||buf[k-1]>='A'&&buf[k-1]<='Z'))&&
                    (k+j-i==fsize||!(buf[k+j-i]>='a'&&buf[k+j-i]<='z'||buf[k+j-i]>='A'&&buf[k+j-i]<='Z')))
                    for(l=0;l<j-i;l++) if(!out[k+l]){out[k+l]=1;t++;}
            i=j-1;
        }
        if(qn[i]>='0'&&qn[i]<='9') {
            j=i;
            while(qn[j]>='0'&&qn[j]<='9') j++;
            for(k=0;k<fsize-(j-i)+1;k++)
                if(!strncmp(qn+i,buf+k,j-i)&&(k==0||!(buf[k-1]>='0'&&buf[k-1]<='9'))&&
                    (k+j-i==fsize||!(buf[k+j-i]>='0'&&buf[k+j-i]<='9')))
                    for(l=0;l<j-i;l++) if(!out[k+l]){out[k+l]=1;t++;}
            i=j-1;
        }
        if(qn[i]<0&&qn[i+1]<0) {
            j=i+2;
            for(k=0;k<fsize-(j-i)+1;k++)
                if(!strncmp(qn+i,buf+k,j-i))
                    for(l=0;l<j-i;l++) if(!out[k+l]){out[k+l]=1;t++;}
            i=j-1;
        }
        i++;
    }
    if(t>=20) {
        for(k=0;k<fsize-4;k++) {
            if(out[k]==0&&out[k+1]==1&&out[k+2]==0) {
                out[k+1]=0;
                t--;
            }
            if(out[k]==0&&out[k+1]==1&&out[k+2]==1&&out[k+3]==0) {
                out[k+1]=0;
                out[k+2]=0;
                t-=2;
            }
            if(t<10) break;
        }
    }
    if(t==0) {
		strcat(ht,"\n\n\n");
		return ht;
    }
    while(t<180&&t<fsize) {
        memset(out2, 0, fsize);
        t=0;
        for(k=0;k<fsize;k++) 
        if(!out2[k]) {
            if(out[k]||k>0&&out[k-1]||k<fsize-1&&out[k+1]) {
                if(out[k]==1)
                    out2[k]=1;
                else {
                    out2[k]=2;
                    if(!out[k]&&buf[k]<0) {
						if(k>0&&out[k-1]&&k<fsize-1&&!out2[k+1]) {out2[k+1]=2;t++;}
						if(k>0&&k<fsize-1&&out[k+1]&&!out2[k-1]) {out2[k-1]=2;t++;}
                    }
                }
                t++;
            }
        }
        memcpy(out,out2,fsize);
    }
    j=0; t=0; q=0;
    for(i=0;i<fsize;i++)
    if(out[i]) {
        if(i>0&&out[i-1]==0) {
            strcat(ht,"..."); j+=3;
        }
		if(out[i]==1&&q==0) {
			strcat(ht,preTag);
			q = 1;
		}
		else if(out[i]!=1&&q==1) {
			strcat(ht,postTag);
			q = 0;
		}
        if(buf[i]=='\n'||buf[i]=='\r') strcat(ht," ");
		else if(buf[i]) {
			sprintf(tmp,"%c",buf[i]);
			strcat(ht,tmp);
		}
        if(p) p=0;
        else if(buf[i]<0) p=1;
        j++;
        if(j>=69&&p==0) {
            t++;
            if(t>=3) break;
            strcat(ht,"\n");
            j=0;
        }
    }
	if(q==1) strcat(ht,postTag);
	return ht;
}

char * highlight(char * fn, char * qn, char * preTag, char * postTag) //将一个文件的内容针对查询关键字高亮
{
    FILE* fp;
	int fsize;

    fp = fopen(fn, "rb");
    if(!fp) {
        return "";
    }
    fsize = fread(buf, 1, INDEX_RANGE, fp);
    fclose(fp);
	return highlight_content(buf, fsize, qn, preTag, postTag);
}

int back_bk,ii;
SOCKET s;

#ifdef WIN32
int _set_nonblocking (SOCKET f) //设置非阻塞socket
{
	u_long i=1;
	if (ioctlsocket(f, FIONBIO, &i) != 0)
		return -1;
    return 0;
}

#else
int _set_nonblocking (SOCKET f)
{
    if (fcntl (f, F_SETFL, O_NONBLOCK) != 0)
        return -1;
    return 0;
}
#endif

int _wait_read (SOCKET f)
{
    struct timeval to;
    int numfds;
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET (f, &readfds);
    to.tv_usec = 0;
    to.tv_sec = 60;
	numfds = select(f+1, &readfds, 0, 0, &to);
	if (numfds == -1) return -1;
	if (FD_ISSET(f, &readfds)) return 0;
	return -1;
}

int init_query(int bk, char * dd, int bigmem) //初始化查询过程
{
	back_bk = bk;
	if (bk) {
		return init_search(dd,bigmem);
	}
	else {
		struct sockaddr_in sin;
#ifdef WIN32
		WSADATA wsaData;
		WSAStartup(MAKEWORD(2,2),&wsaData);
#endif
		if (bigmem==0) bigmem = LISTEN_PORT;
		s = socket(AF_INET, SOCK_STREAM, 0);
		sin.sin_family = AF_INET;
		if (!dd[0]) sin.sin_addr.s_addr = inet_addr("127.0.0.1");
		else sin.sin_addr.s_addr = inet_addr(dd);
		sin.sin_port = htons(bigmem);
		if(connect(s, (struct sockaddr *)&sin, sizeof(struct sockaddr_in))<0) return -1;
		_set_nonblocking(s);
		return 0;
	}
}

int done_query() //结束查询
{
	if (back_bk)
		return done_search();
	else {
#ifdef WIN32
		closesocket(s);
		WSACleanup();
#else
		close(s);
#endif
		return 0;
	}
}

int fetch_total() //得到查询结果的总数
{
	return total;
}

int mask_query(char * s) //协议的加密解密
{
	int i;
	for(i=0;i<strlen(s);i++)
		s[i] ^= 3;
	return 0;
}

int wfetch(char * dataid, char * title, char * tag) //得到查询结果
{
	char * p;
	if (!length) return -1;
	length--;
	if (back_bk) {
		strcpy(dataid, get_filename(ii));
		strcpy(title, get_filetitle(ii));
		strcpy(tag, get_filetag(ii));
		ii++;
	}
	else {
		p = read_line(s);
		mask_query(p);
		strcpy(dataid, p);
		p = read_line(s);
		mask_query(p);
		strcpy(title, p);
		p = read_line(s);
		mask_query(p);
		strcpy(tag, p);
	}
	return 0;
}

int do_query(int i, int j, char * q) //执行查询
{
	char cmd[1024];
	if (back_bk) {
		do_query_all((unsigned char*)q);
		if (i>=res_total) j=0;
		else if (j+i>=res_total) j=res_total-i;
		total=res_total;
		ii=i;
		length = j;
		return j;
	}
	else {
		char * p;
		int k;
		sprintf(cmd, "%d %d %s\n", i, j, q);
		mask_query(cmd);
		buft = 0;
		if (send_line(s,cmd)==0) 
		if (_wait_read(s)==0) {
		    p = read_line(s);
		    if (p==0) return -1;
		    mask_query(p);
		    sscanf(p,"%d %d",&total,&length);
                    return length;
		}
		return -1;
	}
}

