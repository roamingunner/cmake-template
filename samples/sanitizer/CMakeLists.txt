# Fails with ThreadSanitizer
add_executable(tsanFail tsan_fail.cpp)
if(UNIX)
  target_link_libraries(tsanFail PUBLIC pthread)
endif()

# Fails with LeakSanitizer
add_executable(lsanFail lsan_fail.c)

# Fails with AddressSanitizer
add_executable(asanFail asan_fail.cpp)

# Fails with MemorySanitizer
add_executable(msanFail msan_fail.cpp)

# Fails with UndefinedBehaviourSanitizer
add_executable(ubsanFail ubsan_fail.cpp)

