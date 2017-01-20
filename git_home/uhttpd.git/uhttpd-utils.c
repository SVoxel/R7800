/*
 * uhttpd - Tiny single-threaded httpd - Utility functions
 *
 *   Copyright (C) 2010 Jo-Philipp Wich <xm@subsignal.org>
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#define _XOPEN_SOURCE 500	/* crypt() */
#define _BSD_SOURCE			/* strcasecmp(), strncasecmp() */

#include "uhttpd.h"
#include "uhttpd-file.h"
#include "uhttpd-utils.h"
#include "string.h"
#include "stdio.h"
#include <sys/sysinfo.h>

#ifdef HAVE_TLS
#include "uhttpd-tls.h"
#endif

struct auth_login
{
	char login_ip[32];
	char login_mac[32];
	char login_dev[32];
	long login_time;
};

enum {
        AUTH_OK,
        AUTH_TIMEOUT,
        AUTH_MULTI,
	AUTH_MULTI_GUEST
};

int update_login(struct client *cl);
int update_login_guest(struct client *cl);
char *cat_file(char *name);

static char *uh_index_files[] = {
	"start.html",
	"start.htm",
	"default.html",
	"default.htm"
};

const char * sa_straddr(void *sa)
{
	static char str[INET6_ADDRSTRLEN];
	struct sockaddr_in *v4 = (struct sockaddr_in *)sa;
	struct sockaddr_in6 *v6 = (struct sockaddr_in6 *)sa;

	if( v4->sin_family == AF_INET )
		return inet_ntop(AF_INET, &(v4->sin_addr), str, sizeof(str));
	else
		return inet_ntop(AF_INET6, &(v6->sin6_addr), str, sizeof(str));
}

const char * sa_strport(void *sa)
{
	static char str[6];
	snprintf(str, sizeof(str), "%i", sa_port(sa));
	return str;
}

int sa_port(void *sa)
{
	return ntohs(((struct sockaddr_in6 *)sa)->sin6_port);
}

int sa_rfc1918(void *sa)
{
	struct sockaddr_in *v4 = (struct sockaddr_in *)sa;
	unsigned long a = htonl(v4->sin_addr.s_addr);

	if( v4->sin_family == AF_INET )
	{
		return ((a >= 0x0A000000) && (a <= 0x0AFFFFFF)) ||
		       ((a >= 0xAC100000) && (a <= 0xAC1FFFFF)) ||
		       ((a >= 0xC0A80000) && (a <= 0xC0A8FFFF));
	}

	return 0;
}

/* Simple strstr() like function that takes len arguments for both haystack and needle. */
char *strfind(char *haystack, int hslen, const char *needle, int ndlen)
{
	int match = 0;
	int i, j;

	for( i = 0; i < hslen; i++ )
	{
		if( haystack[i] == needle[0] )
		{
			match = ((ndlen == 1) || ((i + ndlen) <= hslen));

			for( j = 1; (j < ndlen) && ((i + j) < hslen); j++ )
			{
				if( haystack[i+j] != needle[j] )
				{
					match = 0;
					break;
				}
			}

			if( match )
				return &haystack[i];
		}
	}

	return NULL;
}

/* interruptable select() */
int select_intr(int n, fd_set *r, fd_set *w, fd_set *e, struct timeval *t)
{
	int rv;
	sigset_t ssn, sso;

	/* unblock SIGCHLD */
	sigemptyset(&ssn);
	sigaddset(&ssn, SIGCHLD);
	sigaddset(&ssn, SIGPIPE);
	sigprocmask(SIG_UNBLOCK, &ssn, &sso);

	rv = select(n, r, w, e, t);

	/* restore signal mask */
	sigprocmask(SIG_SETMASK, &sso, NULL);

	return rv;
}


int uh_tcp_send_lowlevel(struct client *cl, const char *buf, int len)
{
	fd_set writer;
	struct timeval timeout;

	FD_ZERO(&writer);
	FD_SET(cl->socket, &writer);

	timeout.tv_sec = cl->server->conf->network_timeout;
	timeout.tv_usec = 0;

	if (select(cl->socket + 1, NULL, &writer, NULL, &timeout) > 0)
		return send(cl->socket, buf, len, 0);

	return -1;
}

int uh_tcp_send(struct client *cl, const char *buf, int len)
{
#ifdef HAVE_TLS
	if (cl->tls)
		return cl->server->conf->tls_send(cl, (void *)buf, len);
	else
#endif
		return uh_tcp_send_lowlevel(cl, buf, len);
}

int uh_tcp_peek(struct client *cl, char *buf, int len)
{
	/* sanity check, prevent overflowing peek buffer */
	if (len > sizeof(cl->peekbuf))
		return -1;

	int sz = uh_tcp_recv(cl, buf, len);

	/* store received data in peek buffer */
	if( sz > 0 )
	{
		cl->peeklen = sz;
		memcpy(cl->peekbuf, buf, sz);
	}

	return sz;
}

int uh_tcp_recv_lowlevel(struct client *cl, char *buf, int len)
{
	fd_set reader;
	struct timeval timeout;

	FD_ZERO(&reader);
	FD_SET(cl->socket, &reader);

	timeout.tv_sec  = cl->server->conf->network_timeout;
	timeout.tv_usec = 0;

	if (select(cl->socket + 1, &reader, NULL, NULL, &timeout) > 0)
		return recv(cl->socket, buf, len, 0);

	return -1;
}

