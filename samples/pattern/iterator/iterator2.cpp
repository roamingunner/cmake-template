#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <memory>
#include <algorithm>

using namespace std;

class subject
{
private:
    /* data */
    const string subject_name_;
    double avg_score_;
    vector<int> scores_;
public:
    subject(const string& subject_name):subject_name_(subject_name), avg_score_(0) {}
    ~subject() {}
    void append_score(int s){
        double old_total = avg_score_ * scores_.size();
        scores_.push_back(s);
        avg_score_ = (old_total + s)/scores_.size();
    }
    double avg_score() const {
        return avg_score_;
    }
    const string subject_name() const {
        return subject_name_;
    }
};



class student
{
private:
    /* data */
    const int idx_;
    const string name_;
    map<const string,shared_ptr<subject>> subjects_;
public:
    student(const int idx, const string& name):idx_(idx),name_(name){}
    ~student() {}
    typedef typename map<const string,shared_ptr<subject>>::iterator subject_iterator;
    typedef typename map<const string,shared_ptr<subject>>::const_iterator const_subject_iterator;
    const string name() const{
        return name_;
    }
    const int idx() const{
        return idx_;
    }
    // ------------------
    int add_subject(const string sn){
        int ret = 0;
        if (subjects_.find(sn) == subjects_.end()){
            try
            {
                subjects_[sn] = make_shared<subject>(sn);
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
                ret = -ENOMEM;
            }
            
        }else{
            ret = -EEXIST;
        }
        return ret;
    }
    void del_subject(const string sn){
        subjects_.erase(sn);
    }
    subject_iterator sbj_end(){
        return subjects_.end();
    }
    const_subject_iterator sbj(const string sn){
        return subjects_.find(sn);
    }
    subject_iterator sbj_begin(){
        return subjects_.begin();
    }
    const shared_ptr<subject>& operator[](const string sn){
        const_subject_iterator it = sbj(sn);
        if (it == sbj_end()){
            throw out_of_range("key not found");
        }
        return it->second;
    }
    // ------------------
};

class classroom
{
private:
    map<int,shared_ptr<student>> students_;
public:
    typedef typename map<int,shared_ptr<student>>::iterator student_iterator;
    typedef typename map<int,shared_ptr<student>>::const_iterator const_student_iterator;
    classroom(/* args */) {}
    ~classroom() {}

    // ------------------
    int add_student(const int idx, const string name){
        int ret = 0;
        if (students_.find(idx) == students_.end()){
            try
            {
                students_[idx] = make_shared<student>(idx,name);
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
                ret = -ENOMEM;
            }
            
        }else{
            ret = -EEXIST;
        }
        return ret;
    }
    void del_student(const int idx){
        students_.erase(idx);
    }
    const_student_iterator stu(const int idx){
        return students_.find(idx);
    }
    student_iterator stu_begin(){
        return students_.begin();
    }
    student_iterator stu_end(){
        return students_.end();
    }
    const shared_ptr<student>& operator[](const int idx){
        const_student_iterator it = stu(idx);
        if (it == stu_end()){
            throw out_of_range("key not found");
        }
        return it->second;
    }
    // ------------------
};

int main(int argc, char const *argv[])
{
    /* code */
    classroom c;

    try
    {
        c.add_student(1,"Lucy");
        c.add_student(2,"Lee");
        c.add_student(3,"Tom");
        c[1]->add_subject("English");
        c[1]->add_subject("Chinese");
        c[1]->add_subject("History");

        c[2]->add_subject("English");
        c[2]->add_subject("Chinese");
        c[2]->add_subject("Math");

        c[3]->add_subject("English");
        c[3]->add_subject("Chinese");
        c[3]->add_subject("Painting");


        c[1]->operator[]("English")->append_score(90);
        c[1]->operator[]("English")->append_score(88);
        c[1]->operator[]("English")->append_score(78);
        c[1]->operator[]("English")->append_score(91);

        c[1]->operator[]("Chinese")->append_score(90);
        c[1]->operator[]("Chinese")->append_score(91);
        c[1]->operator[]("Chinese")->append_score(92);
        c[1]->operator[]("Chinese")->append_score(93);

        c[1]->operator[]("History")->append_score(85);
        c[1]->operator[]("History")->append_score(85);
        c[1]->operator[]("History")->append_score(85);
        c[1]->operator[]("History")->append_score(87);

        for (classroom::student_iterator stu_it = c.stu_begin(); stu_it != c.stu_end(); stu_it++){
            for (student::subject_iterator sbj_it = stu_it->second->sbj_begin();
                sbj_it != stu_it->second->sbj_end(); sbj_it++){
                cout << "idx:" << stu_it->first << " name:" << stu_it->second->name() 
                    << " subject:" << sbj_it->first << " avg_score:" << sbj_it->second->avg_score() << endl;
            }
        }

        //use STL algorithm
        const string name("Lucy");
        auto l = [&name](const classroom::student_iterator::value_type& pair) -> bool{
            return !pair.second->name().compare(name);
        };
        classroom::student_iterator sit = find_if(c.stu_begin(),c.stu_end(),l);
        cout << "show scores of "<< name << ":\n";
        for (student::subject_iterator sbj_it = sit->second->sbj_begin();
            sbj_it != sit->second->sbj_end(); sbj_it++){
            cout << "idx:" << sit->first << " name:" << sit->second->name() 
                << " subject:" << sbj_it->first << " avg_score:" << sbj_it->second->avg_score() << endl;
        }
        
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    


    return 0;
}
