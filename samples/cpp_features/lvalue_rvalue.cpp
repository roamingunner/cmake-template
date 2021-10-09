#include <iostream>
#include <complex>
#include <vector>
#include <string>

using namespace std;

class mstring
{
public:
    string data_;

    mstring(char *c_str):data_(c_str){
        cout << "concreate mstring\n";
    }
    mstring(const string& s): data_(s) {
        cout << "copy concreate mstring\n";
    }
    mstring(const mstring&& src) : data_(src.data_){
        cout << "move concreate mstring\n";
    }
    ~mstring() {
        cout << "destory mstring\n";
    }

};

// 判断调用哪一个函数
void fun1(string &str) {
    cout << "左值传入 " << str <<  endl;
}

void fun1(string &&str) {
    cout << "右值传入 " << str << endl;
}

void fun2(const string &str){
    cout << "左值传入 " << str <<  endl;
}

void fun3(string &str){
    cout << "左值传入 " << str <<  endl;
}

void test_move(){
    string str = "hello";
    vector<string> v1;
    v1.push_back(str);

    cout << "v[0]=" << v1[0] << ". str="<< str << endl;
    vector<string> v2;
    v2.push_back(std::move(str));
    cout << "v[0]=" << v2[0] << ". str="<< str << endl;

}

int main() {

    test_move();
    string s1 = "helloworld";
    fun1(s1);            // L
    fun1("helloworld");   // R
    fun1(move(s1));      // R
    cout << "s1=" << s1 << endl;

    string s2 = "look";
    fun2(std::move(s2));
    //fun3(std::move(s2)); error

    return 0;
}