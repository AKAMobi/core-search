<html>
<head>
</head>
<body>
<pre>
<%
Set obj = Server.CreateObject("wASPExt.ATLSearch.1")
q = request("q")
ret = obj.asp_init_query(0,"127.0.0.1",4875)
if ret<>0 then
  t = obj.asp_do_query(0,100,q)
  Response.write "змЪ§:"&obj.asp_fetch_total()&"\n"
  For i=0 to UBound(t)
    Response.write t(i)(0)
    Response.write "\n"
    Response.write t(i)(1)
    Response.write "\n"
    Response.write t(i)(2)
    Response.write "\n"
    Response.write obj.asp_highlight(t(i)(0),q,"<B>","</B>")
  next
  obj.asp_done_query();
end if
%>
</pre>
</body>
</html>
