# Adding test executables.
list(APPEND tests
  main.cpp
  attach-simplex.cpp
    )

# Enabling testing.
include(CTest)

# Iterating through the list of tests, adding target for each test and linking it,
# and adding its argument-less invocation to CTest.
foreach(test ${tests})
    # Get name without extension.
    get_filename_component(out_name ${test} NAME_WE)

    add_executable(${out_name} ${test})
    target_link_libraries(${out_name} ${TRIANG_INCLUDE_LIBS} compiler_flags)

    # Add ./out_name to the list of tests performed by CTest, under the name TEST_out_name.
    add_test(NAME "TEST_${out_name}" COMMAND ${out_name})
endforeach()

# Custom tests come here
