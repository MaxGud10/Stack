#define DEBUG
#define USE_CANARIES

#include "TXLib.h" 
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h> // TODO скачать TXLIbe
#include <stdint.h>

#include "stack.h"

//TODO: files dividing
//main.cpp main.h
//stack.cpp stack.h
//error.h ...

#ifdef USE_CANARIES
    #define CANARY_PROTECT
    #define CANARY_VALUE (0x07060504030201) // 0x07060504030201 // 0xDEADC0DEDEADC0DE
#endif

#ifdef DEBUG
    #define dbg        if(1)
    #define DBG(...) __VA_ARGS__
#else
    #define dbg        if(0)
    #define DBG(...)
#endif // 06 - 12:20 фото 


//typedef double Stack_Elem_t; 
typedef unsigned long long Stack_Elem_t;
// typedef int Stack_Elem_t;

// typedef unsigned long long canary_t;
// typedef unsigned long long canary_buffer;
// typedef unsigned long long canary_t; // uint64_t
// typedef unsigned long long canary_buffer;

// canary_buffer giga_chicken_left  = 0xDEADC0DEDEADC0BA;
// canary_buffer giga_chicken_right = 0xDEADC0DEDEADC0BA;


enum Stack_Error 
{
    STACK_CTOR_NULL            = 1,
    STACK_POP_EMPTY            = 2, 
    STACK_PUSH_OVERFLOW        = 4,
    STACK_REALLOC_FAIL         = 8,
    STACK_INVALID_SIZE         = 16,
    STACK_INVALID_CANARY       = 32,
    STACK_INVALID_CAPACITY     = 64,
    STACK_CANARY_LEFT_INVALID  = 128,
    STACK_CANARY_RIGHT_INVALID = 256,
    STACK_INVALID_NULL_POINTER = 512
};

struct Stack_Information
{
    DBG (const char* name;)
    DBG (const char* file;)
    DBG (const char* function;)
    DBG (int line;)
};

//TODO: typedef struct
struct Stack
{
    #ifdef CANARY_PROTECT
    canary_t left_canary;
    #endif

    Stack_Elem_t* data;  
    int size;      // количество элементов записывается в стек  
    int capacity; // емкость (максимальный размер буфера)
    Stack_Information info; 

    #ifdef CANARY_PROTECT
    canary_t right_canary;
    #endif
};

enum Stack_Error stack_ctor (struct Stack* stack, int capacity DBG (, struct Stack_Information info));

void stack_dump (struct Stack* stack DBG (, struct Stack_Information info));

int  stack_push (struct Stack* stack, Stack_Elem_t  elem DBG(, struct Stack_Information info));
int  stack_pop  (struct Stack* stack, Stack_Elem_t* elem DBG(, struct Stack_Information info));

int  stack_error      (struct Stack* stack); // верификатор 
int  stack_assertFunc (struct Stack* stack);

int  stack_destroy (struct Stack* stack);

void print_error_message (enum Stack_Error error);


#define STACK_CTOR(stack, capacity) stack_ctor (stack, capacity DBG (, (struct Stack_Information) {#stack, __FILE__, __func__, __LINE__}))
#define STACK_PUSH(stack, elem)     stack_push (stack, elem     DBG (, (struct Stack_Information) {#stack, __FILE__, __func__, __LINE__}))
#define STACK_POP(stack, elem)      stack_pop  (stack, elem     DBG (, (struct Stack_Information) {#stack, __FILE__, __func__, __LINE__}))
#define STACK_DUMP(stack)           stack_dump (stack           DBG (, (struct Stack_Information) {#stack, __FILE__, __func__, __LINE__}))

int main (void)
{
    dbg printf("\n------------------------------------------------------------------------------\n");

    struct Stack stack = {};

    enum Stack_Error ded_loh = STACK_CTOR (&stack, 4);
    if (ded_loh != STACK_CTOR_NULL) // STACK_CTOR (&stack, 4) != 0
    {
        print_error_message (ded_loh);
        return ded_loh;
    }

    printf("\nsizeof(STACK) = %lld\n", sizeof(struct Stack));

    dbg printf("DED_LOH_0");
    STACK_PUSH (&stack, 1);
    dbg printf("\nded kvadrakopter_1\n");
    STACK_PUSH (&stack, 2);
    STACK_PUSH (&stack, 2.2);
    STACK_PUSH (&stack, 4.5);

    STACK_DUMP (&stack);
    txDump(stack.data - 1);
    dbg printf("\nDED_LOH_1\n");

    Stack_Elem_t x = 0;
    if (STACK_POP (&stack, &x) == 0)
        printf("Popped element: %.2f\n", x);
    else
        print_error_message(STACK_POP_EMPTY);

    dbg printf("\nDED_LOH_2\n");
    STACK_DUMP (&stack);

    dbg printf("\n------------------------------------------------------------------------------\n");

    stack_destroy (&stack);

    return 0;
}

