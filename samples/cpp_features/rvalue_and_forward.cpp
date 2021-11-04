#include <iostream>
#include <complex>
#include <vector>
#include <string>
#include <utility>
using namespace std;


void process(string& str){
    cout << "process got lvalue reference\n";
}

void process(const string& str){
    cout << "process got const lvalue reference\n";
}
void process(string&& str){
    cout << "process got rvalue reference\n";
}

void process(const string&& str){
    cout << "process got const rvalue reference\n";
}


void test_rvalue_reference(void){
    cout << __func__ << endl;
    cout << "1.call process\n";
    string str("str...");
    const string cstr("cstr...");
    process(str);
    process(cstr);
    process(string("str..."));
    auto la = []()->const string{
        const string cstr("cstr...");
        return cstr;
    };
    process(la());

    cout << "2.call process with move\n";
    process(move(string("str...")));
    process(move(str));
    cout << "move and call process str=" << str << endl;
    process(move(cstr));
    cout << "move and call process cstr=" << cstr << endl;


    cout << "3.push str to vector with move\n";
    vector<string> vs;
    vs.push_back(move(str));
    cout << "move and push to vector str=" << str << endl;
    vs.push_back(move(cstr));
    cout << "move and push to vector cstr=" << cstr << endl;

}



void forward(string&& str){
    cout << "forward string&& =>" ;
    process(str);
}

void forward(string& str){
    cout << "forward string& =>" ;
    process(str);
}



template<typename T> 
void perfect_forward(T&& t){
    cout << "perfect_forward =>" ;
    process(forward<T>(t));
}




void test_forward(void){
    cout << __func__ << endl;
    forward(string("str..."));
    string str("str...");
    forward(str);
    perfect_forward(string("str..."));
    perfect_forward(str);

}


int main() {

    test_rvalue_reference();
    test_forward();

    return 0;
}

/* output

test_rvalue_reference
1.call process
process got lvalue reference
process got const lvalue reference
process got rvalue reference
process got const rvalue reference
2.call process with move
process got rvalue reference
process got rvalue reference
move and call process str=str...
process got const rvalue reference
move and call process cstr=cstr...
3.push str to vector with move
move and push to vector str=
move and push to vector cstr=cstr...
test_forward
forward string&& =>process got lvalue reference
forward string& =>process got lvalue reference
perfect_forward string&& =>process got rvalue reference
perfect_forward string& =>process got rvalue reference


## move仅仅把一个左值无条件转化为右值，至于是否是移动，是有类的移动构造或者移动赋值实现的

## 完美转发是在模板函数中使用的。一般情况下&接受左值，&&接受右值，但是在模板函数中&&既可以接受左值又可以接受右值。
这时模板参数的&&是作为万能引用。当接受左值时，& && 会折叠成&；当接受右值时，&& &&会折叠成&&。

template<typename T> 
void perfect_forward(T&& t){
    cout << "perfect_forward =>" ;
    process(forward<T>(t));
}

*/