int uh_tcp_recv(struct client *cl, char *buf, int len)
{
	int sz = 0;
	int rsz = 0;

	/* first serve data from peek buffer */
	if (cl->peeklen > 0)
	{
		sz = min(cl->peeklen, len);
		len -= sz; cl->peeklen -= sz;
		memcpy(buf, cl->peekbuf, sz);
		memmove(cl->peekbuf, &cl->peekbuf[sz], cl->peeklen);
	}

	/* caller wants more */
	if (len > 0)
	{
#ifdef HAVE_TLS
		if (cl->tls)
			rsz = cl->server->conf->tls_recv(cl, (void *)&buf[sz], len);
		else
#endif
			rsz = uh_tcp_recv_lowlevel(cl, (void *)&buf[sz], len);

		if (rsz < 0)
			return rsz;

		sz += rsz;
	}

	return sz;
}


int uh_http_sendhf(struct client *cl, int code, const char *summary, const char *fmt, ...)
{
	va_list ap;

	char buffer[UH_LIMIT_MSGHEAD];
	int len;

	len = snprintf(buffer, sizeof(buffer),
		"HTTP/1.1 %03i %s\r\n"
		"Connection: close\r\n"
		"Content-Type: text/plain\r\n"
		"Transfer-Encoding: chunked\r\n\r\n",
			code, summary
	);

	ensure_ret(uh_tcp_send(cl, buffer, len));

	va_start(ap, fmt);
	len = vsnprintf(buffer, sizeof(buffer), fmt, ap);
	va_end(ap);

	ensure_ret(uh_http_sendc(cl, buffer, len));
	ensure_ret(uh_http_sendc(cl, NULL, 0));

	return 0;
}


int uh_http_sendc(struct client *cl, const char *data, int len)
{
	char chunk[8];
	int clen;

	if( len == -1 )
		len = strlen(data);

	if( len > 0 )
	{
		clen = snprintf(chunk, sizeof(chunk), "%X\r\n", len);
		ensure_ret(uh_tcp_send(cl, chunk, clen));
		ensure_ret(uh_tcp_send(cl, data, len));
		ensure_ret(uh_tcp_send(cl, "\r\n", 2));
	}
	else
	{
		ensure_ret(uh_tcp_send(cl, "0\r\n\r\n", 5));
	}

	return 0;
}

int uh_http_sendf(
	struct client *cl, struct http_request *req, const char *fmt, ...
) {
	va_list ap;
	char buffer[UH_LIMIT_MSGHEAD];
	int len;

	va_start(ap, fmt);
	len = vsnprintf(buffer, sizeof(buffer), fmt, ap);
	va_end(ap);

	if( (req != NULL) && (req->version > 1.0) )
		ensure_ret(uh_http_sendc(cl, buffer, len));
	else if( len > 0 )
		ensure_ret(uh_tcp_send(cl, buffer, len));

	return 0;
}

int uh_http_send(
	struct client *cl, struct http_request *req, const char *buf, int len
) {
	if( len < 0 )
		len = strlen(buf);

	if( (req != NULL) && (req->version > 1.0) )
		ensure_ret(uh_http_sendc(cl, buf, len));
	else if( len > 0 )
		ensure_ret(uh_tcp_send(cl, buf, len));

	return 0;
}


int uh_urldecode(char *buf, int blen, const char *src, int slen)
{
	int i;
	int len = 0;

#define hex(x) \
	(((x) <= '9') ? ((x) - '0') : \
		(((x) <= 'F') ? ((x) - 'A' + 10) : \
			((x) - 'a' + 10)))

	for( i = 0; (i <= slen) && (i <= blen); i++ )
	{
		if( src[i] == '%' )
		{
			if( ((i+2) <= slen) && isxdigit(src[i+1]) && isxdigit(src[i+2]) )
			{
				buf[len++] = (char)(16 * hex(src[i+1]) + hex(src[i+2]));
				i += 2;
			}
			else
			{
				buf[len++] = '%';
			}
		}
		else
		{
			buf[len++] = src[i];
		}
	}

	return len;
}

int uh_urlencode(char *buf, int blen, const char *src, int slen)
{
	int i;
	int len = 0;
	const char hex[] = "0123456789abcdef";

	for( i = 0; (i <= slen) && (i <= blen); i++ )
	{
		if( isalnum(src[i]) || (src[i] == '-') || (src[i] == '_') ||
		    (src[i] == '.') || (src[i] == '~') )
		{
			buf[len++] = src[i];
		}
		else if( (len+3) <= blen )
		{
			buf[len++] = '%';
			buf[len++] = hex[(src[i] >> 4) & 15];
			buf[len++] = hex[(src[i] & 15) & 15];
		}
		else
		{
			break;
		}
	}

	return len;
}

int uh_b64decode(char *buf, int blen, const unsigned char *src, int slen)
{
	int i = 0;
	int len = 0;

	unsigned int cin  = 0;
	unsigned int cout = 0;


	for( i = 0; (i <= slen) && (src[i] != 0); i++ )
	{
		cin = src[i];

		if( (cin >= '0') && (cin <= '9') )
			cin = cin - '0' + 52;
		else if( (cin >= 'A') && (cin <= 'Z') )
			cin = cin - 'A';
		else if( (cin >= 'a') && (cin <= 'z') )
			cin = cin - 'a' + 26;
		else if( cin == '+' )
			cin = 62;
		else if( cin == '/' )
			cin = 63;
		else if( cin == '=' )
			cin = 0;
		else
			continue;

		cout = (cout << 6) | cin;

		if( (i % 4) == 3 )
		{
			if( (len + 3) < blen )
			{
				buf[len++] = (char)(cout >> 16);
				buf[len++] = (char)(cout >> 8);
				buf[len++] = (char)(cout);
			}
			else
			{
				break;
			}
		}
	}

	buf[len++] = 0;
	return len;
}

