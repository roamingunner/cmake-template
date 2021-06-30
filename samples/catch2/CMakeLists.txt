find_path(CATCH2_INCLUDE_DIRS "catch.hpp")
# Some one-offs first:
# 1) Tests and main in one file
add_executable( 010-TestCase
  010-TestCase.cpp
)
target_include_directories(010-TestCase PRIVATE ${CATCH2_INCLUDE_DIRS})

# 2) Tests and main across two files
add_executable( 020-MultiFile
  020-TestCase-1.cpp
  020-TestCase-2.cpp
)
target_include_directories(020-MultiFile PRIVATE ${CATCH2_INCLUDE_DIRS})

#add_executable(231-Cfg_OutputStreams
#    231-Cfg-OutputStreams.cpp
#)
#target_link_libraries(231-Cfg_OutputStreams Catch2_buildall_interface)
#target_compile_definitions(231-Cfg_OutputStreams PUBLIC CATCH_CONFIG_NOSTDOUT)

# These examples use the standard separate compilation
set( SOURCES_IDIOMATIC_EXAMPLES
    030-Asn-Require-Check.cpp
    100-Fix-Section.cpp
    110-Fix-ClassFixture.cpp
    120-Bdd-ScenarioGivenWhenThen.cpp
    210-Evt-EventListeners.cpp
    300-Gen-OwnGenerator.cpp
    301-Gen-MapTypeConversion.cpp
    302-Gen-Table.cpp
    310-Gen-VariablesInGenerators.cpp
    311-Gen-CustomCapture.cpp
)

string( REPLACE ".cpp" "" BASENAMES_IDIOMATIC_EXAMPLES "${SOURCES_IDIOMATIC_EXAMPLES}" )
set( TARGETS_IDIOMATIC_EXAMPLES ${BASENAMES_IDIOMATIC_EXAMPLES} )


foreach( name ${TARGETS_IDIOMATIC_EXAMPLES} )
    add_executable(${name} ${name}.cpp 000-CatchMain.cpp)
    target_include_directories(${name} PRIVATE ${CATCH2_INCLUDE_DIRS})
endforeach()
