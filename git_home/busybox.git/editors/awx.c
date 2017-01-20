/*
 * awk web extension
 *
 * Copyright (C) 2007 by Felix Fietkau <nbd@openwrt.org>
 *
 * Licensed under the GPL v2 or later, see the file LICENSE in this tarball.
 */

#include <cgi.h>
#include <glob.h>
#include "awx_parser.h"

#define LINE_BUF 2048
#define HASH_MAX	1536
#define TR_START	"@TR<<"
#define TR_END		">>"
#define MAX_TR	32

#undef fputs

static xhash *lstr = NULL;
static xhash *formvar = NULL;
static int lang_inuse = 0;

/* look up a translation symbol from the hash */
static inline char *translate_lookup(char *str)
{
	char *name, *def, *p;
	hash_item *hi;
	var *v;

	def = name = str;
	if (((p = strchr(str, '|')) != NULL)
		|| ((p = strchr(str, '#')) != NULL)) {
		def = p + 1;
		*p = 0;
	}
	
	hi = hash_search(lstr, name);
	if (!hi)
		return def;
	
	v = &hi->data.v;

	return getvar_s(v);
}

/* look for translation markers in the line and return the translated string */
static char *translate_line(char *line)
{
	char *tok[MAX_TR * 3];
	char *l, *p, *p2 = NULL, *res;
	int len = 0, _pos = 0, i, tr_abort = 0;
	static char *backlog = NULL;

	if (backlog && line) {
		backlog = xrealloc(backlog, strlen(backlog) + strlen(line) + 1);
		sprintf(backlog + strlen(backlog), line);
		l = backlog;
	} else {
		l = line;
	}

	while (l != NULL) {
		if ((p = strstr(l, TR_START)) == NULL) {
			len += strlen((tok[_pos++] = l));
			break;
		}

		p2 = strstr(p, TR_END);
		if (p2 == NULL) {
			p2 = xstrdup(l);
			tr_abort = 1;
			break;
		}

		*p = 0;
		len += strlen((tok[_pos++] = l));
		*p2 = 0;
		len += strlen((tok[_pos++] = translate_lookup(p + strlen(TR_START))));

		l = p2;
		l += strlen(TR_END);
	}
	len++;

	p = xmalloc(len + 1);
	*p = 0;
	res = p;
	for (i = 0; i < _pos; i++) {
		strcat(p, tok[i]);
		p += strlen(tok[i]);
	}
	if (backlog) {
		free(backlog);
		backlog = NULL;
	}
	if (tr_abort && p2)
		backlog = p2;
	
	return res;
}

/* hook for intercepting awk's use of puts. used for running all printed strings
 * through the translation system */
static inline int fputs_hook (__const char *__restrict __s, FILE *__restrict __stream)
{
	if (lang_inuse && (__stream == stdout)) {
		int ret;
		char *str;
	
		str = translate_line((char *) __s);
		ret = fputs(str, __stream);
		free(str);

		return ret;
	}

	return fputs(__s, __stream);
}

static var *init_lang(var *res, var *args, int nargs)
{
	if (!lstr)
		lstr = hash_init();

	lang_inuse = 1;
	return res;
}


/* load and parse language file */
static void load_lang_file(char *file)
{
	FILE *f;
	char *b, *name, *value;
	char buf1[LINE_BUF];

	if ((f = fopen(file, "r")) == NULL)
		return;

	while (!feof(f) && (fgets(buf1, LINE_BUF - 1, f) != NULL)) {
		b = buf1;
		if (*b == '#')
			continue; /* skip comments */

		while (isspace(*b))
			b++; /* skip leading spaces */
		if (!*b)
			continue;
		
		name = b;
		if ((b = strstr(name, "=>")) == NULL)
			continue; /* separator not found */

		value = b + 2;
		if (!*value)
			continue;
		
		*b = 0;
		for (b--; isspace(*b); b--)
			*b = 0; /* remove trailing spaces */
		
		while (isspace(*value))
			value++; /* skip leading spaces */

		for (b = value + strlen(value) - 1; isspace(*b); b--)
			*b = 0; /* remove trailing spaces */
		
		if (!*value)
			continue;

		setvar_s(findvar(lstr,name), value);
	}

	fclose(f);
}

static var *load_lang(var *res, var *args, int nargs)
{
	const char *langfmt = "/usr/lib/webif/lang/%s.txt";
	char lbuf[LINE_BUF];
	char *lang;

	if (!lang_inuse)
		init_lang(res, args, nargs);
	
	lang = getvar_s(args);
	if (!lang || !strcmp(lang, ""))
		return res;

	sprintf(lbuf, langfmt, lang);
	load_lang_file(lbuf);

	return res;	
}
		
