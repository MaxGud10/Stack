#ifndef STACK.H
#define STACK.H

typedef unsigned long long canary_t;
typedef unsigned long long canary_buffer;

canary_buffer giga_chicken_left  = 0xDEADC0DEDEADC0BA;
canary_buffer giga_chicken_right = 0xDEADC0DEDEADC0BA;

// TODO хранить хеш 

#endif // STACK>H 
// struct Stack
// {
//     int __do_no_touch[40]
// };