static char * canonpath(const char *path, char *path_resolved)
{
	char path_copy[PATH_MAX];
	char *path_cpy = path_copy;
	char *path_res = path_resolved;

	struct stat s;


	/* relative -> absolute */
	if( *path != '/' )
	{
		getcwd(path_copy, PATH_MAX);
		strncat(path_copy, "/", PATH_MAX - strlen(path_copy));
		strncat(path_copy, path, PATH_MAX - strlen(path_copy));
	}
	else
	{
		strncpy(path_copy, path, PATH_MAX);
	}

	/* normalize */
	while( (*path_cpy != '\0') && (path_cpy < (path_copy + PATH_MAX - 2)) )
	{
		if( *path_cpy == '/' )
		{
			/* skip repeating / */
			if( path_cpy[1] == '/' )
			{
				path_cpy++;
				continue;
			}

			/* /./ or /../ */
			else if( path_cpy[1] == '.' )
			{
				/* skip /./ */
				if( (path_cpy[2] == '/') || (path_cpy[2] == '\0') )
				{
					path_cpy += 2;
					continue;
				}

				/* collapse /x/../ */
				else if( (path_cpy[2] == '.') &&
				         ((path_cpy[3] == '/') || (path_cpy[3] == '\0'))
				) {
					while( (path_res > path_resolved) && (*--path_res != '/') )
						;

					path_cpy += 3;
					continue;
				}
			}
		}

		*path_res++ = *path_cpy++;
	}

	/* remove trailing slash if not root / */
	if( (path_res > (path_resolved+1)) && (path_res[-1] == '/') )
		path_res--;
	else if( path_res == path_resolved )
		*path_res++ = '/';

	*path_res = '\0';

	/* test access */
	if( !stat(path_resolved, &s) && (s.st_mode & S_IROTH) )
		return path_resolved;

	return NULL;
}

struct path_info * uh_path_lookup(struct client *cl, const char *url)
{
	static char path_phys[PATH_MAX];
	static char path_info[PATH_MAX];
	static struct path_info p;

	char buffer[UH_LIMIT_MSGHEAD];
	char *docroot = cl->server->conf->docroot;
	char *pathptr = NULL;

	int slash = 0;
	int no_sym = cl->server->conf->no_symlinks;
	int i = 0;
	struct stat s;

	/* back out early if url is undefined */
	if ( url == NULL )
		return NULL;

	memset(path_phys, 0, sizeof(path_phys));
	memset(path_info, 0, sizeof(path_info));
	memset(buffer, 0, sizeof(buffer));
	memset(&p, 0, sizeof(p));

	/* copy docroot */
	memcpy(buffer, docroot,
		min(strlen(docroot), sizeof(buffer) - 1));

	/* separate query string from url */
	if( (pathptr = strchr(url, '?')) != NULL )
	{
		p.query = pathptr[1] ? pathptr + 1 : NULL;

		/* urldecode component w/o query */
		if( pathptr > url )
			uh_urldecode(
				&buffer[strlen(docroot)],
				sizeof(buffer) - strlen(docroot) - 1,
				url, (int)(pathptr - url) - 1
			);
	}

	/* no query string, decode all of url */
	else
	{
		uh_urldecode(
			&buffer[strlen(docroot)],
			sizeof(buffer) - strlen(docroot) - 1,
			url, strlen(url)
		);
	}

	/* create canon path */
	for( i = strlen(buffer), slash = (buffer[max(0, i-1)] == '/'); i >= 0; i-- )
	{
		if( (buffer[i] == 0) || (buffer[i] == '/') )
		{
			memset(path_info, 0, sizeof(path_info));
			memcpy(path_info, buffer, min(i + 1, sizeof(path_info) - 1));

			if( no_sym ? realpath(path_info, path_phys)
			           : canonpath(path_info, path_phys)
			) {
				memset(path_info, 0, sizeof(path_info));
				memcpy(path_info, &buffer[i],
					min(strlen(buffer) - i, sizeof(path_info) - 1));

				break;
			}
		}
	}

	/* check whether found path is within docroot */
	if( strncmp(path_phys, docroot, strlen(docroot)) ||
	    ((path_phys[strlen(docroot)] != 0) &&
		 (path_phys[strlen(docroot)] != '/'))
	) {
		return NULL;
	}

