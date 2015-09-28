<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=gb2312">
</head>
<body>
<?php
dl("wPHPExt.so");

if (isset($_GET["q"])) 
	$q = $_GET["q"];
else 
	$q = "查询变量 q 为空";

/*
 * 有引擎版
 * $ret = wSearch_init_query(0,"127.0.0.1",4875);
 */

$ret = wSearch_init_query(0,"127.0.0.1",4875);
/*
 * 无引擎版
 * $ret = wSearch_init_query(1,"/AKA-wSearch/",0);
 */


if (!$ret) {
	$t = wSearch_do_query(0,10,$q);
	echo "查询'", $q , "' 共有",wSearch_fetch_total(),"条记录\n";
	echo "<hr>\n";
	for($i=0;$i<count($t);$i++) {
		echo "第", $i+1, "条<br>";
		echo "DATAID: ",$t[$i]['DATAID'];
		echo "<br>\n";
		echo "TITLE: ",$t[$i]['TITLE'];
		echo "<br>\n";
		echo "TAG: ",$t[$i]['TAG'];
		echo "<br>\n";
		echo wSearch_highlight($t[$i]['DATAID'],$q,'<font color="red">','</font>');
		echo "<br>\n<hr>\n";
	}
	wSearch_done_query();
}else{
	echo "<h1>wSearch_init_query error code # $ret !</h1>\n";
}
?>
<font color=green size=+2>
Copyright info: <?php echo wSearch_copyright(); ?>
<hr>
<a href="http://wsearch.aka.cn/">
中文100G全文检索引擎
wSearch</a><br>
<a href="http://www.aka.cn/">阿卡信息技术(北京)有限公司 (AKA Information & Technology Co., Ltd.)</a><br>
</font>

</body>
</html>
