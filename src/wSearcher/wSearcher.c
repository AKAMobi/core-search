// WSearcher.c : 定义控制台应用程序的入口点。
//

#include "wSearcher.h"
#include "wsearch.h"

unsigned int ** libs;
int LIB_NUMBER=0;
FILE * fps[MAX_LIB_NUMBER], *f_file, *f_title, *f_tag;
char filename[250];

int option_loadall = 0;
int res_id[MAX_BIG_KEEP];
int res_total=-1,startt=0,toomany=0;

#ifndef WIN32
#define SOCKET int
#define closesocket close
#endif

int wSearchSort(char * s)		//支持用户排序的函数，如果使用则需要定义#define USER_DEFINE_SORT
{
	return atoi(s);
}

int get_total()					//得到搜索结果总数
{
	return res_total;
}

char *get_filename(int i)		//得到某个id对应的文件名(DATAID)
{
    if (res_id[i]>=filet)
        return NULL;
    else if (option_loadall) {
        strcpy(filename, file[res_id[i]].content);
        return filename;
    }
	else {
	short int cc;
		fseek(f_file, file[res_id[i]].id, SEEK_SET);
		fread(&cc,1,2,f_file);
		fread(filename,1,cc,f_file);
		filename[cc] = 0;
		return filename;
	}
}

char *get_filetitle(int i)		//得到某个id对应的文件标题信息
{
    if (res_id[i]>=filet) {
        return NULL;
    }
    else if (option_loadall) {
        strcpy(filename, titled[res_id[i]].content);
        return filename;
    }
	else {
		unsigned char c;
		fseek(f_title, titled[res_id[i]].id, SEEK_SET);
		fread(&c,1,1,f_title);
		fread(filename,1,c,f_title);
		filename[c] = 0;
		return filename;
	}
}

char *get_filetag(int i)		//得到某个id对应的文件附加串信息
{
    if (res_id[i]>=filet) {
        return NULL;
    }
    else if (option_loadall) {
        strcpy(filename, tag[res_id[i]].content);
        return filename;
    }
	else {
		unsigned char c;
		fseek(f_tag, tag[res_id[i]].id, SEEK_SET);
		fread(&c,1,1,f_tag);
		fread(filename,1,c,f_tag);
		filename[c] = 0;
		return filename;
	}
}



