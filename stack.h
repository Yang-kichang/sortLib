#include<stdio.h>

typedef int TYPE;
typedef struct list{
	TYPE data;
	struct list *next;
	struct list *prev;
}STK;

STK *head=NULL;
STK *tail=NULL;

void stk_Push(TYPE a){
	STK *tmp = (STK *)calloc(sizeof(STK),1);
	tmp->data=a;

	if(head==NULL) head=tmp;
	else{
		tail->next=tmp;
		tmp->prev=tail;
		tail=tmp;
	}

}

void stk_Pop(TYPE a
		

