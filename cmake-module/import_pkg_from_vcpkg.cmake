# NOTE: this function definition is copied directly from scripts/cmake/z_vcpkg_function_arguments.cmake
# do not make changes here without making the same change there.
macro(z_vcpkg_function_arguments OUT_VAR)
    if("${ARGC}" EQUAL 1)
        set(z_vcpkg_function_arguments_FIRST_ARG 0)
    elseif("${ARGC}" EQUAL 2)
        set(z_vcpkg_function_arguments_FIRST_ARG "${ARGV1}")
    else()
        # vcpkg bug
        message(FATAL_ERROR "z_vcpkg_function_arguments: invalid arguments (${ARGV})")
    endif()

    set("${OUT_VAR}")

    # this allows us to get the value of the enclosing function's ARGC
    set(z_vcpkg_function_arguments_ARGC_NAME "ARGC")
    set(z_vcpkg_function_arguments_ARGC "${${z_vcpkg_function_arguments_ARGC_NAME}}")

    math(EXPR z_vcpkg_function_arguments_LAST_ARG "${z_vcpkg_function_arguments_ARGC} - 1")
    # GREATER_EQUAL added in CMake 3.7
    if(NOT z_vcpkg_function_arguments_LAST_ARG LESS z_vcpkg_function_arguments_FIRST_ARG)
        foreach(z_vcpkg_function_arguments_N RANGE "${z_vcpkg_function_arguments_FIRST_ARG}" "${z_vcpkg_function_arguments_LAST_ARG}")
            string(REPLACE ";" "\\;" z_vcpkg_function_arguments_ESCAPED_ARG "${ARGV${z_vcpkg_function_arguments_N}}")
            list(APPEND "${OUT_VAR}" "${z_vcpkg_function_arguments_ESCAPED_ARG}")
        endforeach()
    endif()
endmacro()

if(DEFINED VCPKG_INSTALLED_DIR)
    # do nothing
elseif(DEFINED _VCPKG_INSTALLED_DIR)
    set(VCPKG_INSTALLED_DIR "${_VCPKG_INSTALLED_DIR}")
elseif(VCPKG_MANIFEST_MODE)
    set(VCPKG_INSTALLED_DIR "${CMAKE_BINARY_DIR}/vcpkg_installed")
else()
    set(VCPKG_INSTALLED_DIR "${Z_VCPKG_ROOT_DIR}/installed")
endif()

set(VCPKG_INSTALLED_DIR "${VCPKG_INSTALLED_DIR}"
    CACHE PATH
    "The directory which contains the installed libraries for each triplet" FORCE)
set(_VCPKG_INSTALLED_DIR "${VCPKG_INSTALLED_DIR}"
    CACHE PATH
    "The directory which contains the installed libraries for each triplet" FORCE)

if(VCPKG_PREFER_SYSTEM_LIBS)
    set(Z_VCPKG_PATH_LIST_OP APPEND)
else()
    set(Z_VCPKG_PATH_LIST_OP PREPEND)
endif()

if(CMAKE_BUILD_TYPE MATCHES "^[Dd][Ee][Bb][Uu][Gg]$" OR NOT DEFINED CMAKE_BUILD_TYPE) #Debug build: Put Debug paths before Release paths.
    list(${Z_VCPKG_PATH_LIST_OP} CMAKE_PREFIX_PATH
        "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/debug"
        "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}"
    )
    list(${Z_VCPKG_PATH_LIST_OP} CMAKE_LIBRARY_PATH
        "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/debug/lib/manual-link"
        "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/manual-link"
    )
    list(${Z_VCPKG_PATH_LIST_OP} CMAKE_FIND_ROOT_PATH
        "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/debug"
        "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}"
    )
else() #Release build: Put Release paths before Debug paths. Debug Paths are required so that CMake generates correct info in autogenerated target files.
    list(${Z_VCPKG_PATH_LIST_OP} CMAKE_PREFIX_PATH
        "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}"
        "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/debug"
    )
    list(${Z_VCPKG_PATH_LIST_OP} CMAKE_LIBRARY_PATH
        "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/manual-link"
        "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/debug/lib/manual-link"
    )
    list(${Z_VCPKG_PATH_LIST_OP} CMAKE_FIND_ROOT_PATH
        "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}"
        "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/debug"
    )
endif()

