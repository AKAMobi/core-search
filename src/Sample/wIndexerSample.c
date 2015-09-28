#include "../wIndexer/wIndexer.h"

#ifndef WIN32
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

void listdir(char * s) //列目录，方便测试用
{
  DIR* d;
  struct dirent* t;
  struct stat st;
  char di[200];

  chdir( s );

  d = opendir(".");
  while((t=readdir(d))!=NULL) {
	int l;
    stat(t->d_name,&st);
    if(st.st_mode&S_IFDIR&&t->d_name[0]!='.') {
      if(chdir(t->d_name)==0) {
	    sprintf(di,"%s%s/",s,t->d_name);
        listdir(di);
        chdir("..");
      }
    }
	l = strlen(t->d_name);
	if(!(st.st_mode&S_IFDIR)&&!(l>3&&t->d_name[l-1]=='z'&&t->d_name[l-2]=='g'&&t->d_name[l-3]=='t')) {
	  sprintf(di,"%s%s",s,t->d_name);
      printf("%s, %s\n",di,t->d_name);
	  index_file(di,t->d_name,"");
	}
  }
  closedir(d);
}
#else

#include <io.h>

void listdir(char *s) 
{
	long hFile;
	char di[200];
	struct _finddata_t fileinfo;

	_chdir(s);
	sprintf(di,"%s%s",s,"*");
	if ((hFile=_findfirst(di,&fileinfo)) != -1)
	{
		do
		{
			if (!(fileinfo.attrib & _A_SUBDIR))
			{
				sprintf(di,"%s%s",s,fileinfo.name);
				printf("%s, %s\n",di,fileinfo.name);
				index_file(di,fileinfo.name,"");
			}
			else if (fileinfo.name[0]!='.')
			{
				if(_chdir(fileinfo.name)==0) 
				{
					sprintf(di,"%s%s\\",s,fileinfo.name);
					listdir(di);
					_chdir("..");
				}
			}
		} while (_findnext(hFile,&fileinfo) == 0);
		_findclose(hFile);
	}
}

#endif


#ifndef SINGLEINDEX
#ifdef WIN32
int _tmain(int argc, _TCHAR* argv[])
#else
int main(int argc, char ** argv)
#endif
{
#ifndef WIN32
	init_index("/home/AKA-wSearch/","/home/AKA-wSearch/");
	listdir(argv[1]);
//	listdir("/home/zixia/Dev/wSearch/K12-wSearch/10000/");
#else
	init_index("C:\\Program Files\\AKA-coreSearch\\","C:\\Program Files\\AKA-coreSearch\\");
//	listdir("C:\\DS\\");
	listdir(argv[1]);
//	index_file("ReadMe.txt", "README", "abc");
//	index_file("WIndexer.c", "WIndexer", "cba");
#endif
	done_index();
	return 0;
}

#endif


