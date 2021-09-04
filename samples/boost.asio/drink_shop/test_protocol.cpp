#include <iostream>
#include  "drink_shop_protocol.hpp"

int main(int argc, char const *argv[])
{
    /* code */
    class drink_shop_protocol p1;
    char *b1 = "hello";
    std::cout << p1;
    p1.set_command(DRINK_SHOP_PROTO_CMD_ORDER);
    p1.set_result(-ENAVAIL);
    p1.set_body(b1);
    p1.set_body_length(strlen(b1));
    std::cout << p1;

    class drink_shop_protocol p2(DRINK_SHOP_PROTO_CMD_ORDER,0,16);
    std::cout << p2;
    char *b3 = "world";
    class drink_shop_protocol p3(DRINK_SHOP_PROTO_CMD_ORDER,0,strlen(b3),b3);
    std::cout << p3;
    return 0;
}
