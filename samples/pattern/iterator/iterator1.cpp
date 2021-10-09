#include <iostream>
#include <map>
#include <vector>
#include <assert.h>
using namespace std;

class student
{
private:
    string name_;
    int age_;
public:
    student(const string name, int age):
        name_(name),age_(age){}
    ~student() {}
    void do_something(){
        cout << name_ << " do something\n";
    }
};

//for collection like class, add_item/del_item/iterator are the three big things
class classroom
{
private:
    /* data */ 
    map<int,student> stus_;
public:
    classroom(/* args */) {}
    ~classroom() {}
    int add_student(int idx, student&& stu){
        stus_.insert(pair<int,student>(idx,stu));
    }
    void del_student(int id){
        stus_.erase(id);
        return ;
    }
    typedef typename map<int,student>::iterator stu_iterator;
    stu_iterator concreate_iterator(){
        return stus_.begin();
    }
    bool stu_iterator_is_end(const stu_iterator& it){
        return (it == stus_.end());
    }
};


int main(int argc, char const *argv[])
{
    /* code */
    classroom c;
    {
        auto it = c.concreate_iterator();
        assert(c.stu_iterator_is_end(it) == true);
    }
    {
        c.add_student(1,student("lilei",10));
        c.add_student(2,student("lucy",11));
        c.add_student(3,student("lili",12));
        auto it = c.concreate_iterator();
        assert(c.stu_iterator_is_end(it) == false);
        cout << "for each student\n";
        for (;!c.stu_iterator_is_end(it);it++){
            cout << "student " << it->first << " ";
            it->second.do_something();
        }
    }
    return 0;
}

