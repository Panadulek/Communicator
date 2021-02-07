#ifndef PROTOCOLHEADERS_H
#define PROTOCOLHEADERS_H

#define USER_NAME_SIZE 32
#define PROTOCOL_VERSION 1
#define PROTOCOL_TAG 3
#define PROTOCOL_MAX_BUFFER 1024
#define FILE_PROTOCOL_MAX_BUFFER 4096
#define PATH_FILE_SIZE 512
#define FILE_NAME_SIZE 64
#define IP_SIZE 16

#include <cstdint>
enum MessageType : uint8_t {
    SimpleMessageType = 0,
    PrivateMessageType,
    FileRequest,
    FileRequestAccepted,
    NewConnection,
    Connected,
    Disconnected
};



typedef struct
{
    uint8_t version;
    MessageType typeMessage;
    uint32_t size;
    uint8_t extender[16];
} MainHeader;




typedef struct
{
    uint32_t nameSize;
    uint32_t sizeData;
} FileHeader;


#endif // PROTOCOLHEADERS_H
