#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <boost/json.hpp>

using namespace std;

namespace testns{
    struct student
    {
        int id;
        string name;
        int age;
    };
    struct teacher
    {
        string name;
        string subject;
    };
    
    struct class_room{
        int grade;
        vector<student> stus;
        teacher te;
    };

    void tag_invoke(boost::json::value_from_tag, boost::json::value& jv, struct student const& i )
    {
        jv = {
            { "id" , i.id },
            { "name", i.name},
            { "age", i.age }
        };
    }

    void tag_invoke(boost::json::value_from_tag, boost::json::value& jv, struct teacher const& i )
    {
        jv = {
            { "name", i.name},
            { "subject", i.subject }
        };
    }

    void tag_invoke(boost::json::value_from_tag, boost::json::value& jv, struct class_room const& i )
    {
        jv = {
            { "grade" , i.grade },
            { "students", i.stus},
            { "teacher", i.te }
        };
    }

    student tag_invoke( boost::json::value_to_tag<student>, boost::json::value const& jv )
    {
        boost::json::object const& obj = jv.as_object();
        return student{
            boost::json::value_to<int>(obj.at("id")),
            boost::json::value_to<string>(obj.at("name")),
            boost::json::value_to<int>(obj.at("age"))
        };
    }

    teacher tag_invoke( boost::json::value_to_tag<teacher>, boost::json::value const& jv )
    {
        boost::json::object const& obj = jv.as_object();
        return teacher{
            boost::json::value_to<string>(obj.at("name")),
            boost::json::value_to<string>(obj.at("subject"))
        };
    }

    class_room tag_invoke( boost::json::value_to_tag<class_room>, boost::json::value const& jv )
    {
        boost::json::object const& obj = jv.as_object();
        return class_room{
            boost::json::value_to<int>(obj.at("grade")),
            boost::json::value_to<vector<student>>(obj.at("students")),
            boost::json::value_to<teacher>(obj.at("teacher"))
        };
    }

}

int main(int argc, char const *argv[])
{
    {
        cout << "1.generic conversion\n";

        int integer = 99;
        boost::json::value jv_integer = boost::json::value_from(integer);
        cout << "jv_integer:\n" << jv_integer << endl;

        string str = "hello";
        boost::json::value jv_str = boost::json::value_from(str);
        cout << "jv_str:\n" << jv_str << endl;

        vector< string > vstr = {"hello", "world"};
        boost::json::value jv_vstr = boost::json::value_from(vstr);
        cout << "jv_vstr:\n" << jv_vstr << endl;

        map<string, vector< int > > positions = {
            { "Alex", { 42, -60, 18 } },
            { "Blake", { 300, -60, -240} },
            { "Carol", { -60, 30, 30 } }
        };
        boost::json::value jv_positions = boost::json::value_from(positions);
        cout << "jv_positions" << jv_positions << endl;

        map<string, map<string,string>> t1 = {
            { "Alex", {{ "1", "a1"},{"2","a2"},{ "3", "a3"}}},
            { "Blake", {{ "1", "b1"},{"2","b2"},{ "3", "b3"}} },
            { "Carol", {{ "1", "c1"},{"2","c2"},{ "3", "c3"}} }
        };
        boost::json::value jv_t1 = boost::json::value_from(t1);
        cout << "jv_t1" << jv_t1 << endl;

    }

    {
        cout << "\n2.user-type conversion\n";
        testns::class_room cr = {1,{{1,"lucy",10},{2,"hanmeimei",11},{3,"lilei",9}},{"Lee","math"}};
        boost::json::value jv = boost::json::value_from(cr);
        cout << jv << endl;

    }

    
}