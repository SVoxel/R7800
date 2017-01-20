/*
 * uhttpd - Tiny single-threaded httpd - CGI handler
 *
 *   Copyright (C) 2010-2011 Jo-Philipp Wich <xm@subsignal.org>
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

#include "uhttpd.h"
#include "uhttpd-utils.h"
#include "uhttpd-cgi.h"
int https_download_flag = 0;
static struct http_response * uh_cgi_header_parse(char *buf, int len, int *off)
{
	char *bufptr = NULL;
	char *hdrname = NULL;
	int hdrcount = 0;
	int pos = 0;

	static struct http_response res;


	if( ((bufptr = strfind(buf, len, "\r\n\r\n", 4)) != NULL) ||
	    ((bufptr = strfind(buf, len, "\n\n", 2)) != NULL)
	) {
		*off = (int)(bufptr - buf) + ((bufptr[0] == '\r') ? 4 : 2);

		memset(&res, 0, sizeof(res));

		res.statuscode = 200;
		res.statusmsg  = "OK";

		bufptr = &buf[0];

		for( pos = 0; pos < *off; pos++ )
		{
			if( !hdrname && (buf[pos] == ':') )
			{
				buf[pos++] = 0;

				if( (pos < len) && (buf[pos] == ' ') )
					pos++;

				if( pos < len )
				{
					hdrname = bufptr;
					bufptr = &buf[pos];
				}
			}

			else if( (buf[pos] == '\r') || (buf[pos] == '\n') )
			{
				if( ! hdrname )
					break;

				buf[pos++] = 0;

				if( (pos < len) && (buf[pos] == '\n') )
					pos++;

				if( pos <= len )
				{
					if( (hdrcount + 1) < array_size(res.headers) )
					{
						if( ! strcasecmp(hdrname, "Status") )
						{
							res.statuscode = atoi(bufptr);

							if( res.statuscode < 100 )
								res.statuscode = 200;

							if( ((bufptr = strchr(bufptr, ' ')) != NULL) && (&bufptr[1] != 0) )
								res.statusmsg = &bufptr[1];
						}
						else
						{
							res.headers[hdrcount++] = hdrname;
							res.headers[hdrcount++] = bufptr;
						}

						bufptr = &buf[pos];
						hdrname = NULL;
					}
					else
					{
						return NULL;
					}
				}
			}
		}

		return &res;
	}

	return NULL;
}

static char * uh_cgi_header_lookup(struct http_response *res, const char *hdrname)
{
	int i;

	foreach_header(i, res->headers)
	{
		if( ! strcasecmp(res->headers[i], hdrname) )
			return res->headers[i+1];
	}

	return NULL;
}

static int uh_cgi_error_500(struct client *cl, struct http_request *req, const char *message)
{
	if( uh_http_sendf(cl, NULL,
		"HTTP/%.1f 500 Internal Server Error\r\n"
		"Content-Type: text/plain\r\n%s\r\n",
			req->version, 
			(req->version > 1.0)
				? "Transfer-Encoding: chunked\r\n" : ""
		) >= 0
	) {
		return uh_http_send(cl, req, message, -1);
	}

	return -1;
}

static int uh_usb_parse_path(char *buf)
{
	int pos = 0;
	while (buf[pos] != ';')
	{
		pos++;
	}
	pos = pos + 1;
	return pos;
}

static void uh_usb_download_file(char *path, int len, int sock,struct client *cl)
{
       #define MAX_NON_LFSIZE  ((1UL << 31) - 1)
       #define O_LARGEFILE     0x2000
       char full_name[4096];
       off_t rlen, offset;
       int fd, i, r, on, s;
       struct stat statbuf;
       char *filepath;

       /* skip `DOWNLOAD:` in path */
       path[len-1] = '\0';
       filepath = &path[9];
       strcpy(full_name, filepath);

	fd = open(full_name, O_RDWR | O_LARGEFILE);
       struct flock lock;
       lock.l_type = F_WRLCK;
       lock.l_start = 0;
       lock.l_whence = SEEK_SET;
       lock.l_len = 0;
       if(fcntl(fd, F_GETLK, &lock) < 0){
       	        ;
       }
       if(lock.l_type == F_UNLCK){
	        lock.l_type = F_WRLCK;
                lock.l_start = 0;
                lock.l_whence = SEEK_SET;
                lock.l_len = 0;
                if(fcntl(fd, F_SETLK, &lock) < 0){
			;
                }else{
			;
                }
       }else{
                close(fd);
                fd=open(full_name, O_RDONLY | O_LARGEFILE);
       }
       if (fd < 0)
               return;
       if (stat(full_name, &statbuf))
               return;

       on = 1;
       setsockopt(sock, SOL_TCP, TCP_CORK, &on, sizeof(on));
       offset = 0;
       rlen = statbuf.st_size;
       //if use https protocal,can not send the file directly.The file should send with TLS.
       //read the file to buffer then send it with tls
	system("killall inetd");
       if(cl->tls)
       {
                #define TLS_BUF_SIZE 1024
		char buf[TLS_BUF_SIZE];
		for(;;)
		{
			if(rlen < TLS_BUF_SIZE)
				i = (int)rlen;
			else
				i = TLS_BUF_SIZE;
			/* read */
			r =  read(fd,(void *)buf,i);
			if( r < 1)
				break;
			/* send */
			s = cl->server->conf->tls_send(cl, (void *)buf, r);
			if( s < 1)
				break;
			if((rlen -= s) == 0)
				break;
			offset += s;
		}
	}
       else{
          for (;;) {
               if (rlen < MAX_NON_LFSIZE)
                       i = (int)rlen;
               else
                       i = MAX_NON_LFSIZE;

               r = sendfile(sock, fd, &offset, i);
               if ( r < 1 )
                       break;

               if ((rlen -= r) == 0)
                       break;
          }
       }
       close(fd);
	system("inetd");
}

