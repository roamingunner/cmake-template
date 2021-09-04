#ifndef _DRINK_SHOP_HANDLER_H_
#define _DRINK_SHOP_HANDLER_H_
#include <string>
#include "drink_shop_transaction.hpp"
using namespace std;

enum handler_id{
    UNKNOWN_HANDLER_ID,
    MACKER_HANDLER_ID,
    PACKER_HANDLER_ID,
    RECEPTIONIST_HANDLER_ID,
    NULL_HANDLER_ID
};

/**
 * The Handler interface declares a method for building the chain of handlers.
 * It also declares a method for executing a request.
 */
class handler {
    public:
    handler(){};
    virtual ~handler(){}
    virtual handler *set_next(handler *handler) = 0;
    virtual void handle(transacation* t) = 0;
};
/**
 * The default chaining behavior can be implemented inside a base handler class.
 */
class abstract_handler : public handler {
    /**
     * @var Handler
     */
    private:
    enum handler_id id_;
    handler *next_handler_;

    public:
    abstract_handler(const enum handler_id id) : id_(id), next_handler_(nullptr) {
    }
    handler *set_next(handler *h) override {
        this->next_handler_ = h;
        // Returning a handler from here will let us link handlers in a convenient
        // way like this:
        // $monkey->setNext($squirrel)->setNext($dog);
        return h;
    }
    enum handler_id id() const{
        return id_;
    }
    void handle(transacation* t) override {
        if (this->next_handler_) {
            return this->next_handler_->handle(t);
        }
        return ;
    }
    void handle(const enum handler_id hid, transacation* t){
        handler *h = this->next_handler_;
        while(h){
            abstract_handler *ah = reinterpret_cast<abstract_handler *>(h);
            if (ah->id() == hid){
                return ah->handle(t);
            }else{
                h = this->next_handler_;
            }
        }
    }
};


class null_handler : public abstract_handler
{
private:
    /* data */
public:
    null_handler(/* args */):abstract_handler(NULL_HANDLER_ID) {}
    ~null_handler() {}
    void handle(transacation* t) override {
        delete t;
        return;
    }
};

#endif
