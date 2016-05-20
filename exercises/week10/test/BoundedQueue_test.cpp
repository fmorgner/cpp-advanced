//@CMAKE_CUTE_DEPENDENCY=exercises/week10/src/bounded_queue_content_suite.cpp
//@CMAKE_CUTE_DEPENDENCY=exercises/week10/src/bounded_queue_default_behavior_suite.cpp
//@CMAKE_CUTE_DEPENDENCY=exercises/week10/src/bounded_queue_heap_memory_suite.cpp
//@CMAKE_CUTE_DEPENDENCY=exercises/week10/src/bounded_queue_multi_threaded_suite.cpp
//@CMAKE_CUTE_DEPENDENCY=exercises/week10/src/bounded_queue_non_default_constructible_element_type_suite.cpp
//@CMAKE_CUTE_DEPENDENCY=exercises/week10/src/bounded_queue_semantic_suite.cpp
//@CMAKE_CUTE_DEPENDENCY=exercises/week10/src/bounded_queue_signatures_suite.cpp
//@CMAKE_CUTE_DEPENDENCY=exercises/week10/src/bounded_queue_single_threaded_lock_suite.cpp
//@CMAKE_CUTE_DEPENDENCY=exercises/week10/src/bounded_queue_student_suite.cpp

#include "bounded_queue_signatures_suite.h"
#include "bounded_queue_default_behavior_suite.h"
#include "bounded_queue_content_suite.h"
#include "bounded_queue_semantic_suite.h"
#include "bounded_queue_student_suite.h"
#include "bounded_queue_heap_memory_suite.h"
#include "bounded_queue_non_default_constructible_element_type_suite.h"
#include "bounded_queue_single_threaded_lock_suite.h"
#include "bounded_queue_multi_threaded_suite.h"

#include <cute/cute.h>
#include <cute/ide_listener.h>
#include <cute/xml_listener.h>
#include <cute/cute_runner.h>

#include <iostream>
#include <stdexcept>

auto runAllTests(int argc, char const *argv[]){
	//TODO add your test here
	cute::xml_file_opener xmlfile(argc,argv);
	cute::xml_listener<cute::ide_listener<> >  lis(xmlfile.out);

  bool success{true};

	success &= cute::makeRunner(lis,argc,argv)(make_suite_bounded_queue_signatures_suite(), "BoundedQueue API Tests");
	success &= cute::makeRunner(lis,argc,argv)(make_suite_bounded_queue_default_behavior_suite(), "BoundedQueue Default Behavior Tests");
	success &= cute::makeRunner(lis,argc,argv)(make_suite_bounded_queue_content_suite(), "BoundedQueue Content Tests");
	success &= cute::makeRunner(lis,argc,argv)(make_suite_bounded_queue_semantic_suite(), "BoundedQueue Semantic Tests");
	success &= cute::makeRunner(lis,argc,argv)(make_suite_bounded_queue_student_suite(), "BoundedQueue Student Tests");
	success &= cute::makeRunner(lis,argc,argv)(make_suite_bounded_queue_heap_memory_suite(), "BoundedQueue Heap Memory Tests");
	success &= cute::makeRunner(lis,argc,argv)(make_suite_bounded_queue_non_default_constructible_element_type_suite(), "BoundedQueue Non-Default-Constructible Element Type Tests");
	success &= cute::makeRunner(lis,argc,argv)(make_suite_bounded_queue_single_threaded_lock_suite(), "BoundedQueue Single Threaded Lock Tests");
	success &= cute::makeRunner(lis,argc,argv)(make_suite_bounded_queue_multi_threaded_suite(), "BoundedQueue Multi-Threaded Tests");

  return success;
}

int main(int argc, char const *argv[]){
    return !runAllTests(argc,argv);
  }