/* read the contents of an entire file */
static char *get_file(char *fname)
{
	FILE *F;
	char *s = NULL;
	int i, j, flen;

	F = fopen(fname, "r");
	if (!F) {
		return NULL;
	}

	if (fseek(F, 0, SEEK_END) == 0) {
		flen = ftell(F);
		s = (char *)xmalloc(flen+4);
		fseek(F, 0, SEEK_SET);
		i = 1 + fread(s+1, 1, flen, F);
	} else {
		for (i=j=1; j>0; i+=j) {
			s = (char *)xrealloc(s, i+4096);
			j = fread(s+i, 1, 4094, F);
		}
	}

	s[i] = '\0';
	fclose(F);
	return s;
}


/* parse_include():
 * 
 * taken from parse_program from awk.c
 * END{} is not parsed here, and BEGIN{} is executed immediately
 */
static void parse_include(char *p)
{
	uint32_t tclass;
	chain *initseq = NULL;
	chain tmp;
	func *f;
	var *v, *tv;

	tv = nvalloc(1);
	memset(&tmp, 0, sizeof(tmp));
	pos = p;
	t.lineno = 1;
	while ((tclass = next_token(TC_EOF | TC_OPSEQ |
				TC_OPTERM | TC_BEGIN | TC_FUNCDECL)) != TC_EOF) {
		if (tclass & TC_OPTERM)
			continue;

		seq = &tmp;
		if (tclass & TC_BEGIN) {
			initseq = xzalloc(sizeof(chain));
			seq = initseq;
			chain_group();
		} else if (tclass & TC_FUNCDECL) {
			next_token(TC_FUNCTION);
			pos++;
			f = newfunc(t.string);
			f->type = AWKFUNC;
			f->x.body.first = NULL;
			f->nargs = 0;
			while (next_token(TC_VARIABLE | TC_SEQTERM) & TC_VARIABLE) {
				v = findvar(ahash, t.string);
				v->x.aidx = (f->nargs)++;

				if (next_token(TC_COMMA | TC_SEQTERM) & TC_SEQTERM)
					break;
			}
			seq = &(f->x.body);
			chain_group();
			clear_array(ahash);
		}
	}
	if (initseq && initseq->first)
		tv = evaluate(initseq->first, tv);
	nvfree(tv);
}


/* include an awk file and run its BEGIN{} section */
static xhash *includes = NULL;
static void include_file(char *filename)
{
	char *s;
	var *v;

	if (!includes)
		includes = hash_init();
	
	/* find out if the file has been included already */
	v = findvar(includes, filename);
	if (istrue(v))
		return;
	setvar_s(v, "1");

	/* read include file */
	s = get_file(filename);
	if (!s) {
		fprintf(stderr, "Could not open file.\n");
		return;
	}
	parse_include(s+1);
	free(s);
}

static var *include(var *res, var *args, int nargs)
{
	char *s;

	s = getvar_s(args);
	if (s && (strlen(s) > 0))
		include_file(s);

	return res;
}

/* parse an awk expression */
static var *parse_awk(char *str, var *tv)
{
	chain body;
	node *n;

	memset(&body, 0, sizeof(body));
	pos = str;
	seq = &body;
	
	/* end of expression, assume that there's going to be a free byte
	 * at the end of the string that can be used for the ')' */
	strcat(str + strlen(str), "}");
	n = parse_expr(TC_GRPTERM);
	if (!n)
		return NULL;

	return evaluate(n, tv);
}

static inline void print_translate(char *s)
{
	char *str = s;
	if (lang_inuse)
		str = translate_line(s);
	fputs(str, stdout);
	fflush(stdout);
	if (lang_inuse)
		free(str);
}

static void render_element(struct template_cb *tcb, struct template_element *e)
{
	var *v;
	char *s, *s2;
	int i;
	
	if (!e || !e->var)
		return;
	lineno = e->line;
	switch (e->t) {
		case T_TEXT:
			s = malloc(strlen(e->var) + 2);
			strcpy(s, e->var);
			print_translate(s);
			free(s);
			break;
		case T_CODE:
			s = malloc(strlen(e->var) + 2);
			strcpy(s, e->var);
			v = nvalloc(1);
			s2 = strdup(getvar_s(parse_awk(s, v)));
			nvfree(v);
			print_translate(s2);
			free(s);
			free(s2);
			break;
		case T_IF:
			s = malloc(strlen(e->var) + 2);
			strcpy(s, e->var);
			v = nvalloc(1);
			i = istrue(parse_awk(s, v));
			nvfree(v);
			free(s);

			if (i)
				execute_template(tcb, e->sub);
			else if (e->sub2)
				execute_template(tcb, e->sub2);
			break;
		case T_FOR: {
				v = newvar(e->var);
				hashwalk_init(v, iamarray(findvar(vhash, e->in)));
				while (hashwalk_next(v)) {
					execute_template(tcb, e->sub);
				}
				clrvar(v);
			}
			break;
		default:
			break;
	}
}

