add_executable(memleak memleak.c)
add_cppcheck(memleak UNUSED_FUNCTIONS STYLE POSSIBLE_ERROR FORCE FAIL_ON_WARNINGS)

add_executable(member_invalid member_invalid.cpp)
add_cppcheck(member_invalid UNUSED_FUNCTIONS STYLE POSSIBLE_ERROR FORCE FAIL_ON_WARNINGS)

add_executable(outofbound outofbound.c)
add_executable(resource_leak resource_leak.c)
add_cppcheck_sources(custom_check UNUSED_FUNCTIONS STYLE POSSIBLE_ERROR FORCE FAIL_ON_WARNINGS outofbound.c resource_leak.c)

