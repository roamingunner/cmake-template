# The library
add_library(coverage.lib coverage.cpp)
# The executable
add_executable(coverage.main coverage.main.cpp)
target_link_libraries(coverage.main PUBLIC coverage.lib)

#Adds instrumentation to particular targets
## Instruments the library
target_code_coverage(coverage.lib AUTO)
## Adds the executable to the 'ccov' target
target_code_coverage(coverage.main AUTO)
