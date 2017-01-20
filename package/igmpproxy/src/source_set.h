#ifndef __SOURCE_SET__
#define __SOURCE_SET__

typedef unsigned long UINT32;

#define MAX_SOURCE_ENTRY 8
struct source_set
{
	int num;
	UINT32 list[MAX_SOURCE_ENTRY];
};

//#define DPRINTF(fmt,arg...) 

void set_assign(struct source_set *A, struct source_set *B, struct source_set *res, int inverse);
void set_add(struct source_set *A, struct source_set *B, struct source_set *res, int inverse);
void set_subtract(struct source_set *A, struct source_set *B, struct source_set *res, int inverse);
void set_intersection(struct source_set *A, struct source_set *B, struct source_set *res, int inverse);
int set_comp(struct source_set *A, struct source_set *B);
void set_test();

#endif

