#ifndef _DRINK_SHOP_PROTOCOL_H_
#define _DRINK_SHOP_PROTOCOL_H_

#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <exception>
#include <stdexcept>
#include <iostream>

using namespace std;
#define DRINK_SHOP_PROTO_CMD_VERSION 1
#define DRINK_SHOP_PROTO_CMD_ORDER  2
#define DRINK_SHOP_MAGIC 0x12345678
class drink_shop_protocol
{
private:
    /* header */
    uint32_t magic_;
    uint32_t command_;
    int32_t result_;
    uint32_t body_length_;
    /* body */
    const char *body_;
public:
    drink_shop_protocol(const uint32_t command,const int32_t result, const uint32_t body_length, const char *body = nullptr)
        :command_(command),result_(result),body_length_(body_length),body_(body){}
    drink_shop_protocol():magic_(0),command_(0),result_(0),body_length_(0),body_(nullptr){}
    ~drink_shop_protocol() {}
    const uint32_t magic(){
        return magic_;
    }
    const uint32_t command(){
        return command_;
    }
    const int result(){
        return result_;
    }
    const uint32_t body_length(){
        return body_length_;
    }
    const uint32_t header_size(){
        return sizeof(drink_shop_protocol) - sizeof(char *);
    }
    const uint32_t package_size(){
        return sizeof(drink_shop_protocol) - sizeof(char *) + body_length_;
    }
    const char *body(){
        return body_;
    }
    void set_command(const uint32_t command){
        command_ = command;
    }
    void set_result(const int32_t result){
        result_ = result;
    }
    void set_body_length(const uint32_t body_length){
        body_length_ = body_length;
    }
    void set_body(char* body){
        body_ = body;
    }
    int parse_from_buffer(char *buffer){
        if (!buffer)
            return -EINVAL;
        uint32_t *ptr = reinterpret_cast<uint32_t *>(buffer);
        
        if (*ptr != DRINK_SHOP_MAGIC)
            return -EINVAL;
        
        magic_ = ntohl(*ptr);
        ptr++;
        command_ = ntohl(*ptr);
        ptr++;
        result_ = ntohl(*ptr);
        ptr++;
        body_length_ = ntohl(*ptr);
        if (body_length_){
            body_ = buffer + this->header_size();
        }else{
            body_ = nullptr;
        }
        return 0;
    }
    ssize_t serialize_hdr_to_buffer(char *buffer){
        if (!buffer)
            return -EINVAL;
        uint32_t *ptr = reinterpret_cast<uint32_t *>(buffer);
        *ptr = htonl(magic_);
        ptr++;
        *ptr = htonl(command_);
        ptr++;
        *ptr = htonl(result_);
        ptr++;
        *ptr = htonl(body_length_);
        return this->header_size();
    }
    int serialize_pkg_to_buffer(char *buffer){
        if (!buffer)
            return -EINVAL;
        uint32_t *ptr = reinterpret_cast<uint32_t *>(buffer);
        *ptr = htonl(magic_);
        ptr++;
        *ptr = htonl(command_);
        ptr++;
        *ptr = htonl(result_);
        ptr++;
        *ptr = htonl(body_length_);
        if (body_length_ && body_){
            memcpy(buffer + this->header_size(), body_, body_length_);
            return this->package_size();
        }else{
            return this->header_size();
        }
    }
    friend ostream &operator <<(ostream& out, const drink_shop_protocol& instance){
        out << "command:" << instance.command_ << " result:" << instance.result_
            << " body_length:" << instance.body_length_ << " body:" << (void *)instance.body_ << endl; 
        return out;
    }
};


#endif