#ifndef W_SEARCH_HEADER

#define W_SEARCH_HEADER

#define MAX_BIG_KEEP 100000		//最多查询结果包含 MAX_BIG_KEEP 条记录
#define MAX_KEEP 100			//一篇文章中保存同个单词的 MAX_KEEP 个不同位置
#define MAX_SORT 1000			//最多对 MAX_SORT 篇文章排序
//#define MAX_WORD 2513257		//最多 MAX_WORD 条单词，包含中英文 (哈西时使用，因此最好是质数)
//#define MAX_FILE 15132573		//最多 MAX_FILE 条文件             (哈西时使用，因此最好是质数)
#define MAX_WORD 253257
#define MAX_FILE 532570
#define MAX_LIB_NUMBER 500		//最多产生多少库文件
#define SPLIT_NUM 64*1024*1024	//占用内存的分割大小，当index占用超过 SPLIT_NUM 字节之后，内存文件将保存到磁盘
#define INDEX_RANGE 10*1024		//每个文件最多只索引 INDEX_RANGE 范围，后面内容可忽略。这个值必须小于64*1024，必须在2byte范围内
#if INDEX_RANGE>65535
#define INT_INDEX
#endif
#define ALPHA_LENGTH 12			//英文字母组成的词长度不超过 ALPHA_LENGTH
#define NUMBER_LENGTH 5			//数字组成的词长度不超过 NUMBER_LENGTH
#define CHINESE_LENGTH 10		//汉字组成的词长度不超过 CHINESE_LENGTH
#define MAX_KEY_WORDS 100		//一次查询包含最多 MAX_KEY_WORDS 个关键词

struct data_head {				//用于保存字符串数据
    int id;
    char * content;
} ** hash_word, ** hash_file, * word, * file, * titled, * tag;

struct index_file {				//用于保存索引结果
    int fileid;
    char post;
#ifdef INT_INDEX
    unsigned int * pos;
#else
    unsigned short int * pos;
#endif
    struct index_file * next;
};

struct index_lib {				//倒排表的链表头结构
    int total;
    struct index_file * head;
} * lib;


int wordt, filet, memt=0, indext = 0;

int hash_t(char * s, int t)		//计算哈西值
{
    unsigned int i,j=0;
    for(i=0;i<strlen(s);i++)
        j=(j*256+s[i])%t;
    return j;
}

int exists_word(char * fn)		//判断词库是否存在某个单词
{
    int i,j;
    i = hash_t(fn, MAX_WORD);
    j = i;
    while(hash_word[i]) {
        if(!strcmp(hash_word[i]->content, fn))
            return 1;
        i++;
        if(i>=MAX_WORD) i=0;
        if (i==j) {
            printf("MAX_WORD %d excceed! need increse\n", MAX_WORD);
        }
    }
    return 0;
}

int add_file(char * fn)			//增加一个文件
{
    if (filet>=MAX_FILE) {
        printf("MAX_FILE %d excceed! need increase\n", MAX_FILE);
    }
    file[filet].id = filet;
    file[filet].content = (char*)malloc(strlen(fn)+1);
    strcpy(file[filet].content, fn);
    return (filet++);
}

int get_wordid(char * fn,int add)	//得到单词的ID号，如果不存在该单词，并且add==1，则添加单词
{
    int i,j;
    i = hash_t(fn, MAX_WORD);
    j = i;
    while(hash_word[i]) {
        if(!strcmp(hash_word[i]->content, fn))
            return hash_word[i]->id;
        i++;
        if(i>=MAX_WORD) i=0;
        if (i==j) {
            printf("MAX_WORD %d excceed! need increase\n", MAX_WORD);
        }
    }
    if (!add) return -1;
    word[wordt].id = wordt;
    word[wordt].content = (char*)malloc(strlen(fn)+1);
    strcpy(word[wordt].content, fn);
    hash_word[i] = &(word[wordt]);
    return (wordt++);
}


#endif
