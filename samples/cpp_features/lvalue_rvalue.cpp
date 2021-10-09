#include <iostream>
#include <complex>
#include <vector>
#include <string>

using namespace std;


void test_move(){
    cout << "\n===========" << __func__ << "===========\n";
    string str = "hello";
    vector<string> v1;
    v1.push_back(str);

    cout << "v[0]=" << v1[0] << ". str="<< str << endl;
    vector<string> v2;
    v2.push_back(std::move(str));
    cout << "v[0]=" << v2[0] << ". str="<< str << endl;
}

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



void test_rl_parament(void){
    cout << "\n===========" << __func__ << "===========\n";
    string s1 = "helloworld";
    fun1(s1);            // L
    fun1("helloworld");   // R
    fun1(move(s1));      // R
    cout << "s1=" << s1 << endl;

    string s2 = "look";
    fun2(std::move(s2));
    //fun3(std::move(s2)); error
}



class mstring
{
public:
    string data_;

    mstring(const char *c_str):data_(c_str){
        cout << "concreate mstring\n";
    }
    mstring(const mstring& s): data_(s.data_) {
        cout << "copy concreate mstring\n";
    }
    mstring(const mstring&& src) : data_(std::move(src.data_)){
        cout << "move concreate mstring\n";
    }
    ~mstring() {
        cout << "destory mstring\n";
    }

    void dump(const char *tag){
        cout << tag << " " << data_ << endl;
    }

};

void test_constructor(void){
    cout << "\n===========" << __func__ << "===========\n";
    mstring ms1("AAAA");
    mstring ms2(ms1);
    mstring ms3(std::move(ms1));
    ms1.dump("ms1");
    ms2.dump("ms2");
    ms3.dump("ms3");
}

int main() {

    test_move();
    test_rl_parament();
    test_constructor();

/* output
===========test_move===========
v[0]=hello. str=hello
v[0]=hello. str=

===========test_rl_parament===========
左值传入 helloworld
右值传入 helloworld
右值传入 helloworld
s1=helloworld
左值传入 look

===========test_constructor===========
concreate mstring
copy concreate mstring
move concreate mstring
ms1 AAAA
ms2 AAAA
ms3 AAAA
destory mstring
destory mstring
destory mstring


*/
    return 0;
}

