// WIndexer.c : 定义控制台应用程序的入口点。
//

#include "wIndexer.h"
#include "wsearch.h"

char d1[120],d2[120];

int mask(char * s) //加密解密
{
	int i;
	for(i=0;i<strlen(s);i++)
		s[i] ^= 3;
	return 0;
}

int init_wordlib() //初始化单词库，从标准词库中读取单词
{
    FILE *fp,*fp2;
	int i = 0;
    char word[100];
    if (!(fp=fopen("index.dat","rb"))) 
		return -1;
    while(1) {
		int j = 0;
		while (!feof(fp)) {
			word[j] = fgetc(fp);
			j++;
			if ((word[j-1]^3)==10) {
				word[j] = 0;
				break;
			}
		}
		if (feof(fp)) break;
		if (!word[0]) break;
		i++;
		mask(word);
        get_wordid(word,1);
    }
    fclose(fp);
	return i;
}

int load_database() //读入已有的数据库，做增量索引
{
    FILE * fp;
    int i,j,k;
    unsigned char c;
	char fn[200];
    struct index_file * p, * q;

    wordt = 0;
    filet = 0;
	memt = 0;
    memset(hash_word, 0, sizeof(struct data_head *)*MAX_WORD);
    memset(lib, 0, sizeof(struct index_lib)*MAX_WORD);
	memset(file, 0, sizeof(struct data_head)*MAX_FILE);
	memset(titled, 0, sizeof(struct data_head)*MAX_FILE);
	memset(tag, 0, sizeof(struct data_head)*MAX_FILE);

	sprintf(fn, "%s%s", d1, "_word_.index");
	if (fp = fopen(fn, "rb")) {
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
	}

	sprintf(fn, "%s%s", d1, "_file_.index");
	if (fp = fopen(fn, "rb")) {
		fread(&filet, 1, 4, fp);
		fclose(fp);
	}

    sprintf(fn, "%s%d.index", d2, indext);
	if (fp = fopen(fn, "rb")) {
		fread(&j, 1, 4, fp);
		for(i=0;i<j;i++) {
    		fread(&lib[i].total, 1, 4, fp);
			lib[i].head = NULL;
			q = NULL;
			for(k=0;k<lib[i].total;k++) {
				p = (struct index_file *)malloc(sizeof(struct index_file));
				fread(&p->fileid, 1, 4, fp);
				fread(&p->post, 1, 1, fp);
#ifdef INT_INDEX
				p->pos = (unsigned int *)malloc(p->post*4);
				memt += sizeof(struct index_file)+p->post*4;
				fread(p->pos, 1, p->post*4, fp);
#else
				p->pos = (unsigned short int *)malloc(p->post*2);
				memt += sizeof(struct index_file)+p->post*2;
				fread(p->pos, 1, p->post*2, fp);
#endif
				p->next = NULL;
				if (q) q->next = p;
				else lib[i].head = p;
				q = p;
			}
		}
		fclose(fp);
	}

	return 0;
}

int save_database() //保存索引结果
{
    int i,oldfilet;
    char c,newfile=1;
    short int cc;
    FILE* fp;
    char fn[200];
    struct index_file * p, * q;
    
	sprintf(fn, "%s%s", d1, "_word_.index");
	fp = fopen(fn, "wb");
	fwrite(&wordt, 1, 4, fp);
	for(i=0;i<wordt;i++) {
		c = strlen(word[i].content);
		fwrite(&c, 1, 1, fp);
		fwrite(word[i].content, 1, c, fp);
		free(word[i].content);
	}
	fclose(fp);

	sprintf(fn, "%s%s", d1, "_file_.index");
	if (fp = fopen(fn, "rb")) {
		newfile = 0;
		fread(&oldfilet, 1, 4, fp);
		fclose(fp);
	}

	if (newfile) { //从头索引
		sprintf(fn, "%s%s", d1, "_file_.index");
		fp = fopen(fn, "wb");
		fwrite(&filet, 1, 4, fp);
		for(i=0;i<filet;i++) {
			cc = strlen(file[i].content);
			fwrite(&cc, 1, 2, fp);
			fwrite(file[i].content, 1, cc, fp);
			free(file[i].content);
		}
		fclose(fp);
	    
		sprintf(fn, "%s%s", d1, "_title_.index");
		fp = fopen(fn, "wb");
		fwrite(&filet, 1, 4, fp);
		for(i=0;i<filet;i++) {
			c = strlen(titled[i].content);
			fwrite(&c, 1, 1, fp);
			fwrite(titled[i].content, 1, c, fp);
			free(titled[i].content);
		}
		fclose(fp);
	    
		sprintf(fn, "%s%s", d1, "_tag_.index");
		fp = fopen(fn, "wb");
		fwrite(&filet, 1, 4, fp);
		for(i=0;i<filet;i++) {
			c = strlen(tag[i].content);
			fwrite(&c, 1, 1, fp);
			fwrite(tag[i].content, 1, c, fp);
			free(tag[i].content);
		}
		fclose(fp);
	}
	else { //增量索引
		sprintf(fn, "%s%s", d1, "_file_.index");
		fp = fopen(fn, "r+b");
		fwrite(&filet, 1, 4, fp);
		fseek(fp,0,SEEK_END);
		for(i=oldfilet;i<filet;i++) {
			cc = strlen(file[i].content);
			fwrite(&cc, 1, 2, fp);
			fwrite(file[i].content, 1, cc, fp);
			free(file[i].content);
		}
		fclose(fp);
	    
		sprintf(fn, "%s%s", d1, "_title_.index");
		fp = fopen(fn, "r+b");
		fwrite(&filet, 1, 4, fp);
		fseek(fp,0,SEEK_END);
		for(i=oldfilet;i<filet;i++) {
			c = strlen(titled[i].content);
			fwrite(&c, 1, 1, fp);
			fwrite(titled[i].content, 1, c, fp);
			free(titled[i].content);
		}
		fclose(fp);
	    
		sprintf(fn, "%s%s", d1, "_tag_.index");
		fp = fopen(fn, "r+b");
		fwrite(&filet, 1, 4, fp);
		fseek(fp,0,SEEK_END);
		for(i=oldfilet;i<filet;i++) {
			c = strlen(tag[i].content);
			fwrite(&c, 1, 1, fp);
			fwrite(tag[i].content, 1, c, fp);
			free(tag[i].content);
		}
		fclose(fp);
	}
    
    sprintf(fn, "%s%d.index", d2, indext);
    fp = fopen(fn, "wb");
    fwrite(&wordt, 1, 4, fp);
    for(i=0;i<wordt;i++) {
        int t = 0;
    	fwrite(&lib[i].total, 1, 4, fp);
    	p = lib[i].head;
    	while(p) {
			fwrite(&p->fileid, 1, 4, fp);
			fwrite(&p->post, 1, 1, fp);
#ifdef INT_INDEX
			fwrite(p->pos, 1, p->post*4, fp);
#else
			fwrite(p->pos, 1, p->post*2, fp);
#endif
    	    free(p->pos);
    	    q = p->next;
    	    free(p);
    	    p = q;
    	}
    }
    fclose(fp);
	return 0;
}

