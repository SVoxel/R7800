#ifndef __TEMPLATE_PARSER_H
#define __TEMPLATE_PARSER_H

enum type {
	T_TEXT,
	T_FOR,
	T_IF,
	T_CODE
};

struct template_element;
struct template_cb;

struct template_cb {
	void *(*prepare_code)(struct template_element *);
	void (*handle_element)(struct template_cb *, struct template_element *);
	void (*free_code)(struct template_element *);
};

struct template_element {
	enum type t;
	char *var;
	char *in;
	int line;
	void *priv;
	struct template_element *parent;
	struct template_element *sub;
	struct template_element *sub2;
	struct template_element *prev;
	struct template_element *next;
};


struct template_element *parse_template(struct template_cb *cb, FILE *in);
void execute_template(struct template_cb *cb, struct template_element *e);
void free_template(struct template_cb *cb, struct template_element *e);

#endif
