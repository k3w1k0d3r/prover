project(Theorem128)
cmake_minimum_required(VERSION 3.20)
add_library(theorem128 SHARED src/cpp/function.cpp src/cpp/term.cpp src/cpp/predicate.cpp src/cpp/formula.cpp src/cpp/theory.cpp src/cpp/proof.cpp)
add_executable(main test/src/cpp/main.cpp)
set_target_properties(
	theorem128
	PROPERTIES
		PREFIX ""
		OUTPUT_NAME "lib/theorem128"
		LINKER_LANGUAGE C
)
set_target_properties(
	main
	PROPERTIES
		PREFIX ""
		OUTPUT_NAME "bin/main"
)
target_include_directories(theorem128 PUBLIC "${CMAKE_SOURCE_DIR}/include")
target_include_directories(main PUBLIC "${CMAKE_SOURCE_DIR}/include")
target_link_libraries(main theorem128)
set(CMAKE_CXX_FLAGS "-O3 -funroll-all-loops")