	/* test current path */
	if( ! stat(path_phys, &p.stat) )
	{
		/* is a regular file */
		if( p.stat.st_mode & S_IFREG )
		{
			p.root = docroot;
			p.phys = path_phys;
			p.name = &path_phys[strlen(docroot)];
			p.info = path_info[0] ? path_info : NULL;
		}

		/* is a directory */
		else if( (p.stat.st_mode & S_IFDIR) && !strlen(path_info) )
		{
			/* ensure trailing slash */
			if( path_phys[strlen(path_phys)-1] != '/' )
				path_phys[strlen(path_phys)] = '/';

			/* try to locate index file */
			memset(buffer, 0, sizeof(buffer));
			memcpy(buffer, path_phys, sizeof(buffer));
			pathptr = &buffer[strlen(buffer)];

			/* if requested url resolves to a directory and a trailing slash
			   is missing in the request url, redirect the client to the same
			   url with trailing slash appended */
			if( !slash )
			{
				uh_http_sendf(cl, NULL,
					"HTTP/1.1 302 Found\r\n"
					"Location: %s%s%s\r\n"
					"Connection: close\r\n\r\n",
						&path_phys[strlen(docroot)],
						p.query ? "?" : "",
						p.query ? p.query : ""
				);

				p.redirected = 1;
			}
			else if( cl->server->conf->index_file )
			{
				strncat(buffer, cl->server->conf->index_file, sizeof(buffer));

				if( !stat(buffer, &s) && (s.st_mode & S_IFREG) )
				{
					memcpy(path_phys, buffer, sizeof(path_phys));
					memcpy(&p.stat, &s, sizeof(p.stat));
				}
			}
			else
			{
				for( i = 0; i < array_size(uh_index_files); i++ )
				{
					strncat(buffer, uh_index_files[i], sizeof(buffer));

					if( !stat(buffer, &s) && (s.st_mode & S_IFREG) )
					{
						memcpy(path_phys, buffer, sizeof(path_phys));
						memcpy(&p.stat, &s, sizeof(p.stat));
						break;
					}

					*pathptr = 0;
				}
			}

			p.root = docroot;
			p.phys = path_phys;
			p.name = &path_phys[strlen(docroot)];
		}
#ifdef SUPPORT_SOAP
		else if( strstr(path_info, "/soap/") || strstr(path_info, "/HNAP1/" ) )
		{
			p.root = docroot;
			p.phys = path_info;
			p.info = path_info;
			p.name = path_info;
		}
#endif
		/* 1.Fixed Bug28439:[USB] can't access the share folder with HTTP and HTTP(via internet)
		 * handle the special request of USB shares folder via HTTP method.
		 * 2.Fixed Bug29838:[Enable Password Recovery]the function of "Enable Password Recovery" is bad.
		 */
		else if( strstr(path_info, "/shares") || strstr(path_info, "/passwordrecovered.cgi") )
		{
			p.root = docroot;
			p.phys = path_info;
			p.info = path_info;
			p.name = path_info;
		}
	}

	return p.phys ? &p : NULL;
}


static struct auth_realm *uh_realms = NULL;

struct auth_realm * uh_auth_add(char *path, char *user, char *pass)
{
	struct auth_realm *new = NULL;
	struct passwd *pwd;

#ifdef HAVE_SHADOW
	struct spwd *spwd;
#endif

	if((new = (struct auth_realm *)malloc(sizeof(struct auth_realm))) != NULL)
	{
		memset(new, 0, sizeof(struct auth_realm));

		memcpy(new->path, path,
			min(strlen(path), sizeof(new->path) - 1));

		memcpy(new->user, user,
			min(strlen(user), sizeof(new->user) - 1));

		/* given password refers to a passwd entry */
		if( (strlen(pass) > 3) && !strncmp(pass, "$p$", 3) )
		{
#ifdef HAVE_SHADOW
			/* try to resolve shadow entry */
			if( ((spwd = getspnam(&pass[3])) != NULL) && spwd->sp_pwdp )
			{
				memcpy(new->pass, spwd->sp_pwdp,
					min(strlen(spwd->sp_pwdp), sizeof(new->pass) - 1));
			}

			else
#endif

			/* try to resolve passwd entry */
			if( ((pwd = getpwnam(&pass[3])) != NULL) && pwd->pw_passwd &&
				(pwd->pw_passwd[0] != '!') && (pwd->pw_passwd[0] != 0)
			) {
				memcpy(new->pass, pwd->pw_passwd,
					min(strlen(pwd->pw_passwd), sizeof(new->pass) - 1));
			}
		}

		/* ordinary pwd */
		else
		{
			memcpy(new->pass, pass,
				min(strlen(pass), sizeof(new->pass) - 1));
		}

		if( new->pass[0] )
		{
			new->next = uh_realms;
			uh_realms = new;

			return new;
		}

		free(new);
	}

	return NULL;
}