# If one CMAKE_FIND_ROOT_PATH_MODE_* variables is set to ONLY, to  make sure that ${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}
# and ${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/debug are searched, it is not sufficient to just add them to CMAKE_FIND_ROOT_PATH,
# as CMAKE_FIND_ROOT_PATH specify "one or more directories to be prepended to all other search directories", so to make sure that
# the libraries are searched as they are, it is necessary to add "/" to the CMAKE_PREFIX_PATH
if(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE STREQUAL "ONLY" OR
   CMAKE_FIND_ROOT_PATH_MODE_LIBRARY STREQUAL "ONLY" OR
   CMAKE_FIND_ROOT_PATH_MODE_PACKAGE STREQUAL "ONLY")
   list(APPEND CMAKE_PREFIX_PATH "/")
endif()

set(VCPKG_CMAKE_FIND_ROOT_PATH "${CMAKE_FIND_ROOT_PATH}")

# CMAKE_EXECUTABLE_SUFFIX is not yet defined
if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
    set(Z_VCPKG_EXECUTABLE "${Z_VCPKG_ROOT_DIR}/vcpkg.exe")
    set(Z_VCPKG_BOOTSTRAP_SCRIPT "${Z_VCPKG_ROOT_DIR}/bootstrap-vcpkg.bat")
else()
    set(Z_VCPKG_EXECUTABLE "${Z_VCPKG_ROOT_DIR}/vcpkg")
    set(Z_VCPKG_BOOTSTRAP_SCRIPT "${Z_VCPKG_ROOT_DIR}/bootstrap-vcpkg.sh")
endif()

