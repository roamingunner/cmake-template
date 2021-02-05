#include <iostream>

int main(){
        #if __cplusplus==201500L
#warning support C++17
        std::cout << "C++17" << std::endl;
        #elif __cplusplus==201402L
#warning support C++14
        std::cout << "C++14" << std::endl;
        #elif __cplusplus==201103L
#warning support C++11
        std::cout << "C++11" << std::endl;
        #elif __cplusplus==199711L
#warning support C++98
        std::cout << "C++98" << std::endl;
        #else
#warning support C++
        std::cout << "C++" << std::endl;
        #endif

        return 0;
}
