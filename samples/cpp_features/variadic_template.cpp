#include <tuple>
#include <iostream>
#include <string>
#include <stdexcept>
using namespace std;



int main(int argc, char const *argv[])
{
    auto t1 = std::make_tuple ("test", 3.1, 14, 'y');
    cout << "size " << tuple_size<decltype(t1)>::value << endl;
    cout << "0." << std::get<0>(t1);
    cout << "1." << std::get<1>(t1);
    cout << "2." << std::get<2>(t1);
    cout << "3." << std::get<3>(t1);
    cout << endl;

    return 0;
}
