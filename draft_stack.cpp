#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef double Stack_Elem_t; 

struct Stack
{
    Stack_Elem_t* data;  
    int size;      // количество элементов записывается в стек  
    int capacity; // емкость (максимальный размер буфера)
};

int  stack_ctor (struct Stack* stack, int capacity);
int  stack_push (struct Stack* stack, Stack_Elem_t  elem);
int  stack_pop  (struct Stack* stack, Stack_Elem_t* elem);
void stack_dump (struct Stack* stack);
int  stack_destroy (struct Stack* stack);

int main (void)
{
    struct Stack stack = {};
 
    if (stack_ctor(&stack, 4) != 0)
    {
        printf("\nStack initialization error\n");
        return 1;
    }

    //printf("sizeof(%d)", sizeof(stack->data));

    stack_push(&stack, 1  );
    stack_push(&stack, 2  );
    stack_push(&stack, 2.2);
    stack_push(&stack, 4.5);

    stack_dump(&stack);

    Stack_Elem_t x = 0;

    stack_pop(&stack, &x);

    stack_dump(&stack);

    stack_destroy(&stack);

    return 0;
}

int stack_destroy (struct Stack* stack)
{
    free (stack->data);
    stack -> size     = 0;    
    stack ->  capacity = 0; 

    return 0;
}
int stack_ctor (struct Stack* stack, int capacity)
{
    stack -> data = (Stack_Elem_t*) realloc (stack -> data, stack -> capacity * sizeof(Stack_Elem_t));
    if (stack -> data == NULL) return -1; 

    stack -> capacity = capacity;

    return 0;
}

void stack_dump (struct Stack* stack)
{
    printf("Stack dump: size = %d, capacity = %d\n", stack -> size, stack -> capacity);

    for (int i = 0; i < (stack -> capacity); i++)
    {
        printf("data[%d] = %.2f\n", i, stack->data[i]);
    }
}

int stack_push (struct Stack* stack, Stack_Elem_t elem)
{
    if ((stack -> size) >= (stack -> capacity))
    {
        stack -> capacity *=2;
        stack -> data = (Stack_Elem_t*) realloc (stack -> data, stack -> capacity * sizeof(Stack_Elem_t));
        if (stack -> data == NULL) return -1;
    } 

    stack -> data[stack -> size++] = elem;
    return 0;
}

int stack_pop (struct Stack* stack, Stack_Elem_t* elem)
{
    if (stack -> size == 0) return -1;

    *elem = stack -> data[--stack->size];

    return 0; 
}


// void stack_dump(const struct Stack* stack)
// {
//     printf("Stack dump: size = %d, capacity = %d\n", stack->size, stack->capacity);
//     for (int i = 0; i < stack->size; i++)
//     {
//         printf("data[%d] = %f\n", i, stack->data[i]);
//     }
// }