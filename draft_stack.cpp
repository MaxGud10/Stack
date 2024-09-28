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

#define STACK_CTOR(stack, capacity) stack_ctor (stack, capacity, #stack, __FILE__, __LINE__)
#define STACK_PUSH(stack, elem)     stack_push (stack, elem, __func__, __LINE__)
#define STACK_POP(stack, elem)      stack_pop (stack, elem, __func__, __LINE__)
#define STACK_DUMP(stack)           stack_dump (stack, __func__, __LINE__)

typedef double Stack_Elem_t; 

struct Stack
{
    DBG (const char* name;)
    DBG (const char* file;)
    DBG (int line;)

    Stack_Elem_t* data;  
    int size;      // количество элементов записывается в стек  
    int capacity; // емкость (максимальный размер буфера)
};

int  stack_ctor (struct Stack* stack, int capacity, DBG (const char* name, const char* file, int line));
void stack_dump (struct Stack* stack, const char* function, int line);
int  stack_push (struct Stack* stack, Stack_Elem_t elem, const char* function, int line);
int  stack_pop  (struct Stack* stack, Stack_Elem_t* elem, const char* function, int line);
int  stack_assertFunc (struct Stack* stack); // верификатор 
int  stack_destroy (struct Stack* stack);

int main (void)
{
    struct Stack stack = {};
 
    if (STACK_CTOR (&stack, 4) != 0)
    {
        printf("\nStack initialization error\n");
        return 1;
    }

    //stack_push (&stack, 1, "main",  __LINE__); // define main, __LINE__
    STACK_PUSH (&stack, 1);
    STACK_PUSH (&stack, 2);
    STACK_PUSH (&stack, 2.2);
    STACK_PUSH (&stack, 4.5);
    // stack_push (&stack, 2, "main",  __LINE__);
    // stack_push (&stack, 2.2, "main", __LINE__);
    // stack_push (&stack, 4.5, "main", __LINE__);

    STACK_DUMP (&stack);
    // stack_dump (&stack, "main", __LINE__);

    Stack_Elem_t x = 0;
    STACK_POP (&stack, &x);
    // stack_pop (&stack, &x, "main", __LINE__);

    
    STACK_DUMP (&stack);
    // stack_dump (&stack, "main", __LINE__);

    stack_destroy (&stack);


    return 0;
}

int stack_ctor (struct Stack* stack, int capacity, DBG (const char* name, const char* file, int line))
{
    assert(stack);

    stack -> name = name;
    stack -> file = file;
    stack -> line = line;

    stack -> data = (Stack_Elem_t*) realloc (stack -> data, stack -> capacity * sizeof(Stack_Elem_t));
    if (stack -> data == NULL) return -1; 

    stack -> capacity = capacity;
    //stack -> size     = 0;

    return 0;
}

void stack_dump (struct Stack* stack, const char* function, int line) 
{
    printf("\nStack dump: size = %d, capacity = %d\n", stack -> size, stack -> capacity);
    printf("\nStack[0x%p] called from %s: %d (stack_push) name %s born at %s: %d\n",
            stack, function, line, stack -> name, stack -> file, stack -> line);

    for (int i = 0; i < (stack -> capacity); i++)
    {
        printf("data[%d] = %.2f\n", i, stack->data[i]);
    }
}

int stack_push(struct Stack* stack, Stack_Elem_t elem, const char* function, int line) // TODO как сделаю верификатор. переделай функцию
{
    if (stack->size >= stack->capacity) 
    {
        stack -> capacity *= 2;
        stack -> data = (Stack_Elem_t*) realloc (stack -> data, stack -> capacity * sizeof(Stack_Elem_t));
        if (stack -> data == NULL) return -1;
    }

    stack -> data[stack->size++] = elem;

    return 0;
}

int stack_pop(struct Stack* stack, Stack_Elem_t* elem, const char* function, int line)
{
    if (stack -> size == 0) return -1;

    *elem = stack -> data[--stack->size];

    return 0; 
}

int stack_error(struct Stack* stack)
{
    if (stack -> size < 0)
        return -1;

    return 0;
}

int stack_assertFunc(struct Stack* stack) // верификатор 
{
    if (!stack_error(stack))
    {
        STACK_DUMP (stack);
        //stack_dump (stack, "stack_assertFunc",__LINE__);
        assert(0);
    }

    return 0;
}


int stack_destroy (struct Stack* stack)
{
    free (stack->data);
    stack -> size     = 0;    
    stack ->  capacity = 0; 

    return 0;
}