int load_database(char * dd)	//读入数据库，dd是路径信息
{
    FILE * fp;
    int i,j,k,t;
    unsigned char c;
	char fn[200];
    short int cc;

    wordt = 0;
    filet = 0;
    memset(hash_word, 0, sizeof(struct data_head *)*MAX_WORD);
	for(i=0;i<LIB_NUMBER;i++)
		memset(libs[i], 0, 4*MAX_WORD);
    
	sprintf(fn, "%s%s", dd, "_word_.index"); //读入单词word库
    fp = fopen(fn, "rb");
    fread(&wordt, 1, 4, fp);
    for(i=0;i<wordt;i++) {
        fread(&c, 1, 1, fp);
        word[i].id = i;
        word[i].content = (char*) malloc(c+1);
        word[i].content[c] = 0;
        fread(word[i].content, 1, c, fp);
        j = hash_t(word[i].content, MAX_WORD);
        while(hash_word[j]) {j++; if(j>=MAX_WORD) j=0;}
        hash_word[j] = &word[i];
    }
    fclose(fp);

	sprintf(fn, "%s%s", dd, "_file_.index"); //读入文件名file库
    fp = fopen(fn, "rb");
    fread(&filet, 1, 4, fp);
    for(i=0;i<filet;i++) {
		if (option_loadall) {
	        fread(&cc, 1, 2, fp);
			file[i].id = i;
			file[i].content = (char*) malloc(cc+1);
			file[i].content[cc] = 0;
			fread(file[i].content, 1, cc, fp);
		}
		else {
			file[i].id = ftell(fp);
	        fread(&cc, 1, 2, fp);
			fseek(fp, cc, SEEK_CUR);
		}
    }
	if (option_loadall)
		fclose(fp);
	else
		f_file = fp;
    
	sprintf(fn, "%s%s", dd, "_title_.index"); //读入标题title库
    fp = fopen(fn, "rb");
    fread(&filet, 1, 4, fp);
    for(i=0;i<filet;i++) {
		if (option_loadall) {
	        fread(&c, 1, 1, fp);
			titled[i].id = i;
			titled[i].content = (char*) malloc(c+1);
			titled[i].content[c] = 0;
			fread(titled[i].content, 1, c, fp);
		}
		else {
			titled[i].id = ftell(fp);
	        fread(&c, 1, 1, fp);
			fseek(fp, c, SEEK_CUR);
		}
    }
	if (option_loadall)
		fclose(fp);
	else
		f_title = fp;
    
	sprintf(fn, "%s%s", dd, "_tag_.index"); //读入附加串tag库
    fp = fopen(fn, "rb");
    fread(&filet, 1, 4, fp);
    for(i=0;i<filet;i++) {
		if (option_loadall) {
	        fread(&c, 1, 1, fp);
			tag[i].id = i;
			tag[i].content = (char*) malloc(c+1);
			tag[i].content[c] = 0;
			fread(tag[i].content, 1, c, fp);
		}
		else {
			tag[i].id = ftell(fp);
	        fread(&c, 1, 1, fp);
			fseek(fp, c, SEEK_CUR);
		}
    }
	if (option_loadall)
		fclose(fp);
	else
		f_tag = fp;
    
	sprintf(fn, "%s%s", dd, "_all_.index"); //读入倒排表的索引
    fp=fopen(fn,"rb");
    if(fp==NULL) { //如果不存在，则新建索引
        for(i=0;i<LIB_NUMBER;i++) {
            sprintf(fn, "%s%d.index", dd, i);
            fp = fopen(fn, "rb");
            fread(&t, 1, 4, fp);
            for(j=0;j<t;j++) {
                libs[i][j] = ftell(fp);
                fread(&k, 1, 4, fp);
                while(k) {
                    k--;
					fseek(fp, 4, SEEK_CUR);
                    fread(&c, 1, 1, fp);
#ifdef INT_INDEX
					fseek(fp, (unsigned char)c*4, SEEK_CUR);
#else
					fseek(fp, (unsigned char)c*2, SEEK_CUR);
#endif
                }
            }
            fps[i] = fp;
//            printf("%ld\n",ftell(fp));
        }
		sprintf(fn, "%s%s", dd, "_all_.index");
        fp = fopen(fn, "wb");
		for(i=0;i<LIB_NUMBER;i++)
			fwrite(libs[i],1,4*MAX_WORD,fp);
        fclose(fp);
    }
    else {
		for(i=0;i<LIB_NUMBER;i++)
	        fread(libs[i],1,4*MAX_WORD,fp);
        fclose(fp);
        for(i=0;i<LIB_NUMBER;i++) {
            sprintf(fn, "%s%d.index", dd, i);
            fps[i] = fopen(fn, "rb");
        }
    }
	return 0;
}

void save_database()			//关闭结束数据库
{
	int i;
	for(i=0;i<LIB_NUMBER;i++)
		fclose(fps[i]);
	if (!option_loadall) {
		fclose(f_file);
		fclose(f_title);
		fclose(f_tag);
	}
}

char back_dd[200];
int back_bigmem;

int init_search(char * dd, int bigmem)	//初始化search
{
	char buf[200];
	FILE * fp;

	strcpy(back_dd, dd);
	back_bigmem = bigmem;
	option_loadall = bigmem;
	while(1) {
		sprintf(buf, "%s%d.index", dd, LIB_NUMBER);
		fp = fopen(buf, "rb");
		if (fp) fclose(fp);
		else break;
		LIB_NUMBER++;
	}
	if (LIB_NUMBER==0) return -1;
	word = NULL; file = NULL; titled = NULL; tag = NULL; hash_word = NULL; libs = NULL;
    if ((word = (struct data_head *) malloc(sizeof(struct data_head)*MAX_WORD)) && 
		(file = (struct data_head *) malloc(sizeof(struct data_head)*MAX_FILE)) &&
		(titled = (struct data_head *) malloc(sizeof(struct data_head)*MAX_FILE)) &&
		(tag = (struct data_head *) malloc(sizeof(struct data_head)*MAX_FILE)) &&
		(hash_word = (struct data_head **) malloc(sizeof(struct data_head *)*MAX_WORD)) &&
	    (libs = (unsigned int **) malloc(4*LIB_NUMBER))) {		//分配内存
		int ok = 1, i;
		for(i=0;i<LIB_NUMBER;i++) {
			libs[i] = (unsigned int *) malloc(4*MAX_WORD);
			if (!libs[i]) {
				ok = 0;
				break;
			}
		}
		if(ok) {
			load_database(dd);
			return 0;
		}
	}
	if (word) { free(word); word = NULL; }
	if (file) { free(file); file = NULL; }
	if (titled) { free(titled); titled = NULL; }
	if (tag) { free(tag); tag = NULL; }
	if (hash_word) { free(hash_word); hash_word = NULL; }
	if (libs) { free(libs); libs = NULL; }
	
	return -1;
}

