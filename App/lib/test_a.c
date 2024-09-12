#include <stdio.h>

#include "test_a.h"
#include "test_b.h"

void test_func_a() {
  printf("test_func_a\n");
  test_func_b();
}