void print_error_message (enum Stack_Error error)
{
    switch (error)
    {
        //TODO: WARNING all enum cases
        case STACK_CTOR_NULL:
            printf("\nStack initialization error\n");
            break;
        case STACK_POP_EMPTY:
            printf("\nError: Stack is empty, cannot pop element\n");
            break;
        case STACK_PUSH_OVERFLOW:
            printf("\nError: Stack overflow\n");
            break;
        case STACK_REALLOC_FAIL:
            printf("\nError: Failed to allocate memory\n");
            break;
        case STACK_INVALID_CANARY:
            printf("\nError: stack canaries are corrupted\n");
            break;
        default:
            printf("\nded broke stack\n");
            break;
    }
}

// int stack_ctor (struct Stack* stack, int capacity DBG (, const char* name, const char* file, int line))
enum Stack_Error stack_ctor (struct Stack* stack, int capacity DBG (, struct Stack_Information info))
{
    assert(stack);

    #ifdef CANARY_VALUE
    stack->left_canary = CANARY_VALUE;
    #endif 
// [x] можно ли засунуть в один ifdef канарейки в ctor из стека
    DBG(stack -> info = info;)
//TODO: " -> "  -> "->"
    stack->data = 1 + (Stack_Elem_t*) calloc (capacity + 2, sizeof(Stack_Elem_t));
    //TODO: Заливка POISON значениями 
    if (!stack -> data) 
        return STACK_REALLOC_FAIL; 

    //TODO: Fix warning memcpy int to double memory
    //memcpy 101001010010
    //1001010101001
    //(int) x -> 2.2 "101010" -> 2 "10001" 
    (stack -> data[-1])       = giga_chicken_left;
    (stack -> data[capacity]) = giga_chicken_right;
    
    // ? Add независмые канарейки от размера элемента стека -> юзать арифметику укзателей на char* свою и делать memcpy 

    dbg printf("sizeof(DATA) = %lld, sizeof(CAPACITY) = %lld\n", sizeof(stack->data), sizeof(capacity));

    stack -> capacity = capacity;
    stack -> size     = 0;


    #ifdef CANARY_PROTECT
    stack -> right_canary = CANARY_VALUE;
    #endif


    return STACK_CTOR_NULL;
}

// void stack_dump (struct Stack* stack DBG (, const char* function, int line)) 
void stack_dump (struct Stack* stack DBG (, struct Stack_Information info)) 
{
    dbg printf("\n__________________________________________________________\n");

    printf("\nStack dump: size = %d, capacity = %d\n", stack -> size, stack -> capacity);

    #ifdef CANARY_PROTECT
    printf("\nLeft Canary: %llx\n", stack->data[-1]); // 0x07060504030201
    #endif

    uint64_t err = stack_error(stack);

    if (err & STACK_INVALID_NULL_POINTER) printf("\nstack = NULL and I have to write ......\n");

    if (err & STACK_INVALID_SIZE)         printf("\nsize < 0\n");

    if (err & STACK_INVALID_CAPACITY)     printf("\ncapacity <=0 \n");

    if (err & STACK_CANARY_LEFT_INVALID)  printf("\nleft_canary != CANARY_VALUE\n");

    if (err & STACK_CANARY_RIGHT_INVALID) printf("\nright_canary != CANARY_VALUE\n");


    DBG (printf("\nStack[0x%p] called from %s: %d (----%s----) name %s born at %s:%d\n",
            stack, info.function, info.line, info.function, stack -> info.name, stack -> info.file, stack -> info.line));

    for (int i = 0; i <= (stack -> capacity); i++)
        printf("%s data[%2d] = %.2f\n", (i < stack -> size ? " * " : "   "),i, stack->data[i]);
        // TODO: Check for your poison value
        // data[4] = 11033793 (POISON)
    #ifdef CANARY_PROTECT
    printf("\nRight Canary: %llx\n", stack->data[stack->capacity]);
    #endif

    // #ifdef CANARY_PROTECT
    // if (stack->left_canary != CANARY_VALUE || stack->right_canary != CANARY_VALUE) 
    //     printf("Warning: Stack canaries are corrupted.\n");
    // #endif

    dbg printf("\n__________________________________________________________\n");
}

