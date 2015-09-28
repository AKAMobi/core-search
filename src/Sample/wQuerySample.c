#include "../wQuery/wQuery.h"
#include <time.h>
/*
void main_new()
{
	int i,len;
	
	init_query(0,"",0);
	len = do_query(0,100,"test");
	printf("关键字:%s 总数:%d 输出:%d-%d",cmd,fetch_total(),i+1,i+len);
	for(i=0;i<len;i++) {
		char a1[200],a2[200],a3[200];
		fetch(a1,a2,a3);
		printf("%s\n%s\n%s\n%s\n",a1,a2,a3,highlight(a1,cmd,"<B>","</B>"));
	}
	done_query();
}
*/


#ifndef SINGLEQUERY
#ifdef WIN32
int _tmain(int argc, _TCHAR* argv[])
#else
int main(int argc, char ** argv)
#endif
{
	int i,j,k=0,len;
	time_t timer;
	char cmd[1024];
	if (argc>=2&&(!strcmp(argv[1],"-s")||!strcmp(argv[1],"-S"))) {
		k = 1;
	}
	if (argc<4&&k==0 || argc<5&&k==1) {
		printf("Syntax:  wQuery [-s] start length keyword\nExample: wQuery 0 100 测试\n");
		return 0;
	}
	cmd[0] = 0;
	for(i=3+k;i<argc;i++) {
		strcat(cmd,argv[i]);
		strcat(cmd," ");
	}
	i = atoi(argv[1+k]);
	j = atoi(argv[2+k]);
	init_query(k,"",0);
	//printf ( "time before do_query: %d\n", time(&timer) );
	len = do_query(i,j,cmd);
	//printf ( "time after do_query: %d\n", time(&timer) );
	printf("########## 关键字:%s 总数:%d 输出:%d-%d ##########\n",cmd,fetch_total(),i+1,i+len);
	for(i=0;i<len;i++) {
		char a1[200],a2[200],a3[200];
		wfetch(a1,a2,a3);
		printf("========== Result #%d ==========\n",i+1);
		printf("ID: [%s]\nTitle: [%s]\nTag: [%s]\nContent: [%s]\n",a1,a2,a3,highlight(a1,cmd,"<B>","</B>"));
	}
	done_query();
	return 0;
}

#endif
