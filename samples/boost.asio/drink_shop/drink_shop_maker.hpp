#ifndef _DRINK_SHOP_MAKER_H_
#define _DRINK_SHOP_MAKER_H_
#include "drink_shop_handler.hpp"
#include "drink_shop_transaction.hpp"
class drink_shop_maker : public abstract_handler
{
private:
    /* data */
public:
    drink_shop_maker(/* args */):abstract_handler(MACKER_HANDLER_ID) {}
    ~drink_shop_maker() {}
    void handle(transacation* t) override {
        t->add_stamp("maker");
        enum handler_id hid = NULL_HANDLER_ID; 
        if (t->is_packed()){
            hid = PACKER_HANDLER_ID;
        }else{
            hid = RECEPTIONIST_HANDLER_ID;
        }
        return abstract_handler::handle(hid, t);
    }
};
#endif