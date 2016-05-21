#include<stdio.h>
#include<stdlib.h>
#include "TAD.h"

void Init_Stack(Stack** i_stack)
{
    Stack *aux = (Stack*)malloc(sizeof(Stack));

    aux->Next = NULL;
    aux->Back = NULL;
    aux->value = 0;
    *i_stack = aux;
    aux = NULL;
    free(aux);
}

void Push_Stack(Stack** r_stack, int x, int y)//here receive the adress to stack and value with you wish put in stack
{
    Stack *aux = (Stack*)malloc(sizeof(Stack));//allocates the memory for the new stack

    aux->value = x;
    aux->value_aux = y;
    aux->Next = NULL;
    aux->Back = *r_stack;

    *r_stack = aux;//pass the value from new stack to stack older
    aux = NULL;
    free(aux);
}

void Show_Stack(Stack* S_stack)
{
    while(S_stack->Back != NULL)
    {
        printf("\n%d, %d", S_stack->value, S_stack->value_aux);
        S_stack = S_stack->Back;
    }
}

int Pop_Stack(Stack* p_stack)
{
    if(!Void_Stack(p_stack)) return 0;//caso a pilha esteje vazia
    int value = p_stack->value;

    Stack *aux = p_stack->Back;

    aux->Next = NULL;
    p_stack->Back = NULL;
    *p_stack = *aux;
    aux = NULL;
    free(aux);
    return value;
}

void Pop_Stack_Double(Stack* p_stack, int* x, int* y)
{
    if(!Void_Stack(p_stack)) return;//caso a pilha esteje vazia
    *x = p_stack->value;
    *y = p_stack->value_aux;

    Stack *aux = p_stack->Back;

    aux->Next = NULL;
    p_stack->Back = NULL;
    *p_stack = *aux;
    aux = NULL;
    free(aux);
}

int Find_Stack(Stack* f_stack, int x, int y)
{
    if(f_stack->value == x && f_stack->value_aux == y)
        return 1;

    while(f_stack->Back != NULL)
    {
        f_stack = f_stack->Back;
        if(f_stack->value == x && f_stack->value_aux == y)
            return 1;
    }
    return 0;
}

int Void_Stack(Stack* v_stack)
{
    if(v_stack->Back == NULL)
        return 0;
    else return 1;
}

int Count_Stack(Stack* C_stack)
{
    int cont = 0;
    while(C_stack->Back != NULL)
    {
        cont++;
        C_stack = C_stack->Back;
    }
    return --cont;
}

void Init_Queue(Queue** i_queue)
{
    Queue *aux = (Queue*)malloc(sizeof(Queue));
    *i_queue = aux;
    aux->value = 0;
    aux->value_aux = 0;
    aux->Next = NULL;
    aux->Back = NULL;
    aux = NULL;
    free(aux);
}

void Push_Queue(Queue** p_queue, int x, int y)
{
    Queue *aux = (Queue*)malloc(sizeof(Queue));//allocates memory for the new queue

    aux->value = x;
    aux->value_aux = y;
    aux->Back = NULL;
    aux->Next = *p_queue;
    aux->Next->Back = aux;

    *p_queue = aux;// pass the value from new queue to queue older
    aux = NULL;//free to pointer
    free(aux);
}

int Pop_Queue(Queue* p_queue)
{
    Queue* p_aux;
    int x;

    if(p_queue->Next == NULL)
        return 0;

    while(p_queue->Next != NULL)
        p_queue = p_queue->Next;

    if(p_queue->Back != NULL)
    {
        p_aux = p_queue->Back;
        x = p_aux->value;
    }
    else return 0;

    if(p_aux->Back != NULL)
    {
        p_aux = p_aux->Back;
        p_aux->Next = p_queue;
        p_queue->Back = p_aux;
    }
    else
    {
        p_aux->Next = NULL;
        p_queue->Back = NULL;
    }

    p_aux=NULL;
    free(p_aux);

	return x;
}

void Pop_Queue_Double(Queue* p_queue, int *x, int *y)
{
    Queue* p_aux;

    if(p_queue->Next == NULL)
        return;

    while(p_queue->Next != NULL)
        p_queue = p_queue->Next;

    if(p_queue->Back != NULL)
    {
        p_aux = p_queue->Back;
        *x = p_aux->value;
        *y = p_aux->value_aux;
    }
    else return;

    if(p_aux->Back != NULL)
    {
        p_aux = p_aux->Back;
        p_aux->Next = p_queue;
        p_queue->Back = p_aux;
    }
    else
    {
        p_aux->Next = NULL;
        p_queue->Back = NULL;
    }

    p_aux=NULL;
    free(p_aux);
}

void Show_Queue(Queue* s_queue)
{
    while(s_queue->Next != NULL)
        s_queue = s_queue->Next;

    while(s_queue->Back != NULL)
    {
        s_queue = s_queue->Back;
        printf("(%d, %d)->", s_queue->value, s_queue->value_aux);
    }
    s_queue = NULL;
    free(s_queue);
}

int Find_Queue(Queue* f_queue, int value)
{
    while(value != f_queue->value)
	{
        if(f_queue->Next != NULL)
            f_queue = f_queue->Next;
        else
            return 0;
  	}
    return 1;
}

int Find_Queue_Double(Queue* f_queue, int value, int value_aux)
{
    while(f_queue->Next != NULL)
	{
        if( (f_queue->value == value) && (f_queue->value_aux == value_aux) )
            return 1;
        else
            f_queue = f_queue->Next;
    }
    return 0;
}

void Copy_Queue(Queue* queue_src, Queue** queue_dst)
{
    while(queue_src->Next != NULL)
        queue_src = queue_src->Next;

    while(queue_src->Back != NULL)
    {
        queue_src = queue_src->Back;
        Push_Queue(queue_dst, queue_src->value, 0);
    }

    queue_src = NULL;
    free(queue_src);

    queue_dst = NULL;
    free(queue_dst);
}

void Init_List_Doubly(List_Doubly** Listd)
{
    List_Doubly *aux = NULL;
    *Listd = (List_Doubly*)malloc(sizeof(List_Doubly));
    aux = *Listd;

    aux->value = 0;
    aux->Next = NULL;
    aux->Back = NULL;
    aux = NULL;
    free(aux);
}
