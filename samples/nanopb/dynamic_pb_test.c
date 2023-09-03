#include <stdio.h>
#include <pb_encode.h>
#include <pb_decode.h>
#include "dcmd.pb.h"

#define BUFFER_SZ           128

static bool encode_string(pb_ostream_t *stream, const pb_field_t *field, void * const *arg)
{
    return pb_encode_tag_for_field(stream, field) &&
           pb_encode_string(stream, *arg, strlen(*arg));
}

bool encode_cmd_msg(pb_ostream_t *stream, cmd_msg *msg, size_t *encode_sz)
{
    if (!pb_encode_delimited(stream, cmd_msg_fields, msg)){
        printf("encode_CmdMessage failed: %s\n", PB_GET_ERROR(stream));
        return false;
    }else{
        *encode_sz = stream->bytes_written;
        printf("encode_CmdMessage length: %ld\n", stream->bytes_written);
    }
    return true;
}


bool encode_VersionReq(uint8_t *buffer, size_t buffer_sz, size_t *encode_sz)
{
    memset(buffer,0,buffer_sz);
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, buffer_sz);
    cmd_msg message = cmd_msg_init_default;
    message.which_type = cmd_msg_version_req_tag;
    return encode_CmdMessage(&stream,&message, encode_sz);
}


bool encode_VersionRsq(uint8_t *buffer, size_t buffer_sz, size_t *encode_sz)
{
    memset(buffer,0,buffer_sz);
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, buffer_sz);
    cmd_msg message = cmd_msg_init_default;
    message.which_type = cmd_msg_version_rsp_tag;
    message.type.version_rsp.value.funcs.encode = &encode_string;
    message.type.version_rsp.value.arg = "v1.0.0";
    return encode_CmdMessage(&stream,&message, encode_sz);
}

bool encode_PingReq(uint8_t *buffer, size_t buffer_sz, char *talk, size_t *encode_sz){
    memset(buffer,0,buffer_sz);
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, buffer_sz);
    cmd_msg message = cmd_msg_init_default;
    message.which_type = cmd_msg_ping_req_tag;
    message.type.ping_req.str.funcs.encode = &encode_string;
    message.type.ping_req.str.arg = talk;
    message.type.ping_req.pre = 2003;
    return encode_CmdMessage(&stream,&message, encode_sz);
}

bool encode_PingRsq(uint8_t *buffer, size_t buffer_sz, char *talk, size_t *encode_sz){
    memset(buffer,0,buffer_sz);
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, buffer_sz);
    cmd_msg message = cmd_msg_init_default;
    message.which_type = cmd_msg_ping_rsp_tag;
    message.type.ping_rsp.str.funcs.encode = &encode_string;
    message.type.ping_rsp.str.arg = talk;
    message.type.ping_rsp.post = 6002;
    return encode_CmdMessage(&stream,&message, encode_sz);


}

bool decode_msg(uint8_t *buffer, size_t buffer_sz)
{
    pb_istream_t stream = pb_istream_from_buffer(buffer, buffer_sz);
    cmd_msg message = cmd_msg_init_default;
    /* Now we are ready to encode the message! */
    if (!pb_decode_delimited(&stream, cmd_msg_fields, &message)){
        printf("pb_decode_delimited failed: %s\n", PB_GET_ERROR(&stream));
        return false;
    }else{
        switch (message.which_type)
        {
        case CmdMessage_version_req_tag:
            printf("decode CmdMessage_version_req_tag\n");
            break;
        case CmdMessage_version_rsp_tag:
            printf("decode CmdMessage_version_rsp_tag\n");
            printf("version: %s\n",message.type.version_rsp.value);
            break;
        case CmdMessage_ping_req_tag:
            printf("decode CmdMessage_ping_req_tag\n");
            printf("ping_req.str %s\n",message.type.ping_req.str);
            printf("ping_req.pre %d\n",message.type.ping_req.pre);

            break;
        case CmdMessage_ping_rsp_tag:
            printf("decode CmdMessage_ping_rsp_tag\n");
            printf("ping_rsp.str: %s\n",message.type.ping_rsp.str);
            printf("ping_rsp.post: %d\n",message.type.ping_rsp.post);

            break;
        default:
            printf("unknown message\n");

            break;
        }

    }
    return true;
}




int main()
{
    /* This is the buffer where we will store our message. */
    uint8_t buffer[BUFFER_SZ] = {0};
    size_t message_length;

    encode_VersionReq(buffer,BUFFER_SZ,&message_length);
    decode_msg(buffer,message_length);
    encode_VersionRsq(buffer,BUFFER_SZ,&message_length);
    decode_msg(buffer,message_length);

    encode_PingReq(buffer,BUFFER_SZ,"helloworld",&message_length);
    decode_msg(buffer,message_length);
    encode_PingRsq(buffer,BUFFER_SZ,"12345678",&message_length);
    decode_msg(buffer,message_length);
    return 0;
}