if(VCPKG_MANIFEST_MODE AND VCPKG_MANIFEST_INSTALL AND NOT Z_VCPKG_CMAKE_IN_TRY_COMPILE AND NOT Z_VCPKG_HAS_FATAL_ERROR)
    if(NOT EXISTS "${Z_VCPKG_EXECUTABLE}" AND NOT Z_VCPKG_HAS_FATAL_ERROR)
        message(STATUS "Bootstrapping vcpkg before install")

        file(TO_NATIVE_PATH "${CMAKE_BINARY_DIR}/vcpkg-bootstrap.log" Z_VCPKG_BOOTSTRAP_LOG)
        execute_process(
            COMMAND "${Z_VCPKG_BOOTSTRAP_SCRIPT}" ${VCPKG_BOOTSTRAP_OPTIONS}
            OUTPUT_FILE "${Z_VCPKG_BOOTSTRAP_LOG}"
            ERROR_FILE "${Z_VCPKG_BOOTSTRAP_LOG}"
            RESULT_VARIABLE Z_VCPKG_BOOTSTRAP_RESULT)

        if(Z_VCPKG_BOOTSTRAP_RESULT EQUAL 0)
            message(STATUS "Bootstrapping vcpkg before install - done")
        else()
            message(STATUS "Bootstrapping vcpkg before install - failed")
            z_vcpkg_add_fatal_error("vcpkg install failed. See logs for more information: ${Z_VCPKG_BOOTSTRAP_LOG}")
        endif()
    endif()

    if(NOT Z_VCPKG_HAS_FATAL_ERROR)
        message(STATUS "Running vcpkg install")

        set(Z_VCPKG_ADDITIONAL_MANIFEST_PARAMS)

        if(DEFINED VCPKG_HOST_TRIPLET AND NOT VCPKG_HOST_TRIPLET STREQUAL "")
            list(APPEND Z_VCPKG_ADDITIONAL_MANIFEST_PARAMS "--host-triplet=${VCPKG_HOST_TRIPLET}")
        endif()

        if(VCPKG_OVERLAY_PORTS)
            foreach(Z_VCPKG_OVERLAY_PORT IN LISTS VCPKG_OVERLAY_PORTS)
                list(APPEND Z_VCPKG_ADDITIONAL_MANIFEST_PARAMS "--overlay-ports=${Z_VCPKG_OVERLAY_PORT}")
            endforeach()
        endif()
        if(VCPKG_OVERLAY_TRIPLETS)
            foreach(Z_VCPKG_OVERLAY_TRIPLET IN LISTS VCPKG_OVERLAY_TRIPLETS)
                list(APPEND Z_VCPKG_ADDITIONAL_MANIFEST_PARAMS "--overlay-triplets=${Z_VCPKG_OVERLAY_TRIPLET}")
            endforeach()
        endif()

        if(DEFINED VCPKG_FEATURE_FLAGS OR DEFINED CACHE{VCPKG_FEATURE_FLAGS})
            list(JOIN VCPKG_FEATURE_FLAGS "," Z_VCPKG_FEATURE_FLAGS)
            set(Z_VCPKG_FEATURE_FLAGS "--feature-flags=${Z_VCPKG_FEATURE_FLAGS}")
        endif()

        foreach(Z_VCPKG_FEATURE IN LISTS VCPKG_MANIFEST_FEATURES)
            list(APPEND Z_VCPKG_ADDITIONAL_MANIFEST_PARAMS "--x-feature=${Z_VCPKG_FEATURE}")
        endforeach()

        if(VCPKG_MANIFEST_NO_DEFAULT_FEATURES)
            list(APPEND Z_VCPKG_ADDITIONAL_MANIFEST_PARAMS "--x-no-default-features")
        endif()

        if(NOT CMAKE_VERSION VERSION_LESS "3.18") # == GREATER_EQUAL, but that was added in CMake 3.7
            set(Z_VCPKG_MANIFEST_INSTALL_ECHO_PARAMS ECHO_OUTPUT_VARIABLE ECHO_ERROR_VARIABLE)
        else()
            set(Z_VCPKG_MANIFEST_INSTALL_ECHO_PARAMS)
        endif()

        execute_process(
            COMMAND "${Z_VCPKG_EXECUTABLE}" install
                --triplet "${VCPKG_TARGET_TRIPLET}"
                --vcpkg-root "${Z_VCPKG_ROOT_DIR}"
                "--x-wait-for-lock"
                "--x-manifest-root=${VCPKG_MANIFEST_DIR}"
                "--x-install-root=${_VCPKG_INSTALLED_DIR}"
                "${Z_VCPKG_FEATURE_FLAGS}"
                ${Z_VCPKG_ADDITIONAL_MANIFEST_PARAMS}
                ${VCPKG_INSTALL_OPTIONS}
            OUTPUT_VARIABLE Z_VCPKG_MANIFEST_INSTALL_LOGTEXT
            ERROR_VARIABLE Z_VCPKG_MANIFEST_INSTALL_LOGTEXT
            RESULT_VARIABLE Z_VCPKG_MANIFEST_INSTALL_RESULT
            ${Z_VCPKG_MANIFEST_INSTALL_ECHO_PARAMS}
        )

        file(TO_NATIVE_PATH "${CMAKE_BINARY_DIR}/vcpkg-manifest-install.log" Z_VCPKG_MANIFEST_INSTALL_LOGFILE)
        file(WRITE "${Z_VCPKG_MANIFEST_INSTALL_LOGFILE}" "${Z_VCPKG_MANIFEST_INSTALL_LOGTEXT}")

        if(Z_VCPKG_MANIFEST_INSTALL_RESULT EQUAL 0)
            message(STATUS "Running vcpkg install - done")

            # file(TOUCH) added in CMake 3.12
            file(WRITE "${_VCPKG_INSTALLED_DIR}/.cmakestamp" "")
            set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS
                "${VCPKG_MANIFEST_DIR}/vcpkg.json"
                "${_VCPKG_INSTALLED_DIR}/.cmakestamp")
            if(EXISTS "${VCPKG_MANIFEST_DIR}/vcpkg-configuration.json")
                set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS
                    "${VCPKG_MANIFEST_DIR}/vcpkg-configuration.json")
            endif()
        else()
            message(STATUS "Running vcpkg install - failed")
            z_vcpkg_add_fatal_error("vcpkg install failed. See logs for more information: ${Z_VCPKG_MANIFEST_INSTALL_LOGFILE}")
        endif()
    endif()
endif()

list(APPEND CMAKE_PROGRAM_PATH "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/tools")
file(GLOB Z_VCPKG_TOOLS_DIRS "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/tools/*")
foreach(Z_VCPKG_TOOLS_DIR IN LISTS Z_VCPKG_TOOLS_DIRS)
    if(IS_DIRECTORY "${Z_VCPKG_TOOLS_DIR}")
        list(APPEND CMAKE_PROGRAM_PATH "${Z_VCPKG_TOOLS_DIR}")
    endif()
endforeach()

