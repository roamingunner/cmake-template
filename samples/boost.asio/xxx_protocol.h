#ifndef _XXX_PROTOCOL_H_
#define _XXX_PROTOCOL_H_
/*
assume we use TLP porotocol
|magic(0xAA55)|type(2 bytes)|payload length in bytes(4 bytes)|payload....|
type define
1:VERSION---return version string
2:ECHO---respone msg what client sent
*/
struct xxx_protocol{
#define XXX_PROTOCOL_MAGIC		(0xAA55)
#define XXX_PROTOOCL_VERSION	1
#define XXX_PROTOOCL_ECHO		2
	uint16_t magic;
	uint16_t type;
	uint32_t payload_length;
	char payload[0];
};
#endif