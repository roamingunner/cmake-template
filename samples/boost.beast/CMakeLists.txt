add_executable(flat_buffer flat_buffer.cpp)
#add_executable(multi_buffer multi_buffer.cpp)
add_executable(http_downloader http_downloader.cpp LUrlParser/LUrlParser.cpp)
target_link_libraries(http_downloader PRIVATE pthread)
add_executable(http_file_service http_file_service.cpp)
target_link_libraries(http_file_service PRIVATE pthread)