int done_search()			//搜索结束处理
{
	int i;
	save_database();
	if (option_loadall)
		for (i=0;i<filet;i++) {
			if (file[i].content) free(file[i].content);
			if (titled[i].content) free(titled[i].content);
			if (tag[i].content) free(tag[i].content);
		}
	if (word) free(word);
	if (file) free(file);
	if (titled) free(titled);
	if (tag) free(tag);
	if (hash_word) free(hash_word);
	if (libs) {
		for(i=0;i<LIB_NUMBER;i++)
			free(libs[i]);
		free(libs);
	}

	return 0;
}

short int weight[MAX_BIG_KEEP][MAX_KEEP+1], res_pos[MAX_BIG_KEEP][MAX_KEEP+1];
int keys_id[MAX_KEY_WORDS][3], keys_total=0, dbi, minus=0;

void process_word(int pos, char * word)	//处理单个词，pos是单词位置，word是单词内容
{
    int i,j,t,li,lt,wordid;
    unsigned short int pt[MAX_KEEP];
    int pp=0,nfirst=0;
    struct index_file * ltt;
    FILE * fp;

    for(i=0;i<strlen(word);i++)
    if(word[i]>='A'&&word[i]<='Z')
        word[i] = tolower(word[i]);
    wordid = get_wordid(word,0);
    if(wordid==-1||libs[dbi][wordid]==0) {
        if(!minus)
            res_total = startt;
        return;
    }
    
    if(keys_total<MAX_KEY_WORDS&&!minus) {
        keys_id[keys_total][0] = wordid;
        keys_id[keys_total][1] = pos;
        keys_id[keys_total][2] = strlen(word);
        keys_total++;
    }
    else if(!minus) return;
    
    fp = fps[dbi];
    fseek(fp, libs[dbi][wordid], SEEK_SET);
    fread(&lt, 1, 4, fp);
    ltt = (struct index_file *) malloc(sizeof(struct index_file)*lt);
    for(i=0;i<lt;i++) {
        fread(&ltt[i].fileid, 1, 4, fp);
        fread(&ltt[i].post, 1, 1, fp);
        j = ltt[i].post;
        if(j<0) j+=256;
#ifdef INT_INDEX
        ltt[i].pos = (unsigned int *) malloc(j*4);
        fread(ltt[i].pos, 1, j*4, fp);
#else
        ltt[i].pos = (unsigned short int *) malloc(j*2);
        fread(ltt[i].pos, 1, j*2, fp);
#endif
    }
    
    if(lt==0) {
        if(!minus)
            res_total=startt;
        return;
    }
    if(res_total==-1) {
        pp=-1;
        res_total=startt;
    }
    else pp=startt;
    for(li=0;li<lt;li++) {
	    nfirst++;
		if (nfirst>=MAX_BIG_KEEP) {
		    if(pp!=-1&&!minus) {
		        for(;pp<res_total;pp++)
		            res_id[pp] = -1;
		    }
			break;
		}
		j = ltt[li].fileid;
		t = ltt[li].post;
#ifdef INT_INDEX
		memcpy(pt, ltt[li].pos, t*4);
#else
		memcpy(pt, ltt[li].pos, t*2);
#endif
		if(pp==-1) {
		    if(res_total<MAX_BIG_KEEP) {
    		    res_id[res_total] = j;
#ifdef INT_INDEX
    		    memcpy(&(res_pos[res_total][1]), pt, t*4);
#else
    		    memcpy(&(res_pos[res_total][1]), pt, t*2);
#endif
    		    res_pos[res_total][0] = t;
    		    for(j=1;j<=t;j++)
    		    weight[res_total][j] = sqrt((double)res_pos[res_total][j]); //计算权值
	    	    res_total++;
	    	}
	    	else break;
		}
		else {
		    while((res_id[pp]>j||res_id[pp]==-1)&&pp<res_total) {
		        if(!minus)
    		        res_id[pp] = -1;
		        pp++;
		    }
		    if(pp>=res_total) break;
		    if(j==res_id[pp]&&pp<res_total) 
    		    if(minus) {
    		        res_id[pp] = -1;
    		        pp++;
    		    }
    		    else
    		    {
    		        int tt=(keys_id[keys_total-2][1]+keys_id[keys_total-2][2]==keys_id[keys_total-1][1]),tp=1,
    		            lk=keys_id[keys_total-2][2];
    		        j = 0;
    		        for(i=1;i<=res_pos[pp][0];i++)
    		        if(res_pos[pp][i]!=-1)
    		        {
    		            while(j+1<t&&abs(res_pos[pp][i]-pt[j]+lk)>abs(res_pos[pp][i]-pt[j+1]+lk)) j++;
    		            if(tt&&(res_pos[pp][i]-pt[j]+lk))
    		                res_pos[pp][i]=-1;
    		            else {
    		                weight[pp][i]+=sqrt((double)abs(res_pos[pp][i]-pt[j]+lk));
    		                res_pos[pp][i]=pt[j];
    		                tp = 0; //计算权值
    		            }
    		        }
    		        if(tp)
    		            res_id[pp] = -1;
    		        pp++;
    		    }
		}
	}
    for(li=0;li<lt;li++)
        free(ltt[li].pos);
	free(ltt);
    if(pp!=-1&&!minus) {
        for(;pp<res_total;pp++)
            res_id[pp] = -1;
    }
	if(pp!=-1) {
        j = res_total;
        res_total = startt;
        for(i=startt;i<j;i++) 
        if(res_id[i]!=-1) {
            res_id[res_total] = res_id[i];
            memcpy(res_pos[res_total], res_pos[i], sizeof(res_pos[i]));
            memcpy(weight[res_total], weight[i], sizeof(weight[i]));
            res_total++;
        }
	}
}