int uh_cgi_auth_check(
		struct client *cl, struct http_request *req, struct path_info *pi
		) {
	int i, plen, rlen, ret;
	char buffer[UH_LIMIT_MSGHEAD];
	char *user = NULL;
	char *pass = NULL;
	char *remote_addr;
	char command[128];

	ret = AUTH_TIMEOUT;
	plen = strlen(pi->name);

	rlen = strlen(cl->server->conf->cgi_prefix);

	remote_addr = sa_straddr(&cl->peeraddr);
	

	if( (plen >= rlen) && !strncasecmp(pi->name, cl->server->conf->cgi_prefix, rlen) )
	{
		/* get user and pass*/
		foreach_header(i, req->headers)
		{
			if( !strcasecmp(req->headers[i], "Authorization") &&
					(strlen(req->headers[i+1]) > 6) &&
					!strncasecmp(req->headers[i+1], "Basic ", 6)
			  ) {
				memset(buffer, 0, sizeof(buffer));
				uh_b64decode(buffer, sizeof(buffer) - 1,
						(unsigned char *) &req->headers[i+1][6],
						strlen(req->headers[i+1]) - 6);

				if( (pass = strchr(buffer, ':')) != NULL )
				{
					user = buffer;
					*pass++ = 0;
				}

				break;
			}
		}
		/* check user and pass */
		if(!strcmp(remote_addr, "127.0.0.1"))
			ret = AUTH_OK;
		else if(user != NULL && pass != NULL && 
				config_match("http_username", user) && config_match("http_passwd", pass))
		{
			if(strstr(pi->name, "/genie.cgi"))
				ret = AUTH_OK;
			else
				ret=update_login(cl);
		}
		else if(user != NULL && pass != NULL && config_match("guest_enable", "1") && config_match("http_guestname", user) && config_match("http_guestpwd", pass))
		{
			if(strstr(pi->name, "/genie.cgi"))
				ret = AUTH_OK;
			else
				ret=update_login_guest(cl);
		}
	
		if( ret == AUTH_OK )
			return 1;
		else if( ret == AUTH_MULTI )
		{
			uh_http_sendf(cl, NULL,
					"HTTP/%.1f 200 OK\r\n"
					"Server: uhttpd/1.0.0\r\n"
					"Date: %s\r\n"
					"Content-Type: text/html; charset=\"UTF-8\"\r\n"
					"Connection: close\r\n\r\n",
					req->version, uh_file_unix2date(time(NULL))); 

			uh_http_sendf(cl,NULL, "<html><head>\n"
					"<script>\n"
					"top.location.href=\"/multi_login.html\";\n"
					"</script>\n"
					"</head>\n"
					"<body bgcolor=\"#ffffff\">\n"
					"</body>\n"
					"</html>\n");

			return 0;
		}
		else
		{
			uh_http_sendf(cl, NULL,
					"HTTP/%.1f 401 Unauthorized\r\n"
					"WWW-Authenticate: Basic realm=\"NETGEAR %s\"\r\n"
					"Content-Type: text/plain\r\n"
					"Content-Length: 23\r\n\r\n"
					"Authorization Required\n",
					req->version, cat_file("/module_name")
				     );
			return 0;
		}
	}

	return 1;
}

int uh_auth_check(
	struct client *cl, struct http_request *req, struct path_info *pi
) {
	int i, plen, rlen, protected;
	char buffer[UH_LIMIT_MSGHEAD];
	char *user = NULL;
	char *pass = NULL;

	struct auth_realm *realm = NULL;

	plen = strlen(pi->name);
	protected = 0;

	/* check whether at least one realm covers the requested url */
	for( realm = uh_realms; realm; realm = realm->next )
	{
		rlen = strlen(realm->path);

		if( (plen >= rlen) && !strncasecmp(pi->name, realm->path, rlen) )
		{
			req->realm = realm;
			protected = 1;
			break;
		}
	}

	/* requested resource is covered by a realm */
	if( protected )
	{
		/* try to get client auth info */
		foreach_header(i, req->headers)
		{
			if( !strcasecmp(req->headers[i], "Authorization") &&
				(strlen(req->headers[i+1]) > 6) &&
				!strncasecmp(req->headers[i+1], "Basic ", 6)
			) {
				memset(buffer, 0, sizeof(buffer));
				uh_b64decode(buffer, sizeof(buffer) - 1,
					(unsigned char *) &req->headers[i+1][6],
					strlen(req->headers[i+1]) - 6);

				if( (pass = strchr(buffer, ':')) != NULL )
				{
					user = buffer;
					*pass++ = 0;
				}

				break;
			}
		}

		/* have client auth */
		if( user && pass )
		{
			/* find matching realm */
			for( realm = uh_realms; realm; realm = realm->next )
			{
				rlen = strlen(realm->path);

				if( (plen >= rlen) &&
				    !strncasecmp(pi->name, realm->path, rlen) &&
				    !strcmp(user, realm->user)
				) {
					req->realm = realm;
					break;
				}
			}

			/* found a realm matching the username */
			if( realm )
			{
				/* check user pass */
				if (!strcmp(pass, realm->pass) ||
				    !strcmp(crypt(pass, realm->pass), realm->pass))
					return 1;
			}
		}

		/* 401 */
		uh_http_sendf(cl, NULL,
			"HTTP/%.1f 401 Unauthorized\r\n"
			"WWW-Authenticate: Basic realm=\"%s\"\r\n"
			"Content-Type: text/plain\r\n"
			"Content-Length: 23\r\n\r\n"
			"Authorization Required\n",
				req->version, cl->server->conf->realm
		);

		return 0;
	}

	return 1;
}


static struct listener *uh_listeners = NULL;
static struct client *uh_clients = NULL;

struct listener * uh_listener_add(int sock, struct config *conf)
{
	struct listener *new = NULL;
	socklen_t sl;

	if( (new = (struct listener *)malloc(sizeof(struct listener))) != NULL )
	{
		memset(new, 0, sizeof(struct listener));

		new->socket = sock;
		new->conf   = conf;

		/* get local endpoint addr */
		sl = sizeof(struct sockaddr_in6);
		memset(&(new->addr), 0, sl);
		getsockname(sock, (struct sockaddr *) &(new->addr), &sl);

		new->next = uh_listeners;
		uh_listeners = new;

		return new;
	}

	return NULL;
}

struct listener * uh_listener_lookup(int sock)
{
	struct listener *cur = NULL;

	for( cur = uh_listeners; cur; cur = cur->next )
		if( cur->socket == sock )
			return cur;

