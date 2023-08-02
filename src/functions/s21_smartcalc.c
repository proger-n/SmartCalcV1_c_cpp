#include "../s21_smartcalc.h"

int head_calc(char* input, int x_exist, double x_value, double* result) {
  int ret = 0;
  lexem_t* list = NULL;
  if (!validation_input(input) &&
      !parsing_input(input, &list, x_exist, x_value)) {
    if (rpn_and_calculate(list, result)) ret = 1;
  } else {
    puts("incorrect input");
    ret = 1;
  }
  return ret;
}

int validation_input(char* input) {
  remove_spaces(input);
  int ret = 0;
  int brace_opened = 0;
  char* available_values = "0123456789.,x*/+-^()mdcosinltaqrtg";
  int input_len = strlen(input);
  for (int i = 0; i < input_len && brace_opened >= 0; i++) {
    if (!strchr(available_values, input[i])) {
      ret = 1;
    } else if (input[i] == '(') {
      brace_opened += 1;
    } else if (input[i] == ')') {
      brace_opened -= 1;
      if (i > 0 && strchr("+-/*(^", input[i - 1])) brace_opened = -1;
    } else if (strchr("/*^", input[i])) {
      if (i + 1 == input_len) ret = 1;
      if (i > 0 && strchr("(+-/*^", input[i - 1])) ret = 1;
      if ((i + 1) < input_len && strchr(")+-/*^", input[i + 1])) ret = 1;
    } else if (input[i] == '-' || input[i] == '+') {
      if (i + 1 == input_len) ret = 1;
      if (i > 0 && strchr("+-/*^", input[i - 1])) ret = 1;
      if ((i + 1) < input_len && strchr(")+-/*^", input[i + 1])) ret = 1;
    } else if (input[i] == '.' || input[i] == ',')
      if (i > 0 && (input[i - 1] == '.' || input[i - 1] == ',')) ret = 1;
  }
  if (!input_len || brace_opened || ret)
    ret = 1;
  else {
    for (int i = 0; i < input_len; i++) {
      if (input[i] == '-' || input[i] == '+') {
        if (i == 0 || (i > 0 && input[i - 1] == '(')) {
          add_zero_before_minus(input, i);
        }
      }
    }
  }
  return ret;
}

int parsing_input(char* input, lexem_t** list, int x_exist, double x_value) {
  int ret = 0, en = 0, priority = 0;
  size_t len_input_str = strlen(input);
  double number = 0;
  for (size_t i = 0; i < len_input_str && !ret; i++) {
    if (search_num(input, &i, &number, x_exist, x_value)) {
      push(list, number, 0, NUMBER);
    } else if (search_substr(input, "sin(", len_input_str, &i, 2)) {
      push(list, 0, 4, SIN);
      push(list, 0, -1, BRACE_OPEN);
    } else if (search_substr(input, "cos(", len_input_str, &i, 2)) {
      push(list, 0, 4, COS);
      push(list, 0, -1, BRACE_OPEN);
    } else if (search_substr(input, "tan(", len_input_str, &i, 2)) {
      push(list, 0, 4, TAN);
      push(list, 0, -1, BRACE_OPEN);
    } else if (search_substr(input, "asin(", len_input_str, &i, 2)) {
      push(list, 0, 4, ASIN);
      push(list, 0, -1, BRACE_OPEN);
    } else if (search_substr(input, "acos(", len_input_str, &i, 2)) {
      push(list, 0, 4, ACOS);
      push(list, 0, -1, BRACE_OPEN);
    } else if (search_substr(input, "atan(", len_input_str, &i, 2)) {
      push(list, 0, 4, ATAN);
      push(list, 0, -1, BRACE_OPEN);
    } else if (search_substr(input, "log(", len_input_str, &i, 2)) {
      push(list, 0, 4, LOG);
      push(list, 0, -1, BRACE_OPEN);
    } else if (search_substr(input, "ln(", len_input_str, &i, 2)) {
      push(list, 0, 4, LN);
      push(list, 0, -1, BRACE_OPEN);
    } else if (search_substr(input, "sqrt(", len_input_str, &i, 2)) {
      push(list, 0, 4, SQRT);
      push(list, 0, -1, BRACE_OPEN);
    } else if (search_substr(input, "mod", len_input_str, &i, 1)) {
      push(list, 0, 2, MOD);
    } else if (search_operand(input, i, &en, &priority)) {
      push(list, 0, priority, en);
    } else if (input[i] == '(') {
      if (i + 1 < len_input_str && input[i + 1] != ')')
        push(list, 0, -1, BRACE_OPEN);
    } else if (input[i] == ')') {
      if ((i + 1 < len_input_str && input[i + 1] != '(') ||
          i + 1 == len_input_str)
        push(list, 0, 5, BRACE_CLOSE);
    } else {
      ret = 1;
      delete_stack(list);
    }
  }
  return ret;
}