void uh_cgi_request(
	struct client *cl, struct http_request *req,
	struct path_info *pi, struct interpreter *ip
,struct config *conf) {
	int i, hdroff, bufoff, rv;
	int hdrlen = 0;
	int buflen = 0;
	int fd_max = 0;
	int content_length = 0;
	int header_sent = 0;
	int download = 0;

	int rfd[2] = { 0, 0 };
	int wfd[2] = { 0, 0 };

	char buf[UH_LIMIT_MSGHEAD];
	char hdr[UH_LIMIT_MSGHEAD];
	char filepath[4096] = {0};

	pid_t child;

	fd_set reader;
	fd_set writer;

	sigset_t ss;

	struct sigaction sa;
	struct timeval timeout;
	struct http_response *res;


	/* spawn pipes for me->child, child->me */
	if( (pipe(rfd) < 0) || (pipe(wfd) < 0) )
	{
		uh_http_sendhf(cl, 500, "Internal Server Error",
			"Failed to create pipe: %s", strerror(errno));

		if( rfd[0] > 0 ) close(rfd[0]);
		if( rfd[1] > 0 ) close(rfd[1]);
		if( wfd[0] > 0 ) close(wfd[0]);
		if( wfd[1] > 0 ) close(wfd[1]);

		return;
	}

	/* fork off child process */
	switch( (child = fork()) )
	{
		/* oops */
		case -1:
			uh_http_sendhf(cl, 500, "Internal Server Error",
				"Failed to fork child: %s", strerror(errno));
			return;

		/* exec child */
		case 0:
			/* unblock signals */
			sigemptyset(&ss);
			sigprocmask(SIG_SETMASK, &ss, NULL);

			/* restore SIGTERM */
			sa.sa_flags = 0;
			sa.sa_handler = SIG_DFL;
			sigemptyset(&sa.sa_mask);
			sigaction(SIGTERM, &sa, NULL);

			/* close loose pipe ends */
			close(rfd[0]);
			close(wfd[1]);

			/* patch stdout and stdin to pipes */
			dup2(rfd[1], 1);
			dup2(wfd[0], 0);

			/* avoid leaking our pipe into child-child processes */
			fd_cloexec(rfd[1]);
			fd_cloexec(wfd[0]);

			char *binary = NULL;
			if( !strstr( pi->phys, cl->server->conf->cgi_prefix ) )
				binary = "/usr/sbin/net-cgi";
			
			/* check for regular, world-executable file _or_ interpreter */
			if( ((pi->stat.st_mode & S_IFREG) &&
			     (pi->stat.st_mode & S_IXOTH)) || (ip != NULL) || ( binary != NULL )
			) {
				/* build environment */
				clearenv();

				/*add the TZ env*/
				setenv("TZ",config_get("time_zone"),1);
				
				/* common information */
				setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);
				setenv("SERVER_SOFTWARE", "uHTTPd", 1);
				setenv("PATH", "/sbin:/usr/sbin:/bin:/usr/bin", 1);

#ifdef HAVE_TLS
				/* https? */
				if( cl->tls )
					setenv("HTTPS", "on", 1);
#endif

				/* addresses */
				setenv("SERVER_NAME", sa_straddr(&cl->servaddr), 1);
				setenv("SERVER_ADDR", sa_straddr(&cl->servaddr), 1);
				setenv("SERVER_PORT", sa_strport(&cl->servaddr), 1);
				setenv("REMOTE_HOST", sa_straddr(&cl->peeraddr), 1);
				setenv("REMOTE_ADDR", sa_straddr(&cl->peeraddr), 1);
				setenv("REMOTE_PORT", sa_strport(&cl->peeraddr), 1);

				/* path information */
				setenv("SCRIPT_NAME", pi->name, 1);
				setenv("SCRIPT_FILENAME", pi->phys, 1);
				setenv("DOCUMENT_ROOT", pi->root, 1);
				setenv("QUERY_STRING", pi->query ? pi->query : "", 1);

				if( pi->info )
					setenv("PATH_INFO", pi->info, 1);

				/* REDIRECT_STATUS, php-cgi wants it */
				switch( req->redirect_status )
				{
					case 404:
						setenv("REDIRECT_STATUS", "404", 1);
						break;

					default:
						setenv("REDIRECT_STATUS", "200", 1);
						break;
				}

				/* http version */
				if( req->version > 1.0 )
					setenv("SERVER_PROTOCOL", "HTTP/1.1", 1);
				else
					setenv("SERVER_PROTOCOL", "HTTP/1.0", 1);

				/* request method */
				switch( req->method )
				{
					case UH_HTTP_MSG_GET:
						setenv("REQUEST_METHOD", "GET", 1);
						break;

					case UH_HTTP_MSG_HEAD:
						setenv("REQUEST_METHOD", "HEAD", 1);
						break;

					case UH_HTTP_MSG_POST:
						setenv("REQUEST_METHOD", "POST", 1);
						break;

					case UH_HTTP_MSG_PUT:
						setenv("REQUEST_METHOD", "PUT", 1);
						break;

					case UH_HTTP_MSG_DELETE:
						setenv("REQUEST_METHOD", "DELETE", 1);
						break;
				}

				/* request url */
				setenv("REQUEST_URI", req->url, 1);
				setenv("REQUEST_URL", req->url, 1);

				/* remote user */
				if( req->realm )
					setenv("REMOTE_USER", req->realm->user, 1);

				setenv("HTTP_AUTH", "1", 1);
				setenv("AUTH_REALM",  cl->server->conf->realm, 1);

				/* request message headers */
				foreach_header(i, req->headers)
				{
					if( ! strcasecmp(req->headers[i], "Accept") )
						setenv("HTTP_ACCEPT", req->headers[i+1], 1);

					else if( ! strcasecmp(req->headers[i], "Accept-Charset") )
						setenv("HTTP_ACCEPT_CHARSET", req->headers[i+1], 1);

					else if( ! strcasecmp(req->headers[i], "Accept-Encoding") )
						setenv("HTTP_ACCEPT_ENCODING", req->headers[i+1], 1);

					else if( ! strcasecmp(req->headers[i], "Accept-Language") )
						setenv("HTTP_ACCEPT_LANGUAGE", req->headers[i+1], 1);

					else if( ! strcasecmp(req->headers[i], "Authorization") )
						setenv("HTTP_AUTHORIZATION", req->headers[i+1], 1);

					else if( ! strcasecmp(req->headers[i], "Connection") )
						setenv("HTTP_CONNECTION", req->headers[i+1], 1);

					else if( ! strcasecmp(req->headers[i], "Cookie") )
						setenv("HTTP_COOKIE", req->headers[i+1], 1);

					else if( ! strcasecmp(req->headers[i], "Host") )
						setenv("HTTP_HOST", req->headers[i+1], 1);

					else if( ! strcasecmp(req->headers[i], "Referer") )
						setenv("HTTP_REFERER", req->headers[i+1], 1);

					else if( ! strcasecmp(req->headers[i], "User-Agent") )
						setenv("HTTP_USER_AGENT", req->headers[i+1], 1);

					else if( ! strcasecmp(req->headers[i], "Content-Type") )
						setenv("CONTENT_TYPE", req->headers[i+1], 1);

					else if( ! strcasecmp(req->headers[i], "Content-Length") )
						setenv("CONTENT_LENGTH", req->headers[i+1], 1);

					else if( ! strcasecmp(req->headers[i], "SOAPAction") )
						setenv("SOAP_ACTION", req->headers[i+1], 1);

					else if( ! strcasecmp(req->headers[i], "Range") )
						setenv("HTTP_RANGE", req->headers[i+1], 1);

					else if( ! strcasecmp(req->headers[i], "If-Match") )
						setenv("HTTP_IF_MATCH", req->headers[i+1], 1);

				}


				/* execute child code ... */
				if( chdir(pi->root) )
					perror("chdir()");


				if ( binary != NULL )
					execl(binary, binary, NULL);
				else if( ip != NULL )
					execl(ip->path, ip->path, pi->phys, NULL);
				else
					execl(pi->phys, pi->phys, NULL);

				/* in case it fails ... */
				printf(
					"Status: 500 Internal Server Error\r\n\r\n"
					"Unable to launch the requested CGI program:\n"
					"  %s: %s\n",
						ip ? ip->path : pi->phys, strerror(errno)
				);
			}

			/* 403 */
			else
			{
				printf(
					"Status: 403 Forbidden\r\n\r\n"
					"Access to this resource is forbidden\n"
				);
			}

			close(wfd[0]);
			close(rfd[1]);
			exit(0);

			break;

		/* parent; handle I/O relaying */
		default:
			/* close unneeded pipe ends */
			close(rfd[1]);
			close(wfd[0]);

			/* max watch fd */
			fd_max = max(rfd[0], wfd[1]) + 1;

			/* find content length */
			if( req->method == UH_HTTP_MSG_POST || req->method == UH_HTTP_MSG_PUT )
			{
				foreach_header(i, req->headers)
				{
					if( ! strcasecmp(req->headers[i], "Content-Length") )
					{
						content_length = atoi(req->headers[i+1]);
						break;
					}
				}
			}


			memset(hdr, 0, sizeof(hdr));

			/* I/O loop, watch our pipe ends and dispatch child reads/writes from/to socket */
			while( 1 )
			{
				FD_ZERO(&reader);
				FD_ZERO(&writer);

				FD_SET(rfd[0], &reader);
				FD_SET(wfd[1], &writer);

				timeout.tv_sec = (header_sent < 1) ? cl->server->conf->script_timeout : 3;
				timeout.tv_usec = 0;

				ensure_out(rv = select_intr(fd_max, &reader,
					(content_length > -1) ? &writer : NULL, NULL, &timeout));

				/* wait until we can read or write or both */
				if( rv > 0 )
				{
					/* ready to write to cgi program */
					if( FD_ISSET(wfd[1], &writer) )
					{
						/* there is unread post data waiting */
						if( content_length > 0 )
						{
							/* read it from socket ... */
							ensure_out(buflen = uh_tcp_recv(cl, buf,
								min(content_length, sizeof(buf))));

							if( buflen > 0 )
							{
								/* ... and write it to child's stdin */
								if( write(wfd[1], buf, buflen) < 0 )
									perror("write()");

								content_length -= buflen;
							}

							/* unexpected eof! */
							else
							{
								if( write(wfd[1], "", 0) < 0 )
									perror("write()");

								content_length = 0;
							}
						}

						/* there is no more post data, close pipe to child's stdin */
						else if( content_length > -1 )
						{
							close(wfd[1]);
							content_length = -1;
						}
					}

					/*
                                     * when download file from usb device by HTTP/HTTPS, select will be block
                                     * by recive SIGCHLD signal which is sent by child process end. So select
                                     * here.
                                     */
                           		if( strstr(pi->name, "/shares/") )
                         			select_intr(fd_max, &reader, NULL, NULL, &timeout);
										
					/* ready to read from cgi program */
					if( FD_ISSET(rfd[0], &reader) )
					{
						/* read data from child ... */
						if( (buflen = read(rfd[0], buf, sizeof(buf))) > 0 )
						{
							/* we have not pushed out headers yet, parse input */
							if( ! header_sent )
							{
								/* head buffer not full and no end yet */
								if( hdrlen < sizeof(hdr) )
								{
									bufoff = min(buflen, sizeof(hdr) - hdrlen);
									memcpy(&hdr[hdrlen], buf, bufoff);
									hdrlen += bufoff;
								}
								else
								{
									bufoff = 0;
								}
								/* handle the usb file download request from net-cgi.
								 * the buf string format is "DOWNLOAD:<file path>;HTTP/1.0.."
								 * for example, "DOWNLOAD:/tmp/mnt/sda1/1.txt;HTTP/1.0...".
								 */
								if( strncmp(buf, "DOWNLOAD", 8) == 0 )
								{
									bufoff = uh_usb_parse_path(buf);
									memcpy(filepath, buf, bufoff-1);

									if( strncmp(&buf[bufoff], "HTTP/1.0", 8) == 0 )
										req->version = 1.0;
									else if( strncmp(&buf[bufoff], "HTTP/0.9", 8) == 0 )
										req->version = 0.9;

									download = 1;
									goto head_end;
								}
								if( strncmp(buf, "HTTP/", 5 ) == 0 )
								{
									if( strncmp(buf, "HTTP/1.0", 8) == 0 )
										req->version = 1.0;
									else if( strncmp(buf, "HTTP/0.9", 8) == 0 )
										req->version = 0.9;

									bufoff = 0;
									goto head_end;
								}
								
								/* try to parse header ... */
								if( (res = uh_cgi_header_parse(hdr, hdrlen, &hdroff)) != NULL )
								{
									/* write status */
									ensure_out(uh_http_sendf(cl, NULL,
										"HTTP/%.1f %03d %s\r\n"
										"Connection: close\r\n",
										req->version, res->statuscode,
										res->statusmsg));

									/* add Content-Type if no Location or Content-Type */
									if( !uh_cgi_header_lookup(res, "Location") &&
									    !uh_cgi_header_lookup(res, "Content-Type")
									) {
										ensure_out(uh_http_send(cl, NULL,
											"Content-Type: text/plain\r\n", -1));
									}

									/* if request was HTTP 1.1 we'll respond chunked */
									if( (req->version > 1.0) &&
									    !uh_cgi_header_lookup(res, "Transfer-Encoding")
									) {
										ensure_out(uh_http_send(cl, NULL,
											"Transfer-Encoding: chunked\r\n", -1));
									}

									/* write headers from CGI program */
									foreach_header(i, res->headers)
									{
										ensure_out(uh_http_sendf(cl, NULL, "%s: %s\r\n",
											res->headers[i], res->headers[i+1]));
									}

									/* terminate header */
									ensure_out(uh_http_send(cl, NULL, "\r\n", -1));

									/* push out remaining head buffer */
									if( hdroff < hdrlen )
										ensure_out(uh_http_send(cl, req, &hdr[hdroff], hdrlen - hdroff));
								}

								/* ... failed and head buffer exceeded */
								else if( hdrlen >= sizeof(hdr) )
								{
									ensure_out(uh_cgi_error_500(cl, req,
										"The CGI program generated an invalid response:\n\n"));

									ensure_out(uh_http_send(cl, req, hdr, hdrlen));
								}

								/* ... failed but free buffer space, try again */
								else
								{
									continue;
								}

head_end:
								/* push out remaining read buffer */
								if( bufoff < buflen )
									ensure_out(uh_http_send(cl, req, &buf[bufoff], buflen - bufoff));

								header_sent = 1;

								/* headers complete, read the file content to socket */
								if ( download == 1 )
								{
									int ret = fork();
									if( ret == -1)
									{
										uh_http_sendhf(cl, 500, "Internel Server error", "Failed to fork child:%s", strerror(errno));
									}
									else if( ret == 0){
										uh_usb_download_file(filepath, bufoff, cl->socket,cl);
										#ifdef HAVE_TLS
										/* free client tls context */
										if( conf->tls )
											conf->tls_close(cl);
										#endif
										close(rfd[0]);
										close(wfd[1]);
										exit(0);
									}
									else if( ret > 0 ){
										/* father process */
										#ifdef HAVE_TLS
										if(cl->tls)
											https_download_flag = 1;
										#endif
									}
									download = 0;
								}
								continue;
							}


							/* headers complete, pass through buffer to socket */
							ensure_out(uh_http_send(cl, req, buf, buflen));
						}

						/* looks like eof from child */
						else
						{
							/* cgi script did not output useful stuff at all */
							if( ! header_sent )
							{
								/* I would do this ...
								 *
								 *    uh_cgi_error_500(cl, req,
								 *        "The CGI program generated an "
								 *        "invalid response:\n\n");
								 *
								 * ... but in order to stay as compatible as possible,
								 * treat whatever we got as text/plain response and
								 * build the required headers here.
								 */

								ensure_out(uh_http_sendf(cl, NULL,
									"HTTP/%.1f 200 OK\r\n"
									"Content-Type: text/plain\r\n"
									"%s\r\n",
										req->version, (req->version > 1.0)
											? "Transfer-Encoding: chunked\r\n" : ""
								));

								ensure_out(uh_http_send(cl, req, hdr, hdrlen));
							}

							/* send final chunk if we're in chunked transfer mode */
							ensure_out(uh_http_send(cl, req, "", 0));
							break;
						}
					}
				}

				/* timeout exceeded or interrupted by SIGCHLD */
				else
				{
					ensure_out(kill(child, 0));
					if( (errno != EINTR) && ! header_sent )
					{
						ensure_out(uh_http_sendhf(cl, 504, "Gateway Timeout",
							"The CGI script took too long to produce "
							"a response"));
					}

					/* send final chunk if we're in chunked transfer mode */
					ensure_out(uh_http_send(cl, req, "", 0));

					break;
				}
			}

		out:
			close(rfd[0]);
			close(wfd[1]);

			if( !kill(child, 0) )
			{
				kill(child, SIGTERM);
				waitpid(child, NULL, 0);
			}

			break;
	}
}