function(add_executable)
    z_vcpkg_function_arguments(ARGS)
    _add_executable(${ARGS})
    set(target_name "${ARGV0}")

    list(FIND ARGV "IMPORTED" IMPORTED_IDX)
    list(FIND ARGV "ALIAS" ALIAS_IDX)
    list(FIND ARGV "MACOSX_BUNDLE" MACOSX_BUNDLE_IDX)
    if(IMPORTED_IDX EQUAL -1 AND ALIAS_IDX EQUAL -1)
        if(VCPKG_APPLOCAL_DEPS)
            if(Z_VCPKG_TARGET_TRIPLET_PLAT MATCHES "windows|uwp")
                z_vcpkg_set_powershell_path()
                set(EXTRA_OPTIONS "")
                if(X_VCPKG_APPLOCAL_DEPS_SERIALIZED)
                    set(EXTRA_OPTIONS USES_TERMINAL)
                endif()
                add_custom_command(TARGET "${target_name}" POST_BUILD
                    COMMAND "${Z_VCPKG_POWERSHELL_PATH}" -noprofile -executionpolicy Bypass -file "${Z_VCPKG_TOOLCHAIN_DIR}/msbuild/applocal.ps1"
                        -targetBinary "$<TARGET_FILE:${target_name}>"
                        -installedDir "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}$<$<CONFIG:Debug>:/debug>/bin"
                        -OutVariable out
                    ${EXTRA_OPTIONS}
                )
            elseif(Z_VCPKG_TARGET_TRIPLET_PLAT MATCHES "osx")
                if(NOT MACOSX_BUNDLE_IDX EQUAL -1)
                    add_custom_command(TARGET "${target_name}" POST_BUILD
                    COMMAND python "${Z_VCPKG_TOOLCHAIN_DIR}/osx/applocal.py"
                        "$<TARGET_FILE:${target_name}>"
                        "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}$<$<CONFIG:Debug>:/debug>"
                    )
                endif()
            endif()
        endif()
        set_target_properties("${target_name}" PROPERTIES VS_USER_PROPS do_not_import_user.props)
        set_target_properties("${target_name}" PROPERTIES VS_GLOBAL_VcpkgEnabled false)
    endif()
endfunction()

function(add_library)
    z_vcpkg_function_arguments(ARGS)
    _add_library(${ARGS})
    set(target_name "${ARGV0}")

    list(FIND ARGS "IMPORTED" IMPORTED_IDX)
    list(FIND ARGS "INTERFACE" INTERFACE_IDX)
    list(FIND ARGS "ALIAS" ALIAS_IDX)
    if(IMPORTED_IDX EQUAL -1 AND INTERFACE_IDX EQUAL -1 AND ALIAS_IDX EQUAL -1)
        get_target_property(IS_LIBRARY_SHARED "${target_name}" TYPE)
        if(VCPKG_APPLOCAL_DEPS AND Z_VCPKG_TARGET_TRIPLET_PLAT MATCHES "windows|uwp" AND (IS_LIBRARY_SHARED STREQUAL "SHARED_LIBRARY" OR IS_LIBRARY_SHARED STREQUAL "MODULE_LIBRARY"))
            z_vcpkg_set_powershell_path()
            add_custom_command(TARGET "${target_name}" POST_BUILD
                COMMAND "${Z_VCPKG_POWERSHELL_PATH}" -noprofile -executionpolicy Bypass -file "${Z_VCPKG_TOOLCHAIN_DIR}/msbuild/applocal.ps1"
                    -targetBinary "$<TARGET_FILE:${target_name}>"
                    -installedDir "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}$<$<CONFIG:Debug>:/debug>/bin"
                    -OutVariable out
            )
        endif()
        set_target_properties("${target_name}" PROPERTIES VS_USER_PROPS do_not_import_user.props)
        set_target_properties("${target_name}" PROPERTIES VS_GLOBAL_VcpkgEnabled false)
    endif()
endfunction()

