#include <iostream>

// #include <string>
#include <sys/types.h>
#include <dirent.h>  // Get function opendir
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


template<typename Callback>
void iterate_directory(const std::string& path, Callback&& callback)
{
    DIR *dir;
    struct dirent *dp;

    dir = opendir(path.c_str()) ;

    // To determine the cause of error - It is necessary to check the error code.
    if (dir == nullptr) throw std::runtime_error("Error: Cannot read directory");

    while ((dp = readdir(dir)) != nullptr) 
    {
       if(path == "/")
            callback(dp->d_name);
       else 
            callback(path + "/" + dp->d_name);      
    };
    closedir(dir);
}

int main(int argc, char** argv)
{
     std::cout << " List directory contents " << std::endl;

     if(argc < 2)
     {
         std::cout << "Usage: ./unix-readdir <DIRECTORY>" << "\n";
         return EXIT_SUCCESS;
     }

     int idx = 0;
     iterate_directory(argv[1], [&idx](const std::string& path)
     {
         std::cout << "  [" << idx++  << "] path => " << path << "\n";
     });
     return EXIT_SUCCESS;
}
