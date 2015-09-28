#include "wQuery.h"
#include "php.h"
//#include "php_config.h"

ZEND_FUNCTION(wsearch_init_query);
ZEND_FUNCTION(wsearch_done_query);
ZEND_FUNCTION(wsearch_do_query);
ZEND_FUNCTION(wsearch_fetch_total);
ZEND_FUNCTION(wsearch_highlight);
ZEND_FUNCTION(wsearch_highlight_content);
ZEND_FUNCTION(coreSearch_copyright);

PHP_MINFO_FUNCTION(coreSearch);

zend_function_entry coreSearch_functions[] =
{
	ZEND_FE(wsearch_init_query, NULL)
	ZEND_FE(wsearch_done_query, NULL)
	ZEND_FE(wsearch_do_query, NULL)
	ZEND_FE(wsearch_fetch_total, NULL)
	ZEND_FE(wsearch_highlight, NULL)
	ZEND_FE(wsearch_highlight_content, NULL)
	ZEND_FE(coreSearch_copyright, NULL)
	{NULL, NULL, NULL}
};

zend_module_entry coreSearch_module_entry =
{
    STANDARD_MODULE_HEADER,
    "coreSearch",
    coreSearch_functions,
    NULL, 
    NULL, 
    NULL, 
    NULL, 
    PHP_MINFO(coreSearch),
    NO_VERSION_YET,
    STANDARD_MODULE_PROPERTIES
};

/* implement standard "stub" routine to introduce ourselves to Zend */
#if COMPILE_DL
ZEND_GET_MODULE(coreSearch)
#endif


PHP_MINFO_FUNCTION(coreSearch) 
{ 
php_info_print_table_start(); 
php_info_print_table_row(2, "coreSearch Support", "Enabled"); 
php_info_print_table_row(2, "Copyright", "阿卡信息技术(北京)有限公司 版权所有 http://coresearch.aka.cn/"); 
php_info_print_table_row(2, "coreSearch", "海量中文全文检索引擎 SDK"); 
php_info_print_table_row(2, "Capability", "100GB容量下，500ms的检索速度"); 
php_info_print_table_row(2, "Version", "1.2"); 
php_info_print_table_end(); 
} 



ZEND_FUNCTION(wsearch_init_query)
{
	int ac;
	int bk,bigmem,dd_len;
	char *dd;
	
	ac = ZEND_NUM_ARGS();
	if(ac != 3 || zend_parse_parameters(3 TSRMLS_CC,"lsl",&bk,&dd,&dd_len,&bigmem) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	RETURN_LONG(init_query(bk,dd,bigmem));
}

ZEND_FUNCTION(wsearch_done_query)
{
	int ac;
	
	ac = ZEND_NUM_ARGS();
	if(ac != 0) {
		WRONG_PARAM_COUNT;
	}
	RETURN_LONG(done_query());
}

ZEND_FUNCTION(wsearch_highlight)
{
	int ac;
	int fn_len, qn_len, preTag_len, postTag_len;
	char *fn, *qn, *preTag, *postTag;
	
	ac = ZEND_NUM_ARGS();
	if(ac != 4 || zend_parse_parameters(4 TSRMLS_CC,"ssss",&fn,&fn_len,&qn,&qn_len,&preTag,&preTag_len,&postTag,&postTag_len) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	RETURN_STRING(highlight(fn,qn,preTag,postTag),1);
}

ZEND_FUNCTION(wsearch_highlight_content)
{
	int ac;
	int content_len, qn_len, preTag_len, postTag_len, len;
	char *content, *qn, *preTag, *postTag;
	
	ac = ZEND_NUM_ARGS();
	if(ac != 5 || zend_parse_parameters(5 TSRMLS_CC,"slsss",&content,&content_len,&len,&qn,&qn_len,&preTag,&preTag_len,&postTag,&postTag_len) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	RETURN_STRING(highlight_content(content,len,qn,preTag,postTag),1);
}

ZEND_FUNCTION(wsearch_do_query)
{
	int ac;
	int i,j,q_len,n;
	char *q;
	char s1[1024],s2[1024],s3[1024];
	zval *element;
	
	ac = ZEND_NUM_ARGS();
	if(ac != 3 || zend_parse_parameters(3 TSRMLS_CC,"lls",&i,&j,&q,&q_len) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
    if (array_init(return_value) == FAILURE)
        RETURN_FALSE;
    n = do_query(i,j,q);
    for(i=0;i<n;i++) {
    	wfetch(s1,s2,s3);
		MAKE_STD_ZVAL(element);
		array_init(element);
		add_assoc_string(element, "DATAID", s1, 1);
		add_assoc_string(element, "TITLE", s2, 1);
		add_assoc_string(element, "TAG", s3, 1);
		zend_hash_index_update(Z_ARRVAL_P(return_value), i, (void *) &element, sizeof(zval *), NULL);
    }
}

ZEND_FUNCTION(wsearch_fetch_total)
{
	RETURN_LONG(fetch_total());
}

ZEND_FUNCTION(coreSearch_copyright)
{
	RETURN_STRING("Product Info: <br>Full Text Search Engine CoreSearch, AKA Information & Technology Co., Ltd.<br>Copyright 2003-2004, all rights reserved.<br>http://www.aka.cn/<br>http://coresearch.aka.cn/<br>service@aka.cn",1);
}