# This is an experimental function to enable applocal install of dependencies as part of the `make install` process
# Arguments:
#   TARGETS - a list of installed targets to have dependencies copied for
#   DESTINATION - the runtime directory for those targets (usually `bin`)
#   COMPONENT - the component this install command belongs to (optional)
#
# Note that this function requires CMake 3.14 for policy CMP0087
function(x_vcpkg_install_local_dependencies)
    if(Z_VCPKG_TARGET_TRIPLET_PLAT MATCHES "windows|uwp")
        cmake_parse_arguments(PARSE_ARGV 0 __VCPKG_APPINSTALL "" "DESTINATION;COMPONENT" "TARGETS")
        z_vcpkg_set_powershell_path()
        if(NOT IS_ABSOLUTE "${__VCPKG_APPINSTALL_DESTINATION}")
            set(__VCPKG_APPINSTALL_DESTINATION "\${CMAKE_INSTALL_PREFIX}/${__VCPKG_APPINSTALL_DESTINATION}")
        endif()
        if(__VCPKG_APPINSTALL_COMPONENT)
            set(__VCPKG_APPINSTALL_COMPONENT COMPONENT ${__VCPKG_APPINSTALL_COMPONENT})
        endif()
        foreach(TARGET IN LISTS __VCPKG_APPINSTALL_TARGETS)
            get_target_property(TARGETTYPE "${TARGET}" TYPE)
            if(NOT TARGETTYPE STREQUAL "INTERFACE_LIBRARY")
                # Install CODE|SCRIPT allow the use of generator expressions
                if(POLICY CMP0087)
                    cmake_policy(SET CMP0087 NEW)
                endif()
                install(CODE "message(\"-- Installing app dependencies for ${TARGET}...\")
                    execute_process(COMMAND \"${Z_VCPKG_POWERSHELL_PATH}\" -noprofile -executionpolicy Bypass -file \"${Z_VCPKG_TOOLCHAIN_DIR}/msbuild/applocal.ps1\"
                        -targetBinary \"${__VCPKG_APPINSTALL_DESTINATION}/$<TARGET_FILE_NAME:${TARGET}>\"
                        -installedDir \"${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}$<$<CONFIG:Debug>:/debug>/bin\"
                        -OutVariable out)"
                    ${__VCPKG_APPINSTALL_COMPONENT}
                    )
            endif()
        endforeach()
    endif()
endfunction()

if(X_VCPKG_APPLOCAL_DEPS_INSTALL)
    function(install)
        z_vcpkg_function_arguments(ARGS)
        _install(${ARGS})

        if(ARGV0 STREQUAL "TARGETS")
            # Will contain the list of targets
            set(PARSED_TARGETS "")

            # Destination - [RUNTIME] DESTINATION argument overrides this
            set(DESTINATION "bin")

            # Parse arguments given to the install function to find targets and (runtime) destination
            set(MODIFIER "") # Modifier for the command in the argument
            set(LAST_COMMAND "") # Last command we found to process
            foreach(ARG IN LISTS ARGS)
                if(ARG MATCHES "ARCHIVE|LIBRARY|RUNTIME|OBJECTS|FRAMEWORK|BUNDLE|PRIVATE_HEADER|PUBLIC_HEADER|RESOURCE|INCLUDES")
                    set(MODIFIER "${ARG}")
                    continue()
                endif()
                if(ARG MATCHES "TARGETS|DESTINATION|PERMISSIONS|CONFIGURATIONS|COMPONENT|NAMELINK_COMPONENT|OPTIONAL|EXCLUDE_FROM_ALL|NAMELINK_ONLY|NAMELINK_SKIP|EXPORT")
                    set(LAST_COMMAND "${ARG}")
                    continue()
                endif()

                if(LAST_COMMAND STREQUAL "TARGETS")
                    list(APPEND PARSED_TARGETS "${ARG}")
                endif()

                if(LAST_COMMAND STREQUAL "DESTINATION" AND (MODIFIER STREQUAL "" OR MODIFIER STREQUAL "RUNTIME"))
                    set(DESTINATION "${ARG}")
                endif()
                if(LAST_COMMAND STREQUAL "COMPONENT")
                    set(COMPONENT "${ARG}")
                endif()
            endforeach()

            # COMPONENT is optional only set it when it's been set by the install rule
            if(COMPONENT)
                set(COMPONENT "COMPONENT" ${COMPONENT})
            endif()
            x_vcpkg_install_local_dependencies(TARGETS "${PARSED_TARGETS}" DESTINATION "${DESTINATION}" ${COMPONENT})
        endif()
    endfunction()
endif()

if(NOT DEFINED VCPKG_OVERRIDE_FIND_PACKAGE_NAME)
    set(VCPKG_OVERRIDE_FIND_PACKAGE_NAME find_package)
