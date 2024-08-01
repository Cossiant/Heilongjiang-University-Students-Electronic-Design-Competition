/*
 * @author: yihang_01
 * @Date: 2024-08-01 11:47:09
 * @LastEditTime: 2024-08-01 12:40:46
 * QwQ 加油加油
 */
#ifndef _CHESS_STACK_H
#define _CHESS_STACK_H

#include <stdio.h>
#include <stdint.h>

typedef struct {
    int j1, j2, j3, j4;
} Pos;

typedef struct {
    int top;
    int capacity;
    Pos array[10];
} Stack;

void Stack_init(Stack *stack) {
    stack->top = -1;
    stack->capacity = 10;
}

int Stack_isEmpty(Stack *stack) {
    return stack->top == -1;
}

int Stack_isFull(Stack *stack) {
    return stack->top == stack->capacity - 1;
}

void Stack_push(Stack *stack, int j1, int j2, int j3, int j4) {
    if (Stack_isFull(stack)) {
        return;
    }
    stack->array[++(stack->top)] = (Pos){j1, j2, j3, j4};
}

void Stack_pushPos(Stack *stack, Pos item) {
    if (Stack_isFull(stack)) {
        return;
    }
    stack->array[++(stack->top)] = item;
}

int Stack_pop(Stack *stack, ROBOT_DATA *data) {
    if (Stack_isEmpty(stack)) {
        return 1;
    }
    data->move_j4 = stack->array[stack->top].j1;
    data->move_j3 = stack->array[stack->top].j2;
    data->move_j2 = stack->array[stack->top].j3;
    data->move_j1 = stack->array[stack->top].j4;
    stack->top--;
    return 0;
}

Stack whiteStack, blackStack;

#endif // _CHESS_STACK_H
