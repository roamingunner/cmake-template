# 买奶茶

奶茶店买一杯奶茶涉及到的步骤有


- 客户到接待员处点奶茶
- 接待员打单打印单据并收银
- 操作员制作奶茶并专递给打包员
- 打包员完成打包，把奶茶给接待员
- 接待员把奶茶给客户

假设一枚奶茶的打单时间是1s，奶茶制作时间是8s，打包时间为2s
receptionist

```plantuml
@startuml
actor 客户
actor 接待员
actor 制作员
actor 打包员

客户 --> 接待员 : 点单
接待员 -> 制作员 : 订单 
制作员 -> 打包员 : 奶茶
接待员 <-- 打包员 : 打包的奶茶
客户 <--  接待员

@enduml
```

```plantuml
@startuml
actor 客户
component 奶茶店{
    component 接待员
    component 制作员
    component 打包员
    queue product_queue
    queue pack_queue
    queue ready_queue

}
客户 --> 接待员
接待员 -u-> product_queue
product_queue -> 制作员
制作员 -> pack_queue
pack_queue -d-> 打包员
打包员 --> ready_queue
ready_queue -> 接待员
接待员 --> 客户
@enduml
```

```plantuml
@startuml
class receptionist{
  - share_ptr<ready_queue> in
  - share_ptr<product_queue> out
  + handler()
}

class maker{
  - share_ptr<product_queue> in
  - share_ptr<pack_queue> out
  + handler()
}

class packer{
  - share_ptr<pack_queue> in
  - share_ptr<pack_queue> out
  + handler()
}

queue pack_queue{

}
queue product_queue{
    
}
queue ready_queue{
    
}
receptionist::in o-- ready_queue
receptionist::out o-- product_queue
maker::in o-- product_queue
maker::out o-- pack_queue
packer::in o-- pack_queue
packer::out o--- ready_queue
@enduml
```

```
#define DRINK_SHOP_PROTO_CMD_VERSION 1
#define DRINK_SHOP_PROTO_CMD_ORDER   2

struct drink_shop_protocol{
	uint32_t magic;
	int command;
    int result;
	uint32_t payload_length;
	char payload[0];
}

对于version命令的请求，无payload
相应payload为，"{"version":"xxxx"}"

对于order命令的请求，payload为奶茶的参数
{
    "kind":"xxx",
    "size":"big/middle",
    "sweet": 1-3,
}

对于order命令的相应，payload设定为
{
    "order_idx":203，
    "kind":"xxx",
    "size":"big/middle",
    "sweet": 1-3,
    "stamps":[
        "receptionist@hh:mm::ss",
        "maker@hh:mm::ss",
        "packer@hh:mm::ss",
    ]
}

```

```plantuml
@startuml
class transation{
    uint32 order_idx
    string kind
    string size
    int sweet
    list<string> stamps
}
@enduml
```