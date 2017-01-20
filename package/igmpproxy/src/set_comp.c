#include "source_set.h"

void set_assign(struct source_set *A, struct source_set *B, struct source_set *res, int inverse)
{
	if (!A || !B)
		return;
	memcpy(res, B, sizeof (struct source_set));
}

void set_add(struct source_set *A, struct source_set *B, struct source_set *res, int inverse)
{
	int dup=0, res_num = 0;
	int i,j,k;
	int numA, numB;

	if (inverse)
	{
		struct source_set *tmp = A;
		A = B;
		B = tmp;
	}
	numA = (A->num > MAX_SOURCE_ENTRY)?0:A->num;
	numB = (B->num > MAX_SOURCE_ENTRY)?0:B->num;

	for( i=0; i< numA; i++)
              res->list[i] = A->list[i];
	res_num = numA;

	for( j=0; j< numB; j++)
	{  
		dup=0;
		if( res_num < MAX_SOURCE_ENTRY )
		{
			for(k=0; k< numA; k++)
			{
				if( B->list[j] == A->list[k])
					dup++; 
			}

			if(dup == 0)
			{
				res->list[res_num] = B->list[j];
				res_num++;	
			}
		}
		else
			break;

	}

	res->num = res_num;

}



void set_subtract(struct source_set *A, struct source_set *B, struct source_set *res, int inverse)
{
	int dup=0, res_num=0;
	int i,j;
	int numA, numB;

	if (inverse)
	{
		struct source_set *tmp = A;
		A = B;
		B = tmp;
	}
	numA = (A->num > MAX_SOURCE_ENTRY)?0:A->num;
	numB = (B->num > MAX_SOURCE_ENTRY)?0:B->num;

	for( i=0; i< numA; i++)
	{
                dup=0;
				
		  for( j=0; j< numB; j++)
                {
                          if( B->list[j] == A->list[i])
					dup++;	  	
                }
		  if( dup == 0)
		  {        
                         res->list[res_num] =  A->list[i];
			    res_num++;			 
		  }
			
	}   
       res->num = res_num;

}

void set_intersection(struct source_set *A, struct source_set *B, struct source_set *res, int inverse)
{
	int dup=0, res_num=0;
	int i,j;
	int numA = (A->num > MAX_SOURCE_ENTRY)?0:A->num;
	int numB = (B->num > MAX_SOURCE_ENTRY)?0:B->num;
	   
	for( i=0; i< numA; i++)
	{
	          dup=0;
			  
                 for( j=0; j< numB; j++)
                {
                          if( B->list[j] == A->list[i])
					dup++;	  	
                }
		  if( dup != 0)
		  {        
                         res->list[res_num] =  A->list[i];
			    res_num++;			 
		  }
			
	}   
       res->num = res_num;

}


int set_comp(struct source_set *A, struct source_set *B)
{
	int numA = (A->num > MAX_SOURCE_ENTRY)?0:A->num;
	int numB = (B->num > MAX_SOURCE_ENTRY)?0:B->num;
	int i, j, flag;

	if (numA != numB)
		return 1;

	for (i = 0; i < numA; i++)
	{
		flag = 1;
		for (j =0; j < numA; j++)
		{
			if (A->list[i] == B->list[j])
				flag = 0;
		}
		if (flag)
			return 1;
	}
	return 0;
}

#if (ENABLE_DEBUG)
void set_test()
{

DPRINTF("hehehe\n\n");


       struct source_set A,B,C;
	   int i=0;



       A.num=5;
	B.num=7;
	A.list[0]=0x00000001;
	A.list[1]=0x00000002;
	A.list[2]=0x00000003;
	A.list[3]=0x00000004;
	A.list[4]=0x00000007;
	B.list[0]=0x00000004;
	B.list[1]=0x00000005;
	B.list[2]=0x00000006;
	B.list[3]=0x00000001;
	B.list[4]=0x00000008;
	B.list[5]=0x00000009;
	B.list[6]=0x00000011;


       set_add(&A,&B,&C);
       for(i =0; i < C. num; i++)
       	{
		   DPRINTF("----------ADD---------C list[ %d] = %x\n", i,  C.list[i]);
       	}

     set_subtract(&A,&B,&C);
       for(i =0; i < C.num; i++)
       	{
		   DPRINTF("----------SUB---------C list[ %d] = %x\n", i,  C.list[i]);
       	}


     set_intersection(&A,&B,&C);
       for(i =0; i < C.num; i++)
       	{
		   DPRINTF("----------INT---------C list[ %d] = %x\n", i,  C.list[i]);
       	}


}
#endif

