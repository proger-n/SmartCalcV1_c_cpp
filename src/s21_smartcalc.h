#ifndef S21_SMARTCALC
#define S21_SMARTCALC

#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define N 255

typedef enum {
  NUMBER,
  X,
  BRACE_OPEN,
  BRACE_CLOSE,
  ARITHMETIC,
  FUNCTION,
  ASIN,
  ACOS,
  ATAN,
  SIN,
  SQRT,
  LOG,
  LN,
  MOD,
  COS,
  TAN,
  PLUS,
  MINUS,
  DIV,
  MULT,
  POW,
  NOTHING
} type_t;

typedef struct lexem_struct {
  double value;
  int priority;
  struct lexem_struct* next;
  type_t type;
} lexem_t;

struct res_stack {
  double val[255];
  int amount;
};

int head_calc(char* input, int x_exist, double x_value, double* result);

// input processing
int validation_input(char* input);
int parsing_input(char* input, lexem_t** list, int x_exist, double x_value);
int search_substr(char* input, char search_str[], size_t len_input_str,
                  size_t* i,
                  size_t count_char_after);  // 1 - found, 0 - not or err
int search_num(char* input, size_t* i, double* number, int x_exist,
               double x_value);
int search_operand(char* input, size_t i, int* en, int* priority);
void add_zero_before_minus(char* input, size_t minus_index);
void remove_spaces(char* input);

// calculations
int rpn_and_calculate(lexem_t* list, double* result);
double calculate(lexem_t* list, double* result);
double unary_func_using(type_t type, double val);
double binary_func_using(type_t type, double val_1, double val_2);
int is_func_unary(type_t type);
int is_func_binary(type_t type);

// stack
void push(lexem_t** list, double value, int priority, type_t type);
lexem_t pop(lexem_t** plist);
lexem_t peek(lexem_t* plist);
void reverse_stack(lexem_t** plist);
void delete_stack(lexem_t** plist);
// void print_stack(lexem_t* list);
#endif
