//@CMAKE_CUTE_DEPENDENCY=exercises/week03/src/bounded_buffer_content_suite.cpp
//@CMAKE_CUTE_DEPENDENCY=exercises/week03/src/bounded_buffer_default_behavior_suite.cpp
//@CMAKE_CUTE_DEPENDENCY=exercises/week03/src/bounded_buffer_semantic_suite.cpp
//@CMAKE_CUTE_DEPENDENCY=exercises/week03/src/bounded_buffer_signatures_suite.cpp
//@CMAKE_CUTE_DEPENDENCY=exercises/week03/src/bounded_buffer_student_suite.cpp

#include <cute/cute.h>
#include <cute/ide_listener.h>
#include <cute/xml_listener.h>
#include <cute/cute_runner.h>

#include "bounded_buffer_signatures_suite.h"
#include "bounded_buffer_default_behavior_suite.h"
#include "bounded_buffer_content_suite.h"
#include "bounded_buffer_semantic_suite.h"
#include "bounded_buffer_student_suite.h"

bool runAllTests(int argc, char const *argv[]) {
	cute::xml_file_opener xmlfile(argc, argv);
	cute::xml_listener<cute::ide_listener<> > lis(xmlfile.out);

  bool good{true};

	good &= cute::makeRunner(lis,argc,argv)(make_suite_bounded_buffer_signatures_suite(), "BoundedBuffer Signature Tests");
	good &= cute::makeRunner(lis,argc,argv)(make_suite_bounded_buffer_default_behavior_suite(), "BoundedBuffer Default Behavior Tests");
	good &= cute::makeRunner(lis,argc,argv)(make_suite_bounded_buffer_content_suite(), "BoundedBuffer Content Tests");
	good &= cute::makeRunner(lis,argc,argv)(make_suite_bounded_buffer_semantic_suite(), "BoundedBuffer Semantic Tests");
	good &= cute::makeRunner(lis,argc,argv)(make_suite_bounded_buffer_student_suite(), "BoundedBuffer Student Tests");

  return good;
}

int main(int argc, char const *argv[]) {
	return !runAllTests(argc, argv);
}