void process_chinese_word(int pos, char * word)	//处理未分词过的中文字串
{
    int i,j,len=strlen(word);
    char tmp[1024];
    i=0;
    while(i<len) {
        j=len-i;
        if(j>CHINESE_LENGTH) j=CHINESE_LENGTH;
        strncpy(tmp, word+i, j);
        tmp[j] = 0;
        while(!exists_word(tmp)&&j>2) { //分词处理
            j-=2;
            strncpy(tmp, word+i, j);
            tmp[j] = 0;
        }
        process_word(pos+i, tmp);
        i+=j;
    }
}

void do_query_all(unsigned char * inp) //查询词，inp为词内容
{
    int c=0, i, j, k;
    char buf[1024];
    int wt[MAX_SORT];
    int buft,inpt=0;

    res_total = -1; startt = 0;
    toomany = 0;
    
    for(dbi=0; dbi<LIB_NUMBER; dbi++) {	//分LIB_NUMBER个库分别查询，然后融合
        buft = 0;
        inpt = 0; c = 0;
        keys_total = 0;
        while(inpt<strlen((char*)inp)) {
            c = inp[inpt++];
            
            if(c>='A'&&c<='Z'||c>='a'&&c<='z') {	//英文
                int p=inpt-1;
                buf[buft++] = c;
                while(inpt<strlen((char*)inp)) {
                    c = inp[inpt++];
                    if(!c) break;
					if(!(c>='A'&&c<='Z'||c>='a'&&c<='z'||c>='0'&&c<='9')||buft>=ALPHA_LENGTH) {
						inpt--;
						break;
					}
                    buf[buft++] = c;
                }
                buf[buft] = 0;
                process_word(p, buf);
                minus = 0;
                buft = 0;
            }
            else if(c>='0'&&c<='9') {		//数字
                int p=inpt-1;
                buf[buft++] = c;
                while(inpt<strlen((char*)inp)) {
                    c = inp[inpt++];
					if(!(c>='0'&&c<='9')||buft>=NUMBER_LENGTH) {
						inpt--;
						break;
					}
                    buf[buft++] = c;
                }
                buf[buft] = 0;
                process_word(p, buf);
                minus = 0;
                buft = 0;
            }
            else if(c>=0xb0&&c<=0xf7) {		//中文
                int ch = 1, space = 0, p = inpt-1;
                buf[buft++] = c;
                while(inpt<strlen((char*)inp)) {
                    c = inp[inpt++];
                    if(ch==1) {
                        ch = 0;
                        if(c>=0xa1&&c<=0xfe)
                            buf[buft++] = c;
						else {
							inpt--;
                            break;
						}
                    }
                    else {
                        if(c>=0xb0&&c<=0xf7) {
                            if(space) break;
                            if(buft>=100) break;
                            ch = 1;
                            buf[buft++] = c;
                            space = 0;
                        }
						else {
							inpt--;
							break;
						}
                    }
                }
                buf[buft/2*2] = 0;
                if(buft/2)
                    process_chinese_word(p, buf);
                minus = 0;
                buft = 0;
            }
            else {
                if(c>=0x80&&inpt<strlen((char*)inp)) inpt++; 
                if(c=='-'&&res_total!=-1) minus = 1;
            }
        }
        
        startt = res_total;
        res_total = -1;
        if(startt>=MAX_BIG_KEEP) break;
    }

    res_total = startt;
//    if(res_total>MAX_KEEP) {
//        toomany = res_total;
//        res_total = MAX_KEEP;
//    }
    
    for(i=0;i<res_total&&i<MAX_SORT;i++) {	//排序
#ifdef USER_DEFINE_SORT
		wt[i] = wSearchSort(get_filetag(i));
#else
        k = -1;
        for(j=1;j<=res_pos[i][0];j++)
        if(res_pos[i][j]!=-1&&(k==-1||weight[i][j]<weight[i][k]))
            k = j;
        if(k==-1) wt[i] = 10000000;
        else wt[i] = weight[i][k];
#endif
    }
    for(i=0;i<res_total&&i<MAX_SORT;i++)
        for(j=i+1;j<res_total&&j<MAX_SORT;j++)
        if(wt[i]>wt[j]) {
            k=wt[i];
            wt[i]=wt[j];
            wt[j]=k;
            k=res_id[i];
            res_id[i]=res_id[j];
            res_id[j]=k;
        }
	if (res_total < 0) res_total = 0;
}

