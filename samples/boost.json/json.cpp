#include <iostream>
#include <string>
#include <vector>
#include <boost/json.hpp>

using namespace std;
namespace testns{
    struct student{
        int id;
        string name;
        string birthday;
        vector<string> email;
        vector<string> phone;
        string address;
    };

    void tag_invoke(boost::json::value_from_tag, boost::json::value& jv, struct student const& i )
    {
        jv = {
            { "id" , i.id },
            { "name", i.name },
            { "birthday", i.birthday },
            { "email", i.email },
            { "phone", i.phone },
            { "address", i.address }
        };
    }

    student tag_invoke( boost::json::value_to_tag<student>, boost::json::value const& jv )
    {
        student s;
        boost::json::object const& obj = jv.as_object();
        return student{
            boost::json::value_to<int>(obj.at("id")),
            boost::json::value_to<string>(obj.at("name")),
            boost::json::value_to<string>(obj.at("birthday")),
            boost::json::value_to<vector<string>>(obj.at("email")),
            boost::json::value_to<vector<string>>(obj.at("phone")),
            boost::json::value_to<string>(obj.at("address"))
        };

        return s;
    }

}

int main(int argc, char const *argv[])
{

    const char* raw = R"({"id":2,"name":"Lee","birthday":"20110711","contact":{"email":["Lee123@qq.com","lee123@gmail.com"],"phone":["18966669999","13677778888"],"address":"shenzhen"}})";
    {
        cout << "1.prase from raw data\n";
        boost::json::value jv = boost::json::parse(raw);
        cout << "jv: " << jv << endl;

        cout << "2.serialize to string\n";
        string s1 = boost::json::serialize(jv);
        cout << "jv serialize:" << s1 << endl;

        string s2 = boost::json::serialize(jv.as_object()["contact"]);
        cout << "jv contact serialize:" << s1 << endl;

    }

    {
        try
        {
            cout << "\n3.access data\n";
            boost::json::value jv = boost::json::parse(raw);
            assert( jv.is_object());
            assert(jv.kind() == boost::json::kind::object);
            boost::json::object& obj = jv.as_object();
            assert(obj.contains("id"));
            cout << "id:" << obj["id"].as_int64() << endl;
            auto it1 = obj.find("name");
            assert(it1 != obj.end());
            cout  << it1->key() << ":" << it1->value() << endl;

            boost::json::object *pobj = jv.if_object();
            assert(pobj);
            boost::json::value* jv_contact = pobj->if_contains("contact");
            assert(jv_contact);

            //output {"email":["Lee123@qq.com","lee123@gmail.com"],"phone":["18966669999","13677778888"],"address":"shenzhen"}
            cout  << (*jv_contact) << endl;

            //output shenzhen
            cout  << jv.as_object()["contact"].as_object()["address"] << endl;
            //output ["Lee123@qq.com","lee123@gmail.com"]
            cout  << jv.as_object()["contact"].as_object()["email"] << endl;

            //list key in object
            cout << "list key in object:";
            for (boost::json::object::const_iterator it2 = jv.as_object().begin(); it2 != jv.as_object().end(); it2++){
                cout << it2->key() << ",";
            }
            cout << endl;

            //print array
            cout << "print email array:";
            boost::json::array& jv_array = jv.as_object()["contact"].as_object()["email"].as_array();
            jv_array.push_back("lee123@sina.com");

            for (int i = 0; i < jv_array.size(); i++){
                cout << jv_array[i].as_string() << ",";
            }
            cout << endl;

        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }

    {
        try
        {
            cout << "4.convert from struct\n";
            testns::student s1{2, "Lee", "20110711", {"Lee123@qq.com","lee123@gmail.com"} , {"18966669999","13677778888"},"shenzhen"};
            boost::json::value jv = boost::json::value_from(s1);
            cout << jv << endl;

            cout << "5.convert to struct\n";
            testns::student s2 = boost::json::value_to<testns::student>(jv);
            cout << "s2.id:" << s2.id << endl;
            cout << "s2.name:" << s2.name << endl;
            cout << "s2.birthday:" << s2.birthday << endl;
            cout << "s2.email:";
            for (int i = 0; i < s2.email.size(); i++){
                cout << s2.email[i] << ",";
            }
            cout << endl;
            cout << "s2.phone:";
            for (int i = 0; i < s2.phone.size(); i++){
                cout << s2.phone[i] << ",";
            }
            cout << endl;
            cout << "s2.address:" << s2.address << endl;

        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }

    }
    /* code */
    return 0;
}