int search_operand(char* input, size_t i, int* en, int* priority) {
  int found = 0;
  if (strchr("+-^*/", input[i])) {
    found = 1;
    if (input[i] == '+') {
      *en = PLUS;
      *priority = 1;
    }
    if (input[i] == '-') {
      *en = MINUS;
      *priority = 1;
    }
    if (input[i] == '*') {
      *en = MULT;
      *priority = 2;
    }
    if (input[i] == '/') {
      *en = DIV;
      *priority = 2;
    }
    if (input[i] == '^') {
      *en = POW;
      *priority = 3;
    }
  }
  return found;
}

int search_num(char* input, size_t* i, double* number, int x_exist,
               double x_value) {
  size_t i_old = *i;
  int found = 0;
  char buf[255] = "\0";
  int j = 0;
  if (input[*i] == 'x' && x_exist) {
    found = 1;
    *number = x_value;
  } else if (strchr("0123456789", input[*i])) {
    found = 1;
    for (; found && strchr("0123456789.,", input[*i]); j++, (*i)++) {
      buf[j] = input[*i] == ',' ? '.' : input[*i];
    }
    buf[j] = '\0';
    *number = atof(buf);
    if (i_old > 0 && !strchr("(+-*/^d", input[i_old - 1])) found = 0;
    *i = found ? (i_old + strlen(buf) - 1) : i_old;
  }
  return found;
}

int search_substr(char* input, char search_str[], size_t len_input_str,
                  size_t* i, size_t count_char_after) {
  size_t i_old = *i;
  int found = 0;
  size_t len_search_str = strlen(search_str);
  if (len_input_str - *i >= len_search_str + count_char_after)
    found = 1;  // for sin( must be +2, for mod +1
  if (found) {
    char buf[255];
    for (size_t j = 0; j < len_search_str; (*i)++, j++) {
      buf[j] = input[*i];
    }
    buf[len_search_str] = '\0';
    // puts(buf);
    if (strcmp(search_str, buf)) found = 0;
  }
  if (i_old > 0 && !strchr("(+-*/^", input[i_old - 1]) && input[i_old] != 'm')
    found = 0;
  if (input[i_old] == 'm' &&
      (i_old == 0 || (i_old > 0 && strchr("+-/*(^", input[i_old - 1]))))
    found = 0;
  if (!strcmp(search_str, "mod") && !strchr("-(0123456789", input[*i]))
    found = 0;
  *i = found ? (i_old + len_search_str - 1) : i_old;
  return found;
}

void remove_spaces(char* input) {
  char* skip_values = " \n\t";
  char cleaned_input[255];
  int j = 0;
  for (size_t i = 0; i < strlen(input); i++)
    if (!strchr(skip_values, input[i])) cleaned_input[j++] = input[i];
  cleaned_input[j] = '\0';

  for (size_t i = 0; i < strlen(cleaned_input); i++)
    input[i] = cleaned_input[i];
  input[strlen(cleaned_input)] = '\0';
}

void add_zero_before_minus(char* input, size_t minus_index) {
  char new_input[255];
  for (size_t i = 0; i < strlen(input); i++) {
    if (i == minus_index) {
      new_input[i] = '0';
      new_input[i + 1] = input[i];
    } else if (i < minus_index)
      new_input[i] = input[i];
    else
      new_input[i + 1] = input[i];
  }
  new_input[strlen(input) + 1] = '\0';
  for (size_t i = 0; i < strlen(input) + 1; i++) input[i] = new_input[i];
  input[strlen(input) + 1] = '\0';
}

void push(lexem_t** plist, double value, int priority, type_t type) {
  lexem_t* temp = malloc(sizeof(lexem_t));
  temp->value = value;
  temp->priority = priority;
  temp->type = type;
  temp->next = *plist;
  *plist = temp;
}

lexem_t pop(lexem_t** plist) {
  lexem_t res;
  if (plist) {
    lexem_t* p = *plist;
    res.priority = p->priority;
    res.type = p->type;
    res.value = p->value;
    (*plist) = p->next;
    free(p);
  } else
    res.type = NOTHING;
  return res;
}

lexem_t peek(lexem_t* plist) {
  lexem_t res;
  if (plist) {
    res.priority = plist->priority;
    res.type = plist->type;
    res.value = plist->value;
    res.next = plist->next;
  } else
    res.type = NOTHING;

  return res;
}

void reverse_stack(lexem_t** plist) {
  lexem_t* temp = NULL;
  while (*plist != NULL) {
    push(&temp, (*plist)->value, (*plist)->priority, (*plist)->type);
    pop(plist);
  }
  *plist = temp;
}

void delete_stack(lexem_t** plist) {
  while (*plist != NULL) {
    pop(plist);
  }
}