endif()
# NOTE: this is not a function, which means that arguments _are not_ perfectly forwarded
# this is fine for `find_package`, since there are no usecases for `;` in arguments,
# so perfect forwarding is not important
macro("${VCPKG_OVERRIDE_FIND_PACKAGE_NAME}" z_vcpkg_find_package_package_name)
    set(z_vcpkg_find_package_package_name "${z_vcpkg_find_package_package_name}")
    set(z_vcpkg_find_package_ARGN "${ARGN}")
    set(z_vcpkg_find_package_backup_vars)

    # Workaround to set the ROOT_PATH until upstream CMake stops overriding
    # the ROOT_PATH at apple OS initialization phase.
    # See https://gitlab.kitware.com/cmake/cmake/merge_requests/3273
    # Fixed in CMake 3.15
    if(CMAKE_SYSTEM_NAME STREQUAL iOS)
        list(APPEND z_vcpkg_find_package_backup_vars "CMAKE_FIND_ROOT_PATH")
        if(DEFINED CMAKE_FIND_ROOT_PATH)
            set(z_vcpkg_find_package_backup_CMAKE_FIND_ROOT_PATH "${CMAKE_FIND_ROOT_PATH}")
        else()
            set(z_vcpkg_find_package_backup_CMAKE_FIND_ROOT_PATH)
        endif()

        list(APPEND CMAKE_FIND_ROOT_PATH "${VCPKG_CMAKE_FIND_ROOT_PATH}")
    endif()
    string(TOLOWER "${z_vcpkg_find_package_package_name}" z_vcpkg_find_package_lowercase_package_name)

    set(z_vcpkg_find_package_vcpkg_cmake_wrapper_path
        "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/share/${z_vcpkg_find_package_lowercase_package_name}/vcpkg-cmake-wrapper.cmake")

    if(EXISTS "${z_vcpkg_find_package_vcpkg_cmake_wrapper_path}")
        list(APPEND z_vcpkg_find_package_backup_vars "ARGS")
        if(DEFINED ARGS)
            set(z_vcpkg_find_package_backup_ARGS "${ARGS}")
        else()
            set(z_vcpkg_find_package_backup_ARGS)
        endif()

        set(ARGS "${z_vcpkg_find_package_package_name};${z_vcpkg_find_package_ARGN}")
        include("${z_vcpkg_find_package_vcpkg_cmake_wrapper_path}")
    elseif(z_vcpkg_find_package_package_name STREQUAL "Boost" AND EXISTS "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include/boost")
        # Checking for the boost headers disables this wrapper unless the user has installed at least one boost library
        # these intentionally are not backed up
        set(Boost_USE_STATIC_LIBS OFF)
        set(Boost_USE_MULTITHREADED ON)
        set(Boost_NO_BOOST_CMAKE ON)
        set(Boost_USE_STATIC_RUNTIME)
        unset(Boost_USE_STATIC_RUNTIME CACHE)
        if(CMAKE_VS_PLATFORM_TOOLSET STREQUAL "v120")
            set(Boost_COMPILER "-vc120")
        else()
            set(Boost_COMPILER "-vc140")
        endif()
        _find_package("${z_vcpkg_find_package_package_name}" ${z_vcpkg_find_package_ARGN})
    elseif(z_vcpkg_find_package_package_name STREQUAL "ICU" AND EXISTS "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include/unicode/utf.h")
        list(FIND z_vcpkg_find_package_ARGN "COMPONENTS" z_vcpkg_find_package_COMPONENTS_IDX)
        if(NOT z_vcpkg_find_package_COMPONENTS_IDX EQUAL -1)
            _find_package("${z_vcpkg_find_package_package_name}" ${z_vcpkg_find_package_ARGN} COMPONENTS data)
        else()
            _find_package("${z_vcpkg_find_package_package_name}" ${z_vcpkg_find_package_ARGN})
        endif()
    elseif(z_vcpkg_find_package_package_name STREQUAL "GSL" AND EXISTS "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include/gsl")
        _find_package("${z_vcpkg_find_package_package_name}" ${z_vcpkg_find_package_ARGN})
        if(GSL_FOUND AND TARGET GSL::gsl)
            set_property( TARGET GSL::gslcblas APPEND PROPERTY IMPORTED_CONFIGURATIONS Release )
            set_property( TARGET GSL::gsl APPEND PROPERTY IMPORTED_CONFIGURATIONS Release )
            if( EXISTS "${GSL_LIBRARY_DEBUG}" AND EXISTS "${GSL_CBLAS_LIBRARY_DEBUG}")
                set_property( TARGET GSL::gsl APPEND PROPERTY IMPORTED_CONFIGURATIONS Debug )
                set_target_properties( GSL::gsl PROPERTIES IMPORTED_LOCATION_DEBUG "${GSL_LIBRARY_DEBUG}" )
                set_property( TARGET GSL::gslcblas APPEND PROPERTY IMPORTED_CONFIGURATIONS Debug )
                set_target_properties( GSL::gslcblas PROPERTIES IMPORTED_LOCATION_DEBUG "${GSL_CBLAS_LIBRARY_DEBUG}" )
            endif()
        endif()
    elseif("${z_vcpkg_find_package_package_name}" STREQUAL "CURL" AND EXISTS "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include/curl")
        _find_package("${z_vcpkg_find_package_package_name}" ${z_vcpkg_find_package_ARGN})
        if(CURL_FOUND)
            if(EXISTS "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/nghttp2.lib")
                list(APPEND CURL_LIBRARIES
                    "debug" "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/debug/lib/nghttp2.lib"
                    "optimized" "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/nghttp2.lib")
            endif()
        endif()
    elseif("${z_vcpkg_find_package_lowercase_package_name}" STREQUAL "grpc" AND EXISTS "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/share/grpc")
        _find_package(gRPC ${z_vcpkg_find_package_ARGN})
    else()
        _find_package("${z_vcpkg_find_package_package_name}" ${z_vcpkg_find_package_ARGN})
    endif()

    foreach(z_vcpkg_find_package_backup_var IN LISTS z_vcpkg_find_package_backup_vars)
        if(DEFINED z_vcpkg_find_package_backup_${z_vcpkg_find_package_backup_var})
            set("${z_vcpkg_find_package_backup_var}" "${z_vcpkg_find_package_backup_${z_vcpkg_find_package_backup_var}}")
        else()
            set("${z_vcpkg_find_package_backup_var}")
        endif()
    endforeach()
