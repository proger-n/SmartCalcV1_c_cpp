#include "../s21_smartcalc.h"

int rpn_and_calculate(lexem_t *list, double *result) {
  int ret = 0;
  reverse_stack(&list);
  lexem_t *ready_list = NULL, *support_list = NULL, temp_support;
  if (list) {
    do {
      lexem_t temp_lexeme = pop(&list);
      if (temp_lexeme.type == BRACE_CLOSE) {
        while (support_list->type != BRACE_OPEN) {
          temp_support = pop(&support_list);
          push(&ready_list, temp_support.value, temp_support.priority,
               temp_support.type);
          temp_support = peek(support_list);
        }
        pop(&support_list);
      } else if (temp_lexeme.type == BRACE_OPEN) {
        push(&support_list, temp_lexeme.value, temp_lexeme.priority,
             temp_lexeme.type);
      } else if (temp_lexeme.type == NUMBER || temp_lexeme.type == X) {
        push(&ready_list, temp_lexeme.value, temp_lexeme.priority,
             temp_lexeme.type);
      } else if (temp_lexeme.type != NUMBER) {
        temp_support = peek(support_list);
        for (; temp_support.type != NOTHING &&
               temp_support.priority >= temp_lexeme.priority;) {
          temp_support = pop(&support_list);
          push(&ready_list, temp_support.value, temp_support.priority,
               temp_support.type);
          temp_support = peek(support_list);
        }
        push(&support_list, temp_lexeme.value, temp_lexeme.priority,
             temp_lexeme.type);
      }
    } while (list != NULL);
    while (support_list != NULL) {
      temp_support = pop(&support_list);
      push(&ready_list, temp_support.value, temp_support.priority,
           temp_support.type);
      temp_support = peek(support_list);
    }
    reverse_stack(&ready_list);
    ret = calculate(ready_list, result);
  } else
    ret = 1;
  return ret;
}

double calculate(lexem_t *list, double *result) {
  int ret = 0;
  struct res_stack res;
  int i = 0;
  if (list) {
    do {
      lexem_t current_lexeme = pop(&list);
      if (current_lexeme.type == NUMBER) {
        res.val[i] = current_lexeme.value;
        i++;
      } else if (is_func_unary(current_lexeme.type)) {
        res.val[i - 1] = unary_func_using(current_lexeme.type, res.val[i - 1]);
      } else if (is_func_binary(current_lexeme.type)) {
        res.val[i - 2] = binary_func_using(current_lexeme.type, res.val[i - 2],
                                           res.val[i - 1]);
        i--;
      }
    } while (list);

    *result = res.val[0];
  } else
    ret = 1;

  return ret;
}

double unary_func_using(type_t type, double val) {
  double result = 0.0;
  if (type == SIN)
    result = sin(val);
  else if (type == COS)
    result = cos(val);
  else if (type == TAN)
    result = tan(val);
  else if (type == ACOS)
    result = acos(val);
  else if (type == ASIN)
    result = asin(val);
  else if (type == ATAN)
    result = atan(val);
  else if (type == LN)
    result = log(val);
  else if (type == LOG)
    result = log10(val);
  else if (type == SQRT)
    result = sqrt(val);
  return result;
}

double binary_func_using(type_t type, double val_1, double val_2) {
  double result = 0.0;
  if (type == PLUS)
    result = val_1 + val_2;
  else if (type == MINUS)
    result = val_1 - val_2;
  else if (type == MULT)
    result = val_1 * val_2;
  else if (type == DIV)
    result = val_1 / val_2;
  else if (type == MOD)
    result = fmod(val_1, val_2);
  else if (type == POW)
    result = pow(val_1, val_2);
  return result;
}

int is_func_unary(type_t type) {
  int ret = 0;
  if (type == SIN || type == COS || type == TAN || type == ASIN ||
      type == ACOS || type == ATAN || type == LN || type == LOG || type == SQRT)
    ret = 1;
  return ret;
}

int is_func_binary(type_t type) {
  int ret = 0;
  if (type == PLUS || type == MINUS || type == MULT || type == DIV ||
      type == MOD || type == POW)
    ret = 1;
  return ret;
}
