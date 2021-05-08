#include <iostream>
#include <memory>
#include <vector>
#include "specification.h"

using namespace tpl;
class person
{
private:
    std::string name;
    std::string address;
    /* data */
public:
    person(const std::string name_, const std::string address_):
    name(name_),address(address_) {}
    ~person() {}
    const std::string get_name(){
        return name;
    }
    const std::string get_address(){
        return address;
    }
    // Again, a friendly string maker
    friend std::ostream& operator<<(std::ostream& os, const person& p){
        os << p.name << " " << p.address << std::endl;
        return os;
    }
};

class name_spec : public spec<person>
{
private:
    std::string n;
public:
    name_spec(const std::string name):n(name){}
    ~name_spec() {}
    bool is_satisified(person *p){
        return (n == p->get_name());
    }
};

class address_spec : public spec<person>
{
private:
    std::string addr;
public:
    address_spec(const std::string address):addr(address){}
    ~address_spec() {}
    bool is_satisified(person *p){
        return (addr == p->get_address());
    }
};


class book
{
private:
    std::vector<std::shared_ptr<person>> people;
    /* data */
public:
    book(/* args */) {}
    ~book() {}
    int add_person(const std::string name, const std::string address){
        auto p = std::make_shared<person>(name,address);
        people.push_back(p);
    }
    //WHY:error: passing ‘const tpl::spec<person>’ as ‘this’ argument discards qualifiers [-fpermissive]
    //std::vector<std::shared_ptr<person>> select_person(const spec<person>&& sp)
    std::vector<std::shared_ptr<person>> select_person(spec<person>&& sp){
        std::vector<std::shared_ptr<person>> persons;
        for (auto p : people){
            bool b = sp.is_satisified(p.get());
            if(b){
                persons.push_back(p);
            }
           
        }
        return persons;
    }
};

int main(int argc, char const *argv[])
{
    /* code */
    book B;
    B.add_person("leo","hangzhou");
    B.add_person("nina","hangzhou");
    B.add_person("lucy","beijing");
    B.add_person("lili","beijing");
    B.add_person("hanmeimei","suzhou");
    B.add_person("lili","shanghai");

    auto persons1 = B.select_person(name_spec("lili"));
    for (auto p : persons1){
        std::cout << (*p);
    }
    auto persons2 = B.select_person(address_spec("hangzhou"));
    for (auto p : persons2){
        std::cout << (*p);
    }
    return 0;
}
