#define DEBUG

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// #line 10000 "max_is_loh.cpp"

#ifdef DEBUG
    #define dbg        if(1)
    #define DBG(...) __VA_ARGS__
#else
    #define dbg        if(0)
    #define DBG(...)
#endif

// #define STACK_CTOR(stack, capacity) Stack_Information (stack, capacity, #stack, __FILE__, __LINE__)
#define STACK_CTOR(stack, capacity) stack_ctor (stack, capacity, #stack, __FILE__, __LINE__)
#define STACK_PUSH(stack, elem)     stack_push (stack, elem, __func__, __LINE__)
#define STACK_POP(stack, elem)      stack_pop (stack, elem, __func__, __LINE__)
#define STACK_DUMP(stack)           stack_dump (stack, __func__, __LINE__)

typedef double Stack_Elem_t; 
typedef unsigned long long canary_t;

struct Stack_Information
{
    DBG (const char* name;)
    DBG (const char* file;)
    DBG (int line;)
};

struct Stack
{
    DBG (canary_t left_canary;)

    Stack_Elem_t* data;  
    int size;      // количество элементов записывается в стек  
    int capacity; // емкость (максимальный размер буфера)
    Stack_Information info; 

    DBG (canary_t right_canary;)
};

#define CANARY_VALUE (0xDEADC0DEDEADC0DE)

int  stack_ctor (struct Stack* stack, int capacity, DBG (const char* name, const char* file, int line));
void stack_dump (struct Stack* stack, const char* function, int line);
int  stack_push (struct Stack* stack, Stack_Elem_t elem, const char* function, int line);
int  stack_pop  (struct Stack* stack, Stack_Elem_t* elem, const char* function, int line);
int  stack_assertFunc (struct Stack* stack); // верификатор 
int  stack_destroy (struct Stack* stack);
int  stack_error (struct Stack* stack);

int main (void)
{
    printf("\n------------------------------------------------------------------------------\n");

    struct Stack stack = {};
 
    if (STACK_CTOR (&stack, 4) != 0)
    {
        printf("\nStack initialization error\n");
        return 1;
    }

    STACK_PUSH (&stack, 1);
    STACK_PUSH (&stack, 2);
    STACK_PUSH (&stack, 2.2);
    STACK_PUSH (&stack, 4.5);

    STACK_DUMP (&stack);

    Stack_Elem_t x = 0;
    if (STACK_POP (&stack, &x) == 0)
    {
        printf("Popped element: %.2f\n", x);
    }
    
    STACK_DUMP (&stack);

    stack_destroy (&stack);

    printf("\n------------------------------------------------------------------------------\n");

    return 0;
}

int stack_ctor (struct Stack* stack, int capacity, DBG (const char* name, const char* file, int line))
{
    assert(stack);

    stack -> left_canary = CANARY_VALUE;

    stack -> info.name = name;
    stack -> info.file = file;
    stack -> info.line = line;

    stack -> data = (Stack_Elem_t*) realloc (stack -> data, capacity * sizeof(Stack_Elem_t));
    if (stack -> data == NULL) return -1; 

    stack -> capacity = capacity;
    stack -> size     = 0;

    stack->right_canary = CANARY_VALUE; 

    return 0;
}

void stack_dump (struct Stack* stack, const char* function, int line) 
{
    printf("\nStack dump: size = %d, capacity = %d\n", stack -> size, stack -> capacity);
    printf("\nStack[0x%p] called from %s: %d (stack_push) name %s born at %s: %d\n",
            stack, function, line, stack -> info.name, stack -> info.file, stack -> info.line);

    for (int i = 0; i < (stack -> capacity); i++)
    {
        printf("data[%d] = %.2f\n", i, stack->data[i]);
    }
}

int stack_push(struct Stack* stack, Stack_Elem_t elem, const char* function, int line) // TODO как сделаю верификатор. переделай функцию
{
    assert(stack);
    stack_assertFunc(stack);

    if ((stack -> size) >= (stack -> capacity)) 
    {
        stack -> capacity *= 2;
        stack -> data = (Stack_Elem_t*) realloc (stack -> data, stack -> capacity * sizeof(Stack_Elem_t));
        if (stack -> data == NULL) return -1;
    }

    stack -> data[stack->size++] = elem;

    stack_assertFunc(stack);

    return 0;
}

int stack_pop(struct Stack* stack, Stack_Elem_t* elem, const char* function, int line)
{
    assert(stack);
    stack_assertFunc(stack);

    if ((stack -> size) == 0) return -1;

    *elem = stack -> data[--stack->size];

    stack_assertFunc(stack);

    return 0; 
}

int stack_error(struct Stack* stack)
{
    if (stack -> size < 0 || 
        stack -> left_canary  != CANARY_VALUE || 
        stack -> right_canary != CANARY_VALUE)
        return -1;

    return 0;
}

int stack_assertFunc(struct Stack* stack) // верификатор 
{
    if (!stack_error(stack))
    {
        stack_dump(stack, "stack_assertFunc", __LINE__);
        //stack_dump (stack, "stack_assertFunc",__LINE__);
        assert(0);
    }

    return 0;
}

int stack_destroy (struct Stack* stack)
{
    free (stack->data);
    stack -> size      = 0;    
    stack ->  capacity = 0; 

    return 0;
}