int init_index(char * u1, char * u2) //初始化整个索引过程
{
    filet = 0;
    strcpy(d1,u1);
    strcpy(d2,u2);
	word = NULL; file = NULL; titled = NULL; tag = NULL; hash_word = NULL; lib = NULL;
    if ((word = (struct data_head *) malloc(sizeof(struct data_head)*MAX_WORD)) && 
		(file = (struct data_head *) malloc(sizeof(struct data_head)*MAX_FILE)) &&
		(titled = (struct data_head *) malloc(sizeof(struct data_head)*MAX_FILE)) &&
		(tag = (struct data_head *) malloc(sizeof(struct data_head)*MAX_FILE)) &&
		(hash_word = (struct data_head **) malloc(sizeof(struct data_head *)*MAX_WORD)) &&
		(lib = (struct index_lib *) malloc(sizeof(struct index_lib)*MAX_WORD))) { //分配足够内存
		load_database(d1,d2);
		init_wordlib();
		return 0;
	}
	if (word) { free(word); word = NULL; }
	if (file) { free(file); file = NULL; }
	if (titled) { free(titled); titled = NULL; }
	if (tag) { free(tag); tag = NULL; }
	if (hash_word) { free(hash_word); hash_word = NULL; }
	if (lib) { free(lib); lib = NULL; }
	
	return -1;
}

int done_index()
{
	int i;
	unsigned int ** libs;
	FILE * fp;
	char fn[200];

	save_database();

	if (word) free(word);
	if (file) free(file);
	if (titled) free(titled);
	if (tag) free(tag);
	if (hash_word) free(hash_word);
	if (lib) free(lib);

	indext++;
	libs = (unsigned int **) malloc(4*indext);
	for(i=0;i<indext;i++)
		libs[i] = (unsigned int *) malloc(4*MAX_WORD);
	for(i=0;i<indext;i++)
		memset(libs[i], 0, 4*MAX_WORD);
    for(i=0;i<indext;i++) { //生成倒排表的索引_all_.index
		int j,k,t;
		char c;
        sprintf(fn, "%s%d.index", d2, i);
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
		fclose(fp);
    }
	sprintf(fn, "%s%s", d2, "_all_.index");
    fp = fopen(fn, "wb");
	for(i=0;i<indext;i++) {
		fwrite(libs[i],1,4*MAX_WORD,fp);
		free(libs[i]);
	}
    fclose(fp);
	free(libs);

	return 0;
}

int poss[8000][2],posst;

void put_word(int wordid, int pos) //将一个词放入正排表
{
    poss[posst][0] = wordid;
    poss[posst][1] = pos;
    posst++;
}

