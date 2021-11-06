#include <iostream>
#include <vector>
#include <initializer_list>
using namespace std;
class C
{
private:
    vector<int> vec;
public:
    C(int a, int b) {
        cout << "construct through two arguments\n";
        vec.push_back(a);
        vec.push_back(b);
    }
    C(initializer_list<int> args){
        cout << "construct through initializer_list\n";
        for (auto it=args.begin(); it!=args.end(); ++it){
            vec.push_back(*it);
        }
    }
    void dump(){
        cout << "vec=[";
        for (auto& item : vec){
            cout << item << ",";
        }
        cout << "]\n";
    }
    ~C() {}
};

void fun(int , int ){
    cout << "fun pass two argument\n";
    return ;
}

void fun(initializer_list<int> ){
    cout << "fun pass initializer_list\n";
    return ;
}



int main(int argc, char const *argv[])
{
    C c1(1,2);
    c1.dump();
    C c2{1,2};
    c2.dump();
    C c3({1,2});
    c3.dump();    
    C c4{1,2,3,4,5};
    c4.dump();

    fun(1,2);
    //fun(1,2,3,4); error: no matching function for call to ‘fun(int, int, int, int)’
    fun({1,2,3,4});


    return 0;
}
