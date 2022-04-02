//
// file: hpcalc.c
// description: simulates a hp calculator
//
// @author Ryan Nowak rcn8263
// 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>


#define CAPACITY 10 // Maximum size of the stack
#define BUFFER 1024 // Size of the input

/// structure defining the stack where size is the current size of the stack,
/// capacity is the max capacity of the stack before it must be reallocated, 
/// and items is the array of integers in the stack.
typedef struct stack {
    int size;
    int capacity;
    int *items;
} stack_s;

/// Creates and initialized a new stack structure and allocates the necessary 
/// amount of memory
/// 
/// @return the created structure
static stack_s* make_stack() {
    stack_s *st = (stack_s *) malloc(sizeof(struct stack));
    st->size = -1;
    st->capacity = CAPACITY;
    st->items = (int *) calloc(st->capacity, sizeof(char) * st->capacity);
    
    return st;
}

/// frees all of the allocated memory within the given stack structure
/// 
/// @param st the stack_s structure that is to be freed
static void free_stack(stack_s  *st) {
    free(st->items);
    free(st);
}

/// returns the current number of elements in the given stack
/// 
/// @param st stack to get the current size of
/// @return integer representing the current size of the stack
static int get_size(stack_s *st) {
    return st->size + 1;
}


/// pushes the given integer on top of the stack
/// 
/// @param st stack to push onto
/// @param i integer to push onto the stack
static void push(stack_s *st, int i) {
    st->size += 1;
    st->items[st->size] = i;
}

/// pops the topmost element off the given stack
/// 
/// @param st stack to pop the element off of
/// @return integer of the top most element that was on the stack
static int pop(stack_s *st) {
    int temp = st->items[st->size];
    st->size -= 1;
    return temp;
}

/// simulates the hp calculator
int main() {
    stack_s *st = make_stack();
    char in[BUFFER];
    const char *delim = " \n";
    char *token;
    
    printf("hpcalc\n");
    while (1) {
        
        if(fgets(in, BUFFER, stdin) == NULL) {
            break;
        }
        if (!strcmp(in, "\n")) {
            continue;
        }
        
        //parses input into a char array of strings
        char *cmd[BUFFER] = {NULL};
        int numArgs = 0;
        token = strtok(in, delim);
        while (token != NULL) {
            cmd[numArgs] = token;
            numArgs++;
            token = strtok(NULL, delim);
        }
        
        int i = 0;
        while (cmd[i] != NULL) {
            char *temp = cmd[i];

            //check for positive integer
            if (isdigit(*cmd[i])) {
                push(st, strtol(cmd[i], NULL, 10));
            }
            //check for negative integer
            if (temp[0] == '-' && strlen(temp) > 1) {
                if (isdigit(temp[1])) {
                    push(st, -1 * strtol(&temp[1], NULL, 10));
                }
            }
            else if (!strcmp(cmd[i], "*")) {
                push(st, pop(st) * pop(st));
            }
            else if (!strcmp(cmd[i], "+")) {
                push(st, pop(st) + pop(st));
            }
            else if (!strcmp(cmd[i], "-")) {
                int temp1 = pop(st);
                int temp2 = pop(st);
                push(st, temp2 - temp1);
            }
            else if (!strcmp(cmd[i], "/")) {
                int temp1 = pop(st);
                int temp2 = pop(st);
                // check for divide by zero
                if (temp1 == 0) {
                    fprintf(stderr, "error: divide by 0: %d / %d\n", 
                        temp2, temp1);
                    push(st, temp2);
                }
                else {
                    push(st, temp2 / temp1);
                }
            }
            else if (!strcmp(cmd[i], "^")) {
                push(st, pow(pop(st),pop(st)));
            }
            else if (!strcmp(cmd[i], "%")) {
                int temp1 = pop(st);
                int temp2 = pop(st);
                // check for divide by zero
                if (temp1 == 0) {
                    fprintf(stderr, "error: divide by 0: %d %% %d\n", 
                        temp2, temp1);
                    push(st, temp1);
                }
                else {
                    push(st, temp2 % temp1);
                }
            }
            else if (!strcmp(cmd[i], ".")) {
                printf("%d\n", pop(st));
            }
            else if (!strcmp(cmd[i], ".s")) {
                printf("%d\n", get_size(st));
            }
            else if (!strcmp(cmd[i], "bye")) {
                free_stack(st);
                printf("ok\n");
                return 0;
            }
            
            i++;
        }
    }
    free_stack(st);
    
    return 0;
}