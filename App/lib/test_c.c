#include <stdio.h>

#include "test_a.h"
#include "test_b.h"

void test_func_c() {
  printf("test_func_c\n");
  test_func_a();
}
