#include <stdlib.h>
#include "integer.h"

struct integer {
  int value;
};

struct integer* integer_create(int value) {
  struct integer* i = malloc(sizeof(*i));
  i->value = value;
  return i;
}

int integer_value(struct integer* integer) {
  return integer->value;
}
