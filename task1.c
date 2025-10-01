#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>   

#define MAX_LEN 100   // buffer length


void removeSpaces(char* str) {
    char* i = str;  // read pointer
    char* j = str;  // write pointer

    while (*i) {
        if (*i != ' ' && *i != '\n') {  // ignore spaces and newline
            *j++ = *i;
        }
        i++;
    }
    *j = '\0';  
}

// --- Evaluate expression with DMAS order ---
int evaluateExpression(char* expr, int* error) {
    int nums[100];   // numbers
    char ops[100];   // + or -
    int nTop = -1, oTop = -1;

    // first number must be valid
    if (!isdigit((unsigned char)*expr)) {
        *error = 2;  // invalid expression
        return 0;
    }
    nums[++nTop] = strtol(expr, &expr, 10);

    // loop through operators and numbers
    while (*expr) {
        char op = *expr++;  // operator

        // operator must be valid
        if (!(op == '+' || op == '-' || op == '*' || op == '/')) {
            *error = 2;
            return 0;
        }

        // next number must exist
        if (!isdigit((unsigned char)*expr)) {
            *error = 2;
            return 0;
        }

        int num = strtol(expr, &expr, 10);

        if (op == '*') {
            nums[nTop] *= num;
        } else if (op == '/') {
            if (num == 0) { 
                *error = 1;  // division by zero
                return 0;
            }
            nums[nTop] /= num;
        } else {
            ops[++oTop] = op;
            nums[++nTop] = num;
        }
    }

    int result = nums[0];
    int idx = 1;
    for (int j = 0; j <= oTop; j++) {
        if (ops[j] == '+')
            result += nums[idx++];
        else
            result -= nums[idx++];
    }

    return result;
}

int main(void) {
    char expr[MAX_LEN];

    printf("Enter expression: ");
    if (fgets(expr, sizeof(expr), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }

    removeSpaces(expr);  

    int error = 0;
    int result = evaluateExpression(expr, &error);

    if (error == 1) {
        printf("Error: Division by zero!\n");
    } else if (error == 2) {   
        printf("Error: Invalid expression!\n");
    } else {
        printf("Result = %d\n", result);
    }

    return 0;
}