/* awk method render(), which opens a template file and processes all awk ssi calls */
static void render_file(char *filename)
{
	struct template_cb tcb;
	struct template_element *e;
	FILE *f;
	char *oldprg;
	int oldlnr;
	
	if (!filename)
		return;

	oldlnr = lineno;
	oldprg = programname;
	programname = filename;

	f = fopen(filename, "r");
	if (!f)
		return;
	
	memset(&tcb, 0, sizeof(tcb));
	tcb.handle_element = render_element;
	e = parse_template(&tcb, f);
	execute_template(&tcb, e);
	free_template(&tcb, e);
	fclose(f);
	programname = oldprg;
	lineno = oldlnr;
}

static var *render(var *res, var *args, int nargs)
{
	char *s;

	s = getvar_s(args);
	if (!s)
		return res;

	render_file(s);
	
	return res;
}
		
/* Call render, but only if this function hasn't been called already */
static int layout_rendered = 0;
static var *render_layout(var *res, var *args, int nargs)
{
	if (layout_rendered)
		return res;
	layout_rendered = 1;
	return render(res, args, nargs);
}

/* registers a global c function for the awk interpreter */
static void register_cfunc(char *name, awk_cfunc cfunc, int nargs)
{
	func *f;

	f = newfunc(name);
	f->type = CFUNC;
	f->x.cfunc = cfunc;
	f->nargs = nargs;
}

static void putvar(vartype type, char *name, char *value)
{
	if (type != FORM_VAR)
		return;

	setvar_u(findvar(formvar, name), value);
}

static char *cgi_getvar(char *name)
{
	if (!formvar) {
		formvar = hash_init();
		cgi_init(putvar);
	}

	if (!formvar || !name)
		return NULL;
	
	return getvar_s(findvar(formvar, name));
}

/* function call for accessing cgi form variables */
static var *getvar(var *res, var *args, int nargs)
{
	char *s;
	char *svar;

	s = getvar_s(args);
	if (!s)
		return res;
	
	svar = cgi_getvar(s);
	if (!svar)
		return res;

	setvar_u(res, svar);

	return res;
}

/* call an awk function without arguments by string reference */
static var *call(var *res, var *args, int nargs)
{
	char *s = getvar_s(args);
	func *f;

	if (!s)
		goto done;
	
	f = newfunc(s);
	if (f && f->type == AWKFUNC && f->x.body.first)
		return evaluate(f->x.body.first, res);

done:
	return res;
}


static int run_awxscript(char *name)
{
	var tv, *layout, *action;
	char *tmp, *s = NULL;

	zero_out_var(&tv);
	programname = name;

	/* read the main controller source */
	s = get_file(programname);
	if (!s) {
		fprintf(stderr, "Could not open file\n");
		return 1;
	}
	parse_program(s+1);
	free(s);


	/* set some defaults for ACTION and LAYOUT, which will have special meaning */
	layout = newvar("LAYOUT");
	setvar_s(layout, "views/layout.ahtml");

	/* run the BEGIN {} block */
	evaluate(beginseq.first, &tv);

	action = newvar("ACTION");
	if (!(strlen(getvar_s(action)) > 0)) {
		tmp = cgi_getvar("action");
		if (!tmp || (strlen(tmp) <= 0))
			tmp = strdup("default");

		setvar_p(action, tmp);
	}
	
	/* call the action (precedence: begin block override > cgi parameter > "default") */
	tmp = xmalloc(strlen(getvar_s(action)) + 7);
	sprintf(tmp, "handle_%s", getvar_s(action));
	setvar_s(action, tmp);
	call(&tv, action, 1);
	free(tmp);

	/* render the selected layout, will do nothing if render_layout has been called from awk */
	render_layout(&tv, layout, 1);

	return 0;
}


/* main awx processing function. called from awk_main() */
static int do_awx(int argc, char **argv)
{
	int ret = -1;
	var tv;
	int i, c;
	char **args = argv;
	
	zero_out_var(&tv);

	/* register awk C callbacks */
	register_cfunc("getvar", getvar, 1);
	register_cfunc("render", render, 1);
	register_cfunc("render_layout", render_layout, 1);
	register_cfunc("call", call, 1);
	register_cfunc("include", include, 1);
	register_cfunc("init_lang", init_lang, 1);
	register_cfunc("load_lang", load_lang, 1);

	if (!is_awx)
		return 0;

	/* fill in ARGV array */
	setvar_i(V[ARGC], argc + 1);
	setari_u(V[ARGV], 0, "awx");
	i = 0;
	while (*args)
		setari_u(V[ARGV], ++i, *args++);
	
	while((c = getopt(argc, argv, "i:f:")) != EOF) {
		switch(c) {
			case 'i':
				programname = optarg;
				include_file(optarg);
				break;
			case 'f':
				ret = 0;
				programname = optarg;
				render_file(optarg);
				goto done;
		}
	}
	argc -= optind;
	argv += optind;

	if (argc < 1) {
		fprintf(stderr, "Invalid argument.\n");
		goto done;
	}

	ret = run_awxscript(*argv);

done:
	exit(ret);
}

/* entry point for awx applet */
int awx_main(int argc, char **argv)
{
	is_awx = 1;
	return awk_main(argc, argv);
}