	return NULL;
}


struct client * uh_client_add(int sock, struct listener *serv)
{
	struct client *new = NULL;
	socklen_t sl;

	if( (new = (struct client *)malloc(sizeof(struct client))) != NULL )
	{
		memset(new, 0, sizeof(struct client));

		new->socket = sock;
		new->server = serv;

		/* get remote endpoint addr */
		sl = sizeof(struct sockaddr_in6);
		memset(&(new->peeraddr), 0, sl);
		getpeername(sock, (struct sockaddr *) &(new->peeraddr), &sl);

		/* get local endpoint addr */
		sl = sizeof(struct sockaddr_in6);
		memset(&(new->servaddr), 0, sl);
		getsockname(sock, (struct sockaddr *) &(new->servaddr), &sl);

		new->next = uh_clients;
		uh_clients = new;
	}

	return new;
}

struct client * uh_client_lookup(int sock)
{
	struct client *cur = NULL;

	for( cur = uh_clients; cur; cur = cur->next )
		if( cur->socket == sock )
			return cur;

	return NULL;
}

void uh_client_remove(int sock)
{
	struct client *cur = NULL;
	struct client *prv = NULL;

	for( cur = uh_clients; cur; prv = cur, cur = cur->next )
	{
		if( cur->socket == sock )
		{
			if( prv )
				prv->next = cur->next;
			else
				uh_clients = cur->next;

			free(cur);
			break;
		}
	}
}


#ifdef HAVE_CGI
static struct interpreter *uh_interpreters = NULL;

struct interpreter * uh_interpreter_add(const char *extn, const char *path)
{
	struct interpreter *new = NULL;

	if( (new = (struct interpreter *)
			malloc(sizeof(struct interpreter))) != NULL )
	{
		memset(new, 0, sizeof(struct interpreter));

		memcpy(new->extn, extn, min(strlen(extn), sizeof(new->extn)-1));
		memcpy(new->path, path, min(strlen(path), sizeof(new->path)-1));

		new->next = uh_interpreters;
		uh_interpreters = new;

		return new;
	}

	return NULL;
}

struct interpreter * uh_interpreter_lookup(const char *path)
{
	struct interpreter *cur = NULL;
	const char *e;

	for( cur = uh_interpreters; cur; cur = cur->next )
	{
		e = &path[max(strlen(path) - strlen(cur->extn), 0)];

		if( !strcmp(e, cur->extn) )
			return cur;
	}

	return NULL;
}
#endif

int is_domain_name(char *host)
{
	char *p;
	char hostname[128];
	int dot_number=0;
	snprintf(hostname, sizeof(hostname),"%s", host);

	p=hostname;

	while(*p != '\0')
	{
		if( *p == '.')
		{
			 p++;
			 dot_number++;
		}
		else if( *p >= '0' && *p <= '9')
		{
			p++;
			continue;
		}
		else if( dot_number == 3 && *p == ':')
		{
			return 0;

		}
		else
			return 1;       // if hostname no only have number and '.', it auth have char, then it is a domain name
		if( dot_number == 4 )   // if hostname have four '.', the host name is not IP.
			return 1;
	}
	return 0;
}

int is_router_domain_addr(char *httphost)
{
	if ( httphost == NULL )
		return 0;
        else if(strcasecmp(httphost, "routerlogin.net")==0 || strcasecmp(httphost, "www.routerlogin.net")==0)
                return 0;
        else if(strcasecmp(httphost, "www.routerlogin.com")==0 || strcasecmp(httphost, "routerlogin.com")==0)
                return 0;
        else if(strcasecmp(httphost, "readyshare.routerlogin.net")==0 || strcasecmp(httphost, "readyshare.routerlogin.com")==0)
                return 0;
        else if(strcasecmp(httphost, "readyshare")==0)
                return 0;
        else if(strcasecmp(httphost, config_get("lan_ipaddr"))==0)
                return 0;
	else if(config_match("endis_ddns","1") && strncasecmp(httphost,config_get("sysDNSHost"),strlen(config_get("sysDNSHost")))==0)
		return 0;
        else
                return 1;
}

int is_ipv6_domain(char *httphost){
        char *p;
        char hostname[128];
        int colon_num = 0;
        int square_num = 1;
        int square_op_num = 1;

        snprintf(hostname, sizeof(hostname),"%s", httphost);
        p=hostname;
        while(*p != '\0'){
                if(*p == '['){
                        p++;
                        square_num--;
                }else if( *p == ':'){
                        p++;
                        colon_num++;
                }else if( *p == ']'){
                        p++;
                        square_op_num--;
                        break;
                }else{
                        p++;
                }
        }
        if(!square_num&&!square_op_num&&colon_num){
                return 1;
        }
        return 0;
}

int IsSameSubnet(char *lanip, char *lansubnet,char *wanip, char *wansubnet)
{
        unsigned int aip, bip, anet, bnet;

        aip = inet_addr(lanip);
        bip = inet_addr(wanip);

        anet = inet_addr(lansubnet);
        bnet = inet_addr(wansubnet);

        return (aip & anet) == (bip & anet) ||(aip & bnet) == (bip & bnet);
}

