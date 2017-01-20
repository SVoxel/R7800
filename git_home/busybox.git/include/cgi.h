#ifndef CGI_H
#define CGI_H

typedef enum { FORM_VAR, COOKIE_VAR } vartype;
typedef void (*var_handler) (vartype, char *, char *);
int cgi_init(var_handler);

#endif
