#include <stdio.h>

#include "test_a.h"
#include "test_b.h"

void test_func_b() {
  printf("test_func_b\n");
  test_func_c();
}