#ifdef WIN32
int set_nonblocking (SOCKET f)		//设置为非阻塞
{
	u_long i=1;
	if (ioctlsocket(f, FIONBIO, &i) != 0)
		return -1;
    return 0;
}
#else
int set_nonblocking (SOCKET f)
{
    if (fcntl (f, F_SETFL, O_NONBLOCK) != 0)
        return -1;
    return 0;
}
#endif

int _wait_write (SOCKET f)
{
    struct timeval to;
    int numfds;
    fd_set writefds;
    FD_ZERO(&writefds);
    FD_SET (f, &writefds);
    to.tv_usec = 0;
    to.tv_sec = 60;
	numfds = select(f+1, 0, &writefds, 0, &to);
	if (numfds == -1) return -1;
	if (FD_ISSET(f, &writefds)) return 0;
	return -1;
}

int mask_search(char * s)		//加密函数
{
	int i;
	for(i=0;i<strlen(s);i++)
		s[i] ^= 3;
	return 0;
}

#define TIME_OUT 5

// by zixia FILE * qfp;
int on=1;

int send_cmd(SOCKET s, char * buf)
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

int handle_connection(int i)
{
    int j,k;
    j = recv(g_tcp[i].fd, g_tcp[i].buf+g_tcp[i].buft, 1024-g_tcp[i].buft, 0);
    if (j<=0) 
        return -1;
    g_tcp[i].buft += j;
    if (g_tcp[i].buf[g_tcp[i].buft-1]==('\n'^3)||g_tcp[i].buf[g_tcp[i].buft-1]==('\r'^3)) {
        char cmd[1024*2],q[1024];
        strncpy(cmd, g_tcp[i].buf, g_tcp[i].buft);
        cmd[g_tcp[i].buft] = 0;
        g_tcp[i].buft = 0;
        mask_search(cmd);
    	
        if (!strncmp(cmd, "STOP SERVICE", 12)) {
            if (on) done_search();
            on = 0;
            printf("Stop Service\n");
        }
        else if (!strncmp(cmd, "START SERVICE", 13)) {
            if (!on) init_search(back_dd,back_bigmem);
            on = 1;
            printf("Start Service\n");
        }
        else if (on) {
            int di,dj,t;
            sscanf(cmd, "%d %d", &di, &dj);
            t = 0;
            while(cmd[t]!=' '&&t<strlen(cmd)) t++;
            t ++;
            while(cmd[t]!=' '&&t<strlen(cmd)) t++;
            t ++;
            if (t>=strlen(cmd)) return -1;
            strcpy(q,cmd+t);

	/* by zixia
            if (0==di)
                fprintf(qfp, "%s\n", q);

            fflush(qfp);
	*/
            do_query_all((unsigned char*)q);
            if (di>=res_total) j=0;
            else if (dj+di>=res_total) j=res_total-di;
            else j=dj;
            sprintf(cmd, "%d %d\n", res_total, j);
            mask_search(cmd);
            if (_wait_write(g_tcp[i].fd)==0)
            if (send_cmd(g_tcp[i].fd, cmd)==0)
            for(k=di;k<di+j;k++) {
                sprintf(cmd, "%s\n", get_filename(k));
                mask_search(cmd);
                if (send_cmd(g_tcp[i].fd, cmd)==-1) break;
                sprintf(cmd, "%s\n", get_filetitle(k));
                mask_search(cmd);
                if (send_cmd(g_tcp[i].fd, cmd)==-1) break;
                sprintf(cmd, "%s\n", get_filetag(k));
                mask_search(cmd);
                if (send_cmd(g_tcp[i].fd, cmd)==-1) break;
            }
	    printf("key:%s result:%d\n", q, res_total);
        }else{
		// by zxiia: engine not on, return -1 assume error
		return -1;
	}
    	
    }
}

