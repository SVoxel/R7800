%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "busybox.h"
#include "awx_parser.h"

enum {
	S_INIT,
	S_TEXT,
	S_CODE,
	S_IF_START,
	S_FOR_START,
	S_FOR_IN,
	S_END,
	S_ELSE,
	S_EOF
}; 
int state;

#undef DEBUG
#ifdef DEBUG
char *statestr[] = {
	[S_INIT] = "S_INIT",
	[S_TEXT] = "S_TEXT",
	[S_CODE] = "S_CODE",
	[S_IF_START] = "S_IF_START",
	[S_FOR_START] = "S_FOR_START",
	[S_FOR_IN] = "S_FOR_IN",
	[S_EOF] = "S_EOF"
};

char *typestr[] = {
	[T_TEXT] = "T_TEXT",
	[T_FOR] = "T_FOR",
	[T_IF] = "T_IF",
	[T_CODE] = "T_CODE"
};
#endif

static struct template_cb *parse_cb;
static struct template_element *cur, *head;
static char *textbuf;
static unsigned int buflen;
static unsigned int buf_offset;
static int _lnr = 0;

static void buf_realloc(void)
{
	buflen *= 2;
	textbuf = xrealloc(textbuf, buflen);
}

static void parse_error(char *str)
{
	fprintf(stderr, "Parse error%s%s\n", (str ? ": " : "!"), (str ?: ""));
	exit(255);
}


static struct template_element *new_template_element(struct template_element *parent)
{
	struct template_element *ptr;
	
	ptr = xzalloc(sizeof(struct template_element));
	ptr->parent = parent;
	return ptr;
}

static inline void next_template_element(void)
{
	cur->next = new_template_element(cur->parent);
	cur->next->prev = cur;
	cur = cur->next;
}

static void addtext(char *text)
{
	while(buf_offset + strlen(text) + 1 > buflen)
		buf_realloc();

	buf_offset += sprintf(&textbuf[buf_offset], "%s", text);
}

static void set_state(int newstate)
{
	char *ptr;

#ifdef DEBUG
	static int _rec = 0;
	fprintf(stderr, "DEBUG(%d): %s => %s: %s\n", _rec, statestr[state], statestr[newstate], textbuf);
#endif
	ptr = xstrdup(textbuf);
	if (state == S_FOR_IN)
		cur->in = ptr;
	else
		cur->var = ptr;

	if (parse_cb && (cur->t == T_CODE) && parse_cb->prepare_code)
		parse_cb->prepare_code(cur);

	buf_offset = 0;
	*textbuf = 0;

	switch(newstate) {
#if 0
		case S_EOF:
			if (cur->parent)
				parse_error();
			break;
#endif
		case S_FOR_START:
			if (ptr || !cur->prev)
				next_template_element();
			cur->t = T_FOR;
			break;
		case S_IF_START:
			if (ptr || !cur->prev)
				next_template_element();
			cur->t = T_IF;
			break;
		case S_ELSE:
			cur = cur->parent;
			if (!cur)
				parse_error("'@else' without parent element");
			cur->sub2 = new_template_element(cur);
			cur = cur->sub2;
			newstate = S_TEXT;
			break;
		case S_END:
#ifdef DEBUG
			_rec--;
#endif
			cur = cur->parent;
			if (!cur) 
				parse_error("'@end' without parent element");

			next_template_element();
			cur->t = T_TEXT;
			newstate = S_TEXT;
			break;
		case S_TEXT:
			switch (cur->t) {
				case T_CODE:
					next_template_element();
					break;
				case T_IF:
				case T_FOR:
#ifdef DEBUG
					_rec++;
#endif
					cur->sub = new_template_element(cur);
					cur = cur->sub;
					break;
				default:
					break;
			}
			cur->t = T_TEXT;
			break;
		case S_CODE:
			if (ptr || !cur->prev)
				next_template_element();
			cur->t = T_CODE;
			break;
		default:
			break;
	}
	cur->line = _lnr;
	state = newstate;
}

%}

%%
"<%"[ \n\t]*"@if"[ \n\t]+ {
	if (state == S_TEXT) 
		set_state(S_IF_START);
	else
		REJECT;
}

"<%"[ \n\t]*"@for"[ \n\t]+ {
	if (state == S_TEXT) 
		set_state(S_FOR_START);
	else
		REJECT;
}

[ \n\t]+"in"[ \n\t]+ {
	if (state == S_FOR_START)
		set_state(S_FOR_IN);
	else
		REJECT;
}

"<%"[ \n\t]*"@end"[ \n\t]*%> {
	if (state != S_TEXT)
		REJECT;
	set_state(S_END);
}

"<%"[ \n\t]*"@else"[ \n\t]*%> {
	if (state != S_TEXT)
		REJECT;
	set_state(S_ELSE);
}

"<%" {
	if (state != S_TEXT) 
		parse_error("'<%' cannot be nested");
	set_state(S_CODE);
}

[ \n\t]"%>" {
	if (state == S_TEXT)
		REJECT;
	set_state(S_TEXT);
}

\n {
	_lnr++;
	if (state == S_TEXT)
		addtext(yytext);
}
.	{
	addtext(yytext);
}


%%


void execute_template(struct template_cb *cb, struct template_element *e)
{
	static int rec = 0;

	while (e) {
#ifdef DEBUG
		fprintf(stderr, "DEBUG: execute(%d)\t%s\n", rec, typestr[e->t]);
#endif
		rec++;
		if (cb->handle_element)
			cb->handle_element(cb, e);
		rec--;
		e = e->next;
	}
}

int yywrap()
{
	set_state(S_EOF);
	return 1;
}

struct template_element *parse_template(struct template_cb *cb, FILE *in)
{
	_lnr = 1;
	buf_offset = 0;
	state = S_TEXT;
	parse_cb = cb;
	
	buflen = 4096;
	textbuf = xzalloc(buflen);

	head = xzalloc(sizeof(struct template_element));
	head->t = T_TEXT;
	cur = head;

	yyin = in;
	yylex();

	return head;
}

void free_template(struct template_cb *cb, struct template_element *e)
{
	struct template_element *next;
	return;
	if (!e)
		return;
		
	switch (e->t) {
		case T_CODE:
			if (cb->free_code)
				cb->free_code(e);
			break;
		case T_FOR:
		case T_IF:
			free_template(cb, e->sub);
			break;
		default:
			break;
	}
	if (e->var)
		free(e->var);
	if (e->in)
		free(e->in);
		
	next = e->next;
	free(e);
	return free_template(cb, next);
}
