#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>   
#define MAX_LEN 100   

int solve(char* expr, int* error) {
    int numstack[100];  
    char opstack[100]; 
    int result=0; 
    int nTop = -1, oTop = -1;

    if (!isdigit((unsigned char)*expr)) {
        *error = 2;}
    numstack[++nTop] = strtol(expr, &expr, 10);

    while (*expr) {
        char op = *expr++; 
        if (!(op == '+' || op == '-' || op == '*' || op == '/')) {
            *error = 2; }
        if (!isdigit((unsigned char)*expr)) {
            *error = 2; }
        int num = strtol(expr, &expr, 10);
        if (op == '*') {
            numstack[nTop] *= num;
        } else if (op == '/') {
            if (num == 0) { 
                *error = 1;
                return 0; }
            numstack[nTop] /= num;
        } else {
            opstack[++oTop] = op;
            numstack[++nTop] = num;
        }
    }
    result = numstack[0];
    int idx = 1;
    for (int j = 0; j <= oTop; j++) {
        if (opstack[j] == '+')
            result += numstack[idx++];
        else
            result -= numstack[idx++];}
    return result;}

void removeSpaces(char* str) {
    char* readptr = str;  
    char* writeptr = str;  
    while (*readptr) {
        if (*readptr!= ' ' && *readptr != '\n') {  
            *writeptr++ = *readptr;}
        readptr++;}
    *writeptr = '\0';}

int readexpression(char* expr) {
    int read_status = 0; 
    printf("Enter expression: ");
    if (fgets(expr, MAX_LEN, stdin) != NULL)  {
        read_status = 1; 
    } else {
    printf("Error reading input.\n");}
    return read_status; }

int main(void) {
    char expr[MAX_LEN];
    int error = 0; 
    int result = 0; 
    int read_status = readexpression(expr);
    if(read_status){
    removeSpaces(expr);
    result = solve(expr, &error);
    } else {
        error = 2;}
        
    if (error == 1) {
    printf("Error: Division by zero.\n");
    } else if (error == 2) { 
    printf("Error: Invalid expression.\n");
    } else {
    printf("Result = %d\n", result);}
    return 0;
}