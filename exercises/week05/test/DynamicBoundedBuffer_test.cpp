//@CMAKE_CUTE_DEPENDENCY=exercises/week05/src/bounded_buffer_content_suite.cpp
//@CMAKE_CUTE_DEPENDENCY=exercises/week05/src/bounded_buffer_default_behavior_suite.cpp
//@CMAKE_CUTE_DEPENDENCY=exercises/week05/src/bounded_buffer_heap_memory_suite.cpp
//@CMAKE_CUTE_DEPENDENCY=exercises/week05/src/bounded_buffer_semantic_suite.cpp
//@CMAKE_CUTE_DEPENDENCY=exercises/week05/src/bounded_buffer_signatures_suite.cpp
//@CMAKE_CUTE_DEPENDENCY=exercises/week05/src/bounded_buffer_student_suite.cpp
//@CMAKE_CUTE_DEPENDENCY=exercises/week05/src/bounded_buffer_iterator_suite.cpp
//@CMAKE_CUTE_DEPENDENCY=exercises/week05/src/bounded_buffer_non_default_constructible_element_type_suite.cpp

#include "bounded_buffer_signatures_suite.h"
#include "bounded_buffer_default_behavior_suite.h"
#include "bounded_buffer_content_suite.h"
#include "bounded_buffer_semantic_suite.h"
#include "bounded_buffer_student_suite.h"
#include "bounded_buffer_heap_memory_suite.h"
#include "bounded_buffer_iterator_suite.h"

#include <cute/cute.h>
#include <cute/ide_listener.h>
#include <cute/xml_listener.h>
#include <cute/cute_runner.h>

bool runAllTests(int argc, char const *argv[])
  {
  cute::xml_file_opener xmlfile{argc, argv};
  cute::xml_listener<cute::ide_listener<> > lis{xmlfile.out};

  bool good{true};

  good &= cute::makeRunner(lis,argc,argv)(make_suite_bounded_buffer_signatures_suite(), "BoundedBuffer Signature Tests");
  good &= cute::makeRunner(lis,argc,argv)(make_suite_bounded_buffer_default_behavior_suite(), "BoundedBuffer Default Behavior Tests");
  good &= cute::makeRunner(lis,argc,argv)(make_suite_bounded_buffer_content_suite(), "BoundedBuffer Content Tests");
  good &= cute::makeRunner(lis,argc,argv)(make_suite_bounded_buffer_semantic_suite(), "BoundedBuffer Semantic Tests");
  good &= cute::makeRunner(lis,argc,argv)(make_suite_bounded_buffer_student_suite(), "BoundedBuffer Student Tests");
  good &= cute::makeRunner(lis,argc,argv)(make_suite_bounded_buffer_heap_memory_suite(), "BoundedBuffer Heap Memory Tests");
  good &= cute::makeRunner(lis,argc,argv)(make_suite_bounded_buffer_iterator_suite(), "BoundedBuffer Iterator Tests");

  return good;
  }

int main(int argc, char const *argv[])
  {
  return !runAllTests(argc, argv);
  }

