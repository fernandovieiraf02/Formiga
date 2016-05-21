struct S_List_Doubly
{
    int value;
    int value_aux;
    struct S_List_Doubly *Next;
    struct S_List_Doubly *Back;
};

typedef struct S_List_Doubly Stack;
typedef struct S_List_Doubly Queue;
typedef struct S_List_Doubly List_Doubly;


void Init_List_Doubly(List_Doubly**);
///Inicializa uma lista dupla encadeada

void Init_Stack(Stack**);
///Inicializa a pilha

void Init_Queue(Queue**);
///Inicializa a Fila

void Push_Stack(Stack** ,int, int);
///Adiciona um elemento a pilha

void Push_Queue(Queue**, int, int);
///Adiciona um elemento a fila

void Show_Stack(Stack*);
///Imprime a pilha

void Show_Queue(Queue*);
///Imprime a Fila

int Pop_Stack(Stack*);
///Retira e retorna o valor do ultimo elemento da pilha

void Pop_Stack_Double(Stack* p_stack, int*, int*);
///Retira e retorna os dois valores inteiros da ultima posição da pilha

int Pop_Queue(Queue*);
///Retira o primeiro elemento da fila, e retorna seu valor

void Pop_Queue_Double(Queue* p_queue, int *x, int *y);
///Retira o primeiro elemento da fila e passa seus valores para x e y

int Find_Queue(Queue*, int);
///procura um elemento na fila

int Find_Queue_Double(Queue* , int, int);
///procura dois valores na mesma posição de uma fila

void Copy_Queue(Queue* , Queue**);
///Copia uma fila para outra

int Find_Stack(Stack*, int, int);
///procura um elemento na pilha

int Void_Stack(Stack*);
///verifica se a pilha nao esta vazia.
///Retorna 0 zero se estiver vazia, caso contrario retorna 1.

int Count_Stack(Stack*);///Conta quantos elementos tem na pilha
