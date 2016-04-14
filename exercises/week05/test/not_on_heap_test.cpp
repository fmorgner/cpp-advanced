#include "not_on_heap.h"

#include <cute/cute.h>
#include <cute/ide_listener.h>
#include <cute/cute_runner.h>

#include <vector>

void test_stack_allocation()
  {
  auto o = not_on_heap{};
  ASSERT(&o);
  }

void test_allocate_using_new()
  {
  ASSERT_THROWS(new not_on_heap{}, std::bad_alloc);
  }

void test_allocate_using_array_new()
  {
  ASSERT_THROWS(new not_on_heap[1]{}, std::bad_alloc);
  }

void test_should_be_valid_type_for_vector()
  {
  auto v = std::vector<not_on_heap>{};
  ASSERT(&v);
  }

int main(int argc, char * * argv)
  {
  auto suite = cute::suite{};

  suite += cute::test{"Should be allocatable on the stack", test_stack_allocation};
  suite += cute::test{"Should not be allocatable on the heap", test_allocate_using_new};
  suite += cute::test{"Should not be array allocatable on the heap", test_allocate_using_array_new};
  suite += cute::test{"Should be a valid type argument for std::vector", test_should_be_valid_type_for_vector};

  auto listener = cute::ide_listener<>{};
  auto runner = cute::makeRunner(listener, argc, argv);

  return !runner(suite);
  }

