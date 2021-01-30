add_executable(tcp_async_server tcp_async_server.cpp)
include_directories(${PROJECT_SOURCE_DIR}/external/aixlog/include ${PROJECT_SOURCE_DIR})
ADD_DEPENDENCIES(tcp_async_server autorevision)
target_link_libraries(tcp_async_server pthread)

add_executable(tcp_async_multi_thread_server tcp_async_multi_thread_server.cpp)
include_directories(${PROJECT_SOURCE_DIR}/external/aixlog/include ${PROJECT_SOURCE_DIR})
ADD_DEPENDENCIES(tcp_async_multi_thread_server autorevision)
target_link_libraries(tcp_async_multi_thread_server pthread)

add_executable(tcp_sync_client tcp_sync_client.cpp)
include_directories(${PROJECT_SOURCE_DIR}/external/aixlog/include ${PROJECT_SOURCE_DIR})
ADD_DEPENDENCIES(tcp_sync_client autorevision)
target_link_libraries(tcp_sync_client pthread)

add_executable(tcp_sync_multi_client tcp_sync_multi_client.cpp)
include_directories(${PROJECT_SOURCE_DIR}/external/aixlog/include ${PROJECT_SOURCE_DIR})
ADD_DEPENDENCIES(tcp_sync_multi_client autorevision)
target_link_libraries(tcp_sync_multi_client pthread)