void start_listen()				//建立socket服务
{
    struct sockaddr_in sin;
    int   numfds, i;
    struct timeval to;
    int selectErrors = 0;
    int maxfd = 0, usert = 0;
    fd_set readfds;
    fd_set bakfds;
    SOCKET sockfd, csock;
    int value;

#ifdef WIN32
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2),&wsaData);
#endif
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
#ifdef WIN32
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
#else
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    //sin.sin_addr.s_addr = inet_addr("127.0.0.1");
#endif
    sin.sin_port = htons(LISTEN_PORT);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
#ifndef WIN32
	{
		int opt = 1; int len = sizeof(opt);
		setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&opt,(socklen_t)&len);     
	}
#endif
    set_nonblocking(sockfd);
	if ((bind(sockfd, (struct sockaddr *) &sin, sizeof(sin)) < 0) || (listen(sockfd, 5) < 0)) {
#ifdef WIN32
		WSACleanup();
#endif
		return;
	}
    // by zixia qfp = fopen("query.log", "a");	//保存查询内容

    FD_ZERO(&bakfds);
    FD_SET (sockfd, &bakfds);
    maxfd=sockfd;
    while (1)
    {
	    to.tv_usec = 0;
	    to.tv_sec = 60;
	    readfds = bakfds;
	    numfds = select (maxfd + 1, &readfds, 0, 0, &to);
        if (numfds == -1)
        {
            if (++selectErrors == 5)
            {
				break;
		    }
			continue;
		}
		if(numfds==0)
			continue;
		selectErrors = 0;
        if (FD_ISSET (sockfd, &readfds))
        {
            int sinsize=sizeof(sin);
            SOCKET f;
            if ((f = accept (sockfd, (struct sockaddr *) &sin, &sinsize)) > 0)
	    {
	        if(f<CONNECTION_NUM)
	        {
	            g_tcp[usert].fd=f;
	            g_tcp[usert].ip=sin.sin_addr.s_addr;
	            g_tcp[usert].port=sin.sin_port;
	            g_tcp[usert].buft=0;
	            usert ++;
	            set_nonblocking(f);
                    FD_SET (f,&bakfds);
#ifndef WIN32
                    if(maxfd<f)
                        maxfd=f;
#endif
	        }
	        else
	        {
                    closesocket(f);
	        }
            }
        }
        for(i=0;i<usert;i++)
        {
            if(g_tcp[i].fd && FD_ISSET(g_tcp[i].fd,&readfds))
            {
                if(handle_connection(i)<=0)
                {
                    FD_CLR(g_tcp[i].fd,&bakfds);
		    closesocket(g_tcp[i].fd);
		    memcpy(&g_tcp[i],&g_tcp[usert-1],sizeof(TcpInfo));
                    usert --;
                }
            }    
        }
    }
    // by zixia fclose(qfp);
}

#ifndef SINGLESEARCH
#ifdef WIN32
int _tmain(int argc, _TCHAR* argv[])
#else
int main(int argc, char ** argv)
#endif
{
    fprintf ( stderr, "coreSearch Full Text Search Engine ver 1.0, AKA Information & Technology Co., Ltd.\n" \
		"http://coresearch.aka.cn/ http://www.aka.cn/ service@aka.cn\n" \
		"Copyright 2003-2004, All rights reserved.\n" \
		"\n" );
	if (init_search("/home/AKA-coreSearch/",0)==-1){
	fprintf ( stderr, "wSearch init error, pls check data directory(/home/AKA-coreSearch).\n" );
	 return -1;
    }
    start_listen();
    done_search();
	return 0;
}

#endif
