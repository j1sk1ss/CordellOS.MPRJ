#include "../include/calculator.h"

#include "../../../kernel/include/string.h"
#include "../../../kernel/include/stdio.h"

typedef struct {
    double stack[MAX_CALCULATOR_STACK_SIZE];
    int top;
} calc_stack;

static void stack_push(calc_stack *c_stack, double value) {
    if (c_stack->top >= MAX_CALCULATOR_STACK_SIZE)
        return;

    c_stack->stack[++c_stack->top] = value;
}

static double stack_pop(calc_stack *c_stack) {
    double val;
    
    val = c_stack->stack[c_stack->top--];
    return val;
}

char* calculator(char **args, int n_args) {
    calc_stack c_stack;
    double a, b, res;

    memset(&c_stack.stack, 0, sizeof(c_stack.stack));
    c_stack.top = -1;

    if (n_args < 3) 
        return "null";

    for (int i = 0; i < n_args; i++) {
        if (isdigit(*args[i])) {
            stack_push(&c_stack, atof(args[i]));
            continue;
        }

        switch(*args[i]) {
            case '+':
                if (i + 1 < n_args && isdigit(*args[i + 1])) 
                    stack_push(&c_stack, atof(args[i]));
                else {
                    a = stack_pop(&c_stack);
                    b = stack_pop(&c_stack);

                    stack_push(&c_stack, a + b);
                }

            break;

            case '-':
                if (i + 1 < n_args && isdigit(*args[i + 1])) 
                    stack_push(&c_stack, atof(args[i]));
                else {
                    b = stack_pop(&c_stack);
                    a = stack_pop(&c_stack);

                    stack_push(&c_stack, a - b);
                }
            break;

            case '*':
                a = stack_pop(&c_stack);
                b = stack_pop(&c_stack);

                stack_push(&c_stack, a * b);
            break;

            case '/':
                b = stack_pop(&c_stack);
                a = stack_pop(&c_stack);

                if (b == 0) {
                    printf("Division by zero!");
                    return "null";
                }

                stack_push(&c_stack, a / b);
            break;

            case 'p':
                a = stack_pop(&c_stack);
                b = stack_pop(&c_stack);

                int answer = 1;
                for (int i = 0; i < b; i++)
                    answer *= a;

                stack_push(&c_stack, answer);
            break;
        }
    }

    res = stack_pop(&c_stack);
    return double_to_string(res);
}