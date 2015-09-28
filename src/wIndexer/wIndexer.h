#ifdef WIN32
#include <stdlib.h>
#include <tchar.h>
#endif

#include <stdio.h>

/**************************************************
 * 函数名: init_index                             *
 * 功能:   初始化索引,分配内存以及读入必要数据    *
 * 参数:   u1 - 词库以及文件库所在的路径，末尾要  *
 *              加"\\"。例如："C:\\wordlib\\"。   *
 *              当前路径则用空字符串，如""        *
 *         u2 - 索引文件0.index,1.index..所在路径 *
 *              当前路径则用空字符串""            *
 * 返回:   0表示成功，-1表示失败                  *
 **************************************************/
int init_index(char * u1, char * u2);
/**************************************************
 * 函数名: done_index                             *
 * 功能:   结束索引任务,清除内存以及保存必要数据  *
 * 参数:   无                                     *
 * 返回:   0表示成功，-1表示失败                  *
 **************************************************/
int done_index();
/**************************************************
 * 函数名: index_file                             *
 * 功能:   索引单个文件                           *
 * 参数:   dataid - 文件名称。索引函数将读取该文件内容*
 *         title - 文件标题信息                   *
 *         tags  - 文件附加串信息                 *
 * 返回:   0表示成功，-1表示失败                  *
 **************************************************/
int index_file(char * dataid, char * title, char * tags);
/**************************************************
 * 函数名: index_file_content                     *
 * 功能:   索引单个文件                           *
 * 参数:   fn - 文件名称。索引函数不读取该文件内容*
 *         title - 文件标题信息                   *
 *         tags  - 文件附加串信息                 *
 *         content - 文件实际内容                 *
 *         len  - 文件实际内容的长度              *
 * 返回:   0表示成功，-1表示失败                  *
 **************************************************/
int index_file_content(char * dataid, char * title, char * tags, unsigned char * content, int len);