// int stack_push(struct Stack* stack, Stack_Elem_t elem DBG(, const char* function, int line))
int stack_push(struct Stack* stack, Stack_Elem_t elem DBG(, struct Stack_Information info)) // TODO как сделаю верификатор. переделай функцию
{
    assert(stack);
    stack_assertFunc(stack); //TODO: Add in assertFunc - info information! 

    if ((stack -> size) >= (stack -> capacity)) 
    {
        int new_capacity = stack -> capacity * 2;

        Stack_Elem_t* new_data = 1 + (Stack_Elem_t*) realloc (stack->data + 1, (new_capacity + 2) * sizeof(Stack_Elem_t));
        //TODO: capacity - size - заполнить Poison
        if (!new_data) return STACK_REALLOC_FAIL;

        (stack -> data[new_capacity]) = giga_chicken_right;

        stack -> data     = new_data;
        stack -> capacity = new_capacity;
    }

    stack -> data[stack->size++] = elem;

    stack_assertFunc(stack);

    return 0;
}

// int stack_pop(struct Stack* stack, Stack_Elem_t* elem DBG(, const char* function, int line))
int stack_pop(struct Stack* stack, Stack_Elem_t* elem DBG(, struct Stack_Information info)) // TODO сделать realloc когда мы вытащили из массива 
{
    assert(stack);
    stack_assertFunc(stack);

    if ((stack -> size) == 0) return STACK_POP_EMPTY;

    *elem = stack -> data[--stack->size];
    //TODO: REALLOC REVERT
    stack -> data[stack -> size] = 13116274355181; // BED_DED_F_ABED//TODO: REMOVE MAGIC NUMBER 

    stack_assertFunc(stack);

    return 0; 
}

// int stack_error(struct Stack* stack) // верификатор 
// {
//     int x = ...;
//     if (!stack)               return STACK_INVALID_NULL_POINTER; 
    
//     if (stack->size < 0)      return STACK_INVALID_SIZE;

//     if (stack->capacity <= 0) return STACK_INVALID_CAPACITY;

//     #ifdef CANARY_PROTECT
//     if (stack->left_canary  != CANARY_VALUE) return STACK_CANARY_LEFT_INVALID;
//     if (stack->right_canary != CANARY_VALUE) return STACK_CANARY_RIGHT_INVALID;
//     #endif






    // if (!stack || 
    //     stack -> size     <  0 || 
    //     stack -> capacity <= 0)
    //     return STACK_INVALID_SIZE;

    // #ifdef CANARY_PROTECT
    // if (stack -> left_canary  != CANARY_VALUE || 
    //     stack -> right_canary != CANARY_VALUE)
    //     return STACK_INVALID_CANARY;
    // #endif
    //TODO: Add better collection of errors in one number побитовые ошибки
    // K&R - битовые операции, а именно & и наложение маски на число

    //int x = 0; 
    //int x = 101010100101; 
    // x | 8
    //if (!stack) {x = x & };
    //0 bit - stack null

//     return 0;
// }

int stack_error(struct Stack* stack)
{
    uint64_t err = 0;

    if (!stack) return STACK_INVALID_NULL_POINTER;

    if (stack->size < 0) err |= STACK_INVALID_SIZE;

    if (stack->capacity <=0) err |= STACK_INVALID_CAPACITY;

    #ifdef CANARY_PROTECT
    if (stack->left_canary  != CANARY_VALUE) err |= STACK_CANARY_LEFT_INVALID;
    if (stack->right_canary != CANARY_VALUE) err |= STACK_CANARY_RIGHT_INVALID;
    #endif


    return err;
}

int stack_assertFunc(struct Stack* stack) // верификационный палач 
{
    int error_code = stack_error(stack);
    if (error_code)
    {
        //Check bit: n bit check -> x & 2^n
        //TODO: Check all bits &
        stack_dump(stack DBG(, (struct Stack_Information) {"stack_assertFunc", __FILE__, __func__, __LINE__}));
        if (stack == NULL)
            printf("ERROR: stack = %p\n", stack);

        if ((stack -> data) == NULL)
            printf("ERROR: stack->data = %p\n", (stack -> data));

        if ((stack ->size) > (stack -> capacity))
            printf("ERROR: stack->size = %d | stack->capacity = %d\n", (stack -> size), (stack -> capacity));

        assert(0);
    } 

    //return 0;
    return error_code;
}

int stack_destroy (struct Stack* stack)
{
    free (stack -> data);
    stack -> data      = NULL;
    stack -> size      = 0;    
    stack -> capacity = 0; 
    //TODO: canary destroy
    return 0;
}