char *cat_file(char *name)
{
	int i;
	FILE *fp;
	static char buf[512];

	buf[0] = '\0';

	fp = fopen(name, "r");
	if (fp == NULL)
		return buf;
	fgets(buf, sizeof(buf), fp);
	fclose(fp);

	i = 0;
	while (buf[i] != '\0' && buf[i] != '\r' && buf[i] != '\n')
		i++;
	buf[i] = '\0';

	return buf;
}

void echo_set(char *value, char *file)
{
	FILE *fp;

	fp = fopen(file, "w");
	if (fp == NULL)
		return;
	fputs(value, fp);
	fclose(fp);
}


void arp_mac(char *ipaddr, char *mac, char *dev)
{
	int i;
	FILE *fp;
	char *p, line[256];
	int get_mac=0;

	*mac = *dev = '\0';

	/* ipv4 */
	fp = fopen("/proc/net/arp", "r");
	if (fp == NULL)
		goto ipv6;

	/* discard the one header lines */
	fgets(line, sizeof(line), fp);
	while (fgets(line, sizeof(line), fp))
	{
		p = line; i = 0;
		while(*p == ' ') p++;
		while(ipaddr[i] == *p) { p++; i++; }
		if (ipaddr[i] ||*p != ' ')
			continue;

		/* skip 2 columns */
		for (i = 2; i > 0 && *p != '\0'; i--)
		{
			while(*p == ' ') p++;
			while(*p != ' ' && *p) p++;
		}

		while(*p == ' ') p++;
		while(*p != ' ' && *p) *mac++ = *p++;
		*mac = '\0';

		while(*p == ' ' ||*p == '*') p++;
		while(*p != '\n' && *p) *dev++ = *p++;
		*dev = '\0';

		get_mac = 1;
		break;
	}
	fclose(fp);

	if( get_mac == 1)
		return;
ipv6:
	/* ipv6 */
	fp = popen("ip -6 neigh show", "r");
	if( fp == NULL )
		return;


	while (fgets(line, sizeof(line), fp))
	{
		p = line; i = 0;
		while(*p == ' ') p++;
		while(ipaddr[i] == *p) { p++; i++; }
		if (ipaddr[i] ||*p != ' ')
			continue;

		/* skip 1 columns */
		while(*p == ' ') p++;
		while(*p != ' ' && *p) p++;

		/* get dev */
		while(*p == ' ') p++;
		while(*p != ' ' && *p) *dev++ = *p++;
		*dev = '\0';

		/* skip 1 columns */
		while(*p == ' ') p++;
		while(*p != ' ' && *p) p++;

		/* get mac */
		while(*p == ' ') p++;
		while(*p != ' ' && *p) *mac++ = *p++;
		*mac = '\0';

		break;
	}

	pclose(fp);

}

#define LAN_IFNAME	"br0"

char *login_type(char *dev, int *local)
{
	if (strcmp(dev, LAN_IFNAME) == 0)
	{
		*local = 1;
		return "admin";
	}
	else
	{
		*local = 0;
		return "remote";
	}
}

static int rdfile(char *file, void *buf, int size)
{
	int fd, ret;

	fd = open(file, O_RDONLY, 0666);
	if (fd < 0)
		return 0;

	if (read(fd, buf,  size) != size)
		ret = 0;
	else
		ret = 1;
	close(fd);

	return ret;
}

/* read a word/integer non-zero value from `file'. */
int readw(char *file)
{
	int value;

	if (!rdfile(file, &value, sizeof(value)))
		value = 0;

	return value;
}

/* write a word/integer value to `file'. */
int writew(char *file, int value)
{
	int fd;
	int ret;

	fd = open(file, O_WRONLY |O_CREAT |O_TRUNC, 0666);
	if (fd < 0)
		return 0;

	ret = write(fd, &value, sizeof(int)) == sizeof(int);
	close(fd);

	return ret;
}

#define NO_NEEDAUTH	"/tmp/AUTH_no_need_auth"
#define LOGIN_TIME	"/tmp/AUTH_login_time"
#define LOGIN_IP	"/tmp/AUTH_login_ip"
#define LOGIN_DEV	"/tmp/AUTH_login_dev"
#define LOGIN_MAC	"/tmp/AUTH_login_mac"
#define LOGIN_GUEST     "/tmp/AUTH_login_guest"
#define LOGIN_TIMEOUT	300 /* 5 min */
#define GUEST_NUM       8

int update_login(struct client *cl)
{
	int ret, save, local;
	struct sysinfo info;
	char mac[32], dev[32];
	char from[32], *login_ip, *login_mac;
	char time[128];
	long last_time, login_time=0;

	sprintf(from,"%s",sa_straddr(&cl->peeraddr));
	save = 0;
	sysinfo(&info);
	last_time = atol(cat_file(LOGIN_TIME));

	login_ip = cat_file(LOGIN_IP);

	if ( *login_ip == '\0')
	{
		ret =AUTH_OK;
		save = 1;
		arp_mac(from, mac, dev);
		echo_set( from, LOGIN_IP );
		login_time = info.uptime;
		syslog(6, "[%s login] from source %s,",
				login_type(dev, &local), from);
	}
	else if (strcmp(login_ip, from) == 0 )
	{
		if ((info.uptime - last_time) > LOGIN_TIMEOUT)
		{
			ret = AUTH_TIMEOUT;
			echo_set( "", LOGIN_IP );
		}
		else
		{
			ret = AUTH_OK;
			login_time = info.uptime;
		}
	}
	else if ((info.uptime - last_time) > LOGIN_TIMEOUT)
	{
		ret = AUTH_TIMEOUT;
		echo_set( "", LOGIN_IP );
	}
	else
	{
		arp_mac(from, mac, dev);
		login_mac = cat_file(LOGIN_MAC);
		if ( *login_mac != '\0' && strcmp(login_mac, mac) == 0)
		{
			ret = AUTH_OK;
			echo_set( from, LOGIN_IP );
			login_time = info.uptime;
			syslog(6, "[%s login] from source %s,",
					login_type(dev, &local), from);
		}
		else
		{
			ret = AUTH_MULTI;
			/*To fix bug 31078 [log]there is a strange log message
			 *                          *Filter the ipv6 unknown addr "::" */
			if ( strcmp(from, "::") != 0 )
			{
				syslog(6, "[%s login failure] from source %s,",
						login_type(dev, &local), from);
			}
		}
	}

	if (save == 1)
	{
		echo_set(dev, LOGIN_DEV);
		echo_set(mac, LOGIN_MAC);
		if (mac[0] != '\0' && local)
		{
			config_set("wan_remote_mac", mac);
			if(config_match("quick_fastlane_dev", ""))
				config_set("quick_fastlane_dev", mac);
		}
	}

	if(login_time != 0)
	{
		sprintf(time, "%ld", login_time);
		echo_set(time, LOGIN_TIME);
	}

	return ret;
}

int update_login_guest(struct client *cl)
{
	int ret, local;
	struct sysinfo info;
	char mac[32], dev[32];
	char *login_ip, *login_mac, from[32];
	long last_time, login_time=0;
	struct auth_login auth_logins[GUEST_NUM];
	FILE *fp;
	int i;
	char *username;
	char uname[64];
	char authinfo[128];

	sprintf(from,"%s",sa_straddr(&cl->peeraddr));
	sysinfo(&info);
	username = config_get("http_guestname");
	strcpy(uname, username);
	config_set("http_loginname", uname);

	fp = fopen(LOGIN_GUEST, "r+");
	if(fp == NULL){
		for(i=0; i<GUEST_NUM; i++){
			strcpy(auth_logins[i].login_ip, "null");
			strcpy(auth_logins[i].login_mac, "null");
			strcpy(auth_logins[i].login_dev, "null");
			auth_logins[i].login_time = 0;
		}
		fp = fopen(LOGIN_GUEST, "w+");
		if(fp == NULL){
			return 0;
		}
		fwrite(&auth_logins[0], sizeof(struct auth_login), GUEST_NUM, fp);
	}
	rewind(fp);
	fread(&auth_logins[0], sizeof(struct auth_login), GUEST_NUM, fp);
	for(i=0; i<GUEST_NUM; i++)
	{
		login_ip = auth_logins[i].login_ip;
		if(strcmp(login_ip, "null") != 0 && strcmp(login_ip, from) == 0){
			last_time = auth_logins[i].login_time;
			if ((info.uptime - last_time) > LOGIN_TIMEOUT) {
				ret = AUTH_TIMEOUT;
				strcpy(auth_logins[i].login_ip, "null");
			} else {
				auth_logins[i].login_time = info.uptime;
				ret = AUTH_OK;
			}
			rewind(fp);
			fwrite(&auth_logins[0], sizeof(struct auth_login), GUEST_NUM, fp);
			fclose(fp);
			return ret;
		}
	}
	for(i=0; i<GUEST_NUM; i++)
	{
		login_ip = auth_logins[i].login_ip;
		if( strcmp(login_ip, "null") == 0 ){
			ret = AUTH_OK;
			arp_mac(from, mac, dev);
			strcpy(auth_logins[i].login_ip, from);
			strcpy(auth_logins[i].login_dev, dev);
			strcpy(auth_logins[i].login_mac, mac);
			auth_logins[i].login_time = info.uptime;
			rewind(fp);
			fwrite(&auth_logins[0], sizeof(struct auth_login), GUEST_NUM, fp);
			fclose(fp);
			//syslog(6, "[%s login] from source %s,",
			//		login_type(dev, &local), from);
			return ret;
		}
	}
	for(i=0; i<GUEST_NUM; i++)
	{
		last_time = auth_logins[i].login_time;
		if((info.uptime - last_time) > LOGIN_TIMEOUT){
			ret = AUTH_TIMEOUT;
			strcpy(auth_logins[i].login_ip, "null");
			rewind(fp);
			fwrite(&auth_logins[0], sizeof(struct auth_login), GUEST_NUM, fp);
			fclose(fp);
			return ret;
		}
	}
	for(i=0; i<GUEST_NUM; i++)
	{
		arp_mac(from, mac, dev);

		login_mac = auth_logins[i].login_mac;
		if (strcmp(login_mac, "null") != 0 && strcmp(login_mac,mac) == 0) {
			ret = AUTH_OK;
			strcpy(auth_logins[i].login_ip, from);
			auth_logins[i].login_time = info.uptime;
			rewind(fp);
			fwrite(&auth_logins[0], sizeof(struct auth_login), GUEST_NUM, fp);
			fclose(fp);
			//syslog(6, "[%s login] from source %s,",
			//		login_type(dev, &local), from);

		} else {
			ret = AUTH_MULTI_GUEST;
			fclose(fp);
			if ( strcmp(from, "::") != 0 ){
				//syslog(6, "[%s login failure] from source %s,",
				//		login_type(dev, &local), from);
			}
		}
		return ret;
	}
}