endmacro()

set(VCPKG_TOOLCHAIN ON)
set(Z_VCPKG_UNUSED "${CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION}")
set(Z_VCPKG_UNUSED "${CMAKE_EXPORT_NO_PACKAGE_REGISTRY}")
set(Z_VCPKG_UNUSED "${CMAKE_FIND_PACKAGE_NO_PACKAGE_REGISTRY}")
set(Z_VCPKG_UNUSED "${CMAKE_FIND_PACKAGE_NO_SYSTEM_PACKAGE_REGISTRY}")
set(Z_VCPKG_UNUSED "${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_SKIP}")

# Propogate these values to try-compile configurations so the triplet and toolchain load
if(NOT Z_VCPKG_CMAKE_IN_TRY_COMPILE)
    if(Z_VCPKG_CMAKE_EMULATE_TRY_COMPILE_PLATFORM_VARIABLES)
        file(TO_CMAKE_PATH "${VCPKG_CHAINLOAD_TOOLCHAIN_FILE}" Z_VCPKG_CHAINLOAD_FILE_CMAKE)
        file(TO_CMAKE_PATH "${Z_VCPKG_ROOT_DIR}" Z_VCPKG_ROOT_DIR_CMAKE)
        file(WRITE "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/vcpkg.config.cmake"
            "set(VCPKG_TARGET_TRIPLET \"${VCPKG_TARGET_TRIPLET}\" CACHE STRING \"\")\n"
            "set(VCPKG_TARGET_ARCHITECTURE \"${VCPKG_TARGET_ARCHITECTURE}\" CACHE STRING \"\")\n"
            "set(VCPKG_APPLOCAL_DEPS \"${VCPKG_APPLOCAL_DEPS}\" CACHE STRING \"\")\n"
            "set(VCPKG_CHAINLOAD_TOOLCHAIN_FILE \"${Z_VCPKG_CHAINLOAD_FILE_CMAKE}\" CACHE STRING \"\")\n"
            "set(Z_VCPKG_ROOT_DIR \"${Z_VCPKG_ROOT_DIR_CMAKE}\" CACHE STRING \"\")\n"
        )
    else()
        list(APPEND CMAKE_TRY_COMPILE_PLATFORM_VARIABLES
            VCPKG_TARGET_TRIPLET
            VCPKG_TARGET_ARCHITECTURE
            VCPKG_APPLOCAL_DEPS
            VCPKG_CHAINLOAD_TOOLCHAIN_FILE
            Z_VCPKG_ROOT_DIR
        )
    endif()
endif()

if(Z_VCPKG_HAS_FATAL_ERROR)
    message(FATAL_ERROR "${Z_VCPKG_FATAL_ERROR}")
endif()