void finalize_index(int fileid) //将正排表保存为倒排
{
    int i,j,k,u;
    struct index_file * t, * p;
    
    for(i=0;i<posst;i++) 
    if(poss[i][0]!=-1&&lib[poss[i][0]].total<MAX_BIG_KEEP) {
        u = poss[i][0];
        k = 0;
        for(j=i;j<posst;j++)
			if(u==poss[j][0]) k++;
        t = (struct index_file *)malloc(sizeof(struct index_file));
        t->fileid = fileid;
        if(k>MAX_KEEP) k = MAX_KEEP;
        t->post = k;
#ifdef INT_INDEX
        t->pos = (unsigned int *)malloc(4*k);
        memt += sizeof(struct index_file)+4*k;
#else
        t->pos = (unsigned short int *)malloc(2*k);
        memt += sizeof(struct index_file)+2*k;
#endif
        k = 0;
        for(j=i;j<posst;j++)
        if(u==poss[j][0]) {
            if (k<MAX_KEEP) t->pos[k++] = poss[j][1];
            poss[j][0] = -1;
        }
        lib[u].total++;
        p = lib[u].head;
        t->next = p;
        lib[u].head = t;
    }
}

void process_word(int fileid, int pos, char * word) //处理单个词
{
    int i,wordid;
    for(i=0;i<strlen(word);i++)
        word[i] = tolower(word[i]);
    wordid = get_wordid(word,1);
    put_word(wordid, pos);
}

void process_chinese_word(int fileid, int pos, char * word) //处理未分词的中文词
{
    int i,j,len=strlen(word),wordid;
    char tmp[1024];
    i=0;
    while(i<len) {
        j=len-i;
        if(j>CHINESE_LENGTH) j=CHINESE_LENGTH;
        strncpy(tmp, word+i, j);
        tmp[j] = 0;
        while(!exists_word(tmp)&&j>2) {
            j-=2;
            strncpy(tmp, word+i, j);
            tmp[j] = 0;
        }
        wordid = get_wordid(tmp,1);
        put_word(wordid, pos+i);
        i+=j;
    }
}

int index_file_content(char * dataid, char * title, char * tags, unsigned char * content, int len) //索引某个文件的内容content
{
    int c=0;
    char buf[1024];
    int buft;
    int fileid, pos=0;

	while (memt>=SPLIT_NUM) {
		save_database();
		indext++;
		load_database();
	}

    fileid = add_file(dataid);
    posst = 0;
    titled[fileid].content = (char*) malloc(strlen(title)+1);
    strcpy(titled[fileid].content, title);
    tag[fileid].content = (char*) malloc(strlen(tags)+1);
    strcpy(tag[fileid].content, tags);

    buft = 0;
    while(pos<len) {
        c = content[pos++];
#ifdef PROCESS_XML
		if(c=='<') {
			while(content[pos++]!='>');
		}
		else
#endif
        if(c>='A'&&c<='Z'||c>='a'&&c<='z') {
            int p=pos-1;
            buf[buft++] = c;
            while(pos<len) {
                c = content[pos++];
				if(!(c>='A'&&c<='Z'||c>='a'&&c<='z'||c>='0'&&c<='9')||buft>=ALPHA_LENGTH) {
					pos--;
					break;
				}
                buf[buft++] = c;
            }
            buf[buft] = 0;
            process_word(fileid, p, buf);
            buft = 0;
        }
        else if(c>='0'&&c<='9') {
            int p=pos-1;
            buf[buft++] = c;
            while(pos<len) {
                c = content[pos++];
				if(!(c>='0'&&c<='9')||buft>=NUMBER_LENGTH) {
					pos--;
					break;
				}
                buf[buft++] = c;
            }
            buf[buft] = 0;
            process_word(fileid, p, buf);
            buft = 0;
        }
        else if(c>=0xb0&&c<=0xf7) {
            int ch = 1, space = 0, p = pos-1;
            buf[buft++] = c;
            while(pos<len) {
                c = content[pos++];
                if(ch==1) {
                    ch = 0;
                    if(c>=0xa1&&c<=0xfe)
                        buf[buft++] = c;
					else {
						pos--;
                        break;
					}
                }
                else {
                    if(c>=0xb0&&c<=0xf7) {
                        if(buft>=100) break;
                        ch = 1;
                        buf[buft++] = c;
                        space = 0;
                    }
                    else if(c==' ') {
                    }
                    else if(c==10||c==13) {
                        space++;
                        if(space>=3) break;
                    }
					else {
						pos--;
						break;
					}
                }
            }
            buf[buft/2*2] = 0;
            if(buft/2)
                process_chinese_word(fileid, p, buf);
            buft = 0;
        }
        else {
            if(c>=0x80&&pos<len)
                pos++;
            if(c==0x1b) { //过滤ANSI
                while(pos<len) {
                    c = content[pos++];
                    if(c>='a'&&c<='z'||c>='A'&&c<='Z') break;
                }
            }
        }
        if(pos>INDEX_RANGE) break;
    }

    finalize_index(fileid);
    return 0;
}

int index_file(char * dataid, char * title, char * tags) //索引某个文件
{
	unsigned char buf[INDEX_RANGE];
	FILE * fp;
	int len,ret;
	if (fp = fopen(dataid, "rb")) {
		len = fread(buf, 1, INDEX_RANGE, fp);
		fclose(fp);
		if (len)
			ret = index_file_content(dataid, title, tags, buf, len);
		return ret;
	}
	else
		return 0;
}


