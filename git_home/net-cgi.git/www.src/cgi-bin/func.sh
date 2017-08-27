#!/bin/sh
nvram=/bin/config
CONSOLE=/dev/console


GUI_Region=$($nvram get GUI_Region)

print_charset()
{
        CHARSET="UTF-8"
        echo -n  "$CHARSET"
}

print_language_js()
{
        lang_file="language/$GUI_Region.js"
        path="/www/$lang_file"

        if [ -f $path ]; then
                echo "<script language=javascript type=text/javascript src='/$lang_file'></script>"
        else
                echo "<script language=javascript type=text/javascript src='/language/English.js'></script>"
        fi

}

print_cgi_header() # $1: content_type
{
        time_stamp_dni=$(cat /proc/uptime)
        local content_type date

        content_type="$1"
        [ "x$content_type" = "x" ] && content_type="text/html"
        date=`date -u '+%a, %d %b %Y %H:%M:%S %Z'`

cat <<EOF
Content-type: $content_type

EOF
}

print_http_header()
{
date=`date -u '+%a, %d %b %Y %H:%M:%S %Z'`
cat <<EOF
HTTP/1.0 200 OK
Server: uhttpd/1.0.0
Date: $date
Cache-Control: no-cache
Pragma: no-cache
Expires: 0
Content-Type: text/html; charset="UTF-8"
Connection: close

EOF
}

print_menu_header()
{
    echo '<HTML><HEAD><TITLE>WNR2000</TITLE>'
    echo "<META http-equiv=content-type content='text/html;charset=UTF-8'>"
    echo '<META content="MSHTML 6.00.2800.1141" name=GENERATOR>'
    echo "<script language=javascript type=text/javascript src='/languages-en.js'></script>"
}

print_http_refresh () # $1: page to be refresh, $2: delay_time
{
        local delay_time
        delay_time="$2"
        [ "x$delay_time" = "x" ] && delay_time="0"

		local url="$1"
		
print_http_header
cat <<EOF
<HTML>
<HEAD><meta http-equiv="Refresh" content="$delay_time; url=$url">
<Meta http-equiv="Pragma" Content="no-cache">
<META HTTP-equiv="Cache-Control" content="no-cache">
<Meta http-equiv="Expires" Content="0">
<META http-equiv='Content-Type' content='text/html; charset=$(print_charset)'>
EOF
print_language_js
cat <<EOF
<link rel="stylesheet" type="text/css" href="../style/hijack_style.css">
</head>
<body bgcolor=#ffffff>
<div  align="center"><br /><br /><b><script>document.write(bh_plz_wait_moment)</script></b><br /><br /></div>
<div class="waiting_img_div" align="center">
<img src="/image/wait30.gif" />
</div>

</body>
</HTML>
EOF
}

print_wait_message () 
{
cat <<EOF
<HTML>
<HEAD><meta http-equiv="Refresh">
<Meta http-equiv="Pragma" Content="no-cache">
<META HTTP-equiv="Cache-Control" content="no-cache">
<Meta http-equiv="Expires" Content="0">
<META http-equiv='Content-Type' content='text/html; charset=$(print_charset)'>
EOF
print_language_js
cat <<EOF
<link rel="stylesheet" href="/form.css">
</HEAD>
<BODY bgcolor=#ffffff>
<tr><td colspan=2><br><img src=/liteblue.gif width=100% height=12></td></tr>
<script>document.write(wait_message)</script>
</BODY>
</HTML>
EOF
}

print_401 ()
{
cat <<EOF
HTTP/1.0 401 Unauthorized
Server: uhttpd/1.0.0
WWW-Authenticate: Basic realm="NETGEAR ${AUTH_REALM}"
Cache-Control: no-cache
Pragma: no-cache
Expires: 0
Content-Type: text/html; charset="UTF-8"
Connection: close

<HTML><HEAD>
<META http-equiv='Pragma' content='no-cache'>
<META http-equiv='Cache-Control' content='no-cache'>
<TITLE> 401 Authorization</TITLE>
<script language=javascript type=text/javascript>
function cancelevent()
{
        location.href='/unauth.cgi';
	}
	</script>
	</HEAD><BODY onload=cancelevent()></BODY></HTML>
EOF
}

print_multi_login ()
{

print_http_header
cat <<EOF
<html>
<body>
<script language=javascript type=text/javascript>
top.location.href="/multi_login.html";
</script>
</body>
</html>
EOF
}
