#ifndef _DRINK_SHOP_PACKER_H_
#define _DRINK_SHOP_PACKER_H_
#include "drink_shop_handler.hpp"
#include "drink_shop_transaction.hpp"
#include "aixlog.hpp"
class drink_shop_packer : public abstract_handler
{
private:
    /* data */
public:
    drink_shop_packer(/* args */):abstract_handler(PACKER_HANDLER_ID) {}
    ~drink_shop_packer() {}
    void handle(transacation* t) override {
        LOG(DEBUG) << "drink_shop_packer handle\n"; 
        t->add_stamp("packer");
        return abstract_handler::handle(t);
    }
};
#endif