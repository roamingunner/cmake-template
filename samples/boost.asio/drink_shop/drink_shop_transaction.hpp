#ifndef _DRINK_SHOP_TRANSACTION_H_
#define _DRINK_SHOP_TRANSACTION_H_
#include <stdint.h>
#include <string>
#include <vector>
#include <ctime>
#include <iostream>
#include "aixlog.hpp"

using namespace std;

class transacation
{
private:
    /* data */
    uint32_t tid_;
    uint32_t rid_;
    string type_;
    string size_;
    bool packed_;
    vector<string> stamps_;
public:
    static uint32_t tid_iterator_;
public:
    transacation(const uint32_t rid, const string& type, const string& size, const bool packed)
        :tid_(tid_iterator_++), rid_(rid), type_(type), size_(size),packed_(packed){
            LOG(NOTICE) << "transacation create: tid=" 
                << tid_ << " rid_=" << rid_ << " type=" << type_ << " size=" << size_ << " packed=" << packed_ << endl;
    }
    ~transacation() {
            LOG(NOTICE) << "transacation destory: tid=" 
                << tid_ << " rid_=" << rid_ << " type=" << type_ << " size=" << size_ << " packed=" << packed_ << endl;
    }
    void add_stamp(const string& who){
        string stamp = who;
        time_t now = time(0);
        stamp += "@";
        stamp += ctime(&now);
        stamps_.push_back(stamp);
    }
    const bool is_packed(){
        return packed_;
    }
    const uint32_t tid() const {
        return tid_;
    }
    const uint32_t rid() const {
        return rid_;
    }
    const string type() const {
        return type_;
    }
    const string size() const {
        return size_;
    }
    vector<string>& stamps() {
        return stamps_;
    }
    friend ostream &operator <<(ostream& out, const transacation& instance){
        out << "transacation tid:" << instance.tid_ << " rid:" << instance.rid_
            << " type:" << instance.type_ << " size_:" << instance.size_ << " packed:" << instance.packed_ << endl; 
        return out;
    }
};

#endif