include <stdint.h>

typedef struct __BootConfigration
{
    uint16_t type;
    uint8_t partition;
    uint8_t number;
    uint16_t language;
    uint8_t timeout;
    uint8_t number;
    uint8_t FileSysId[4];
    uint8_t InfoSysReq;
    uint8_t _Rsvd[3];
    uint8_t _Rsvd1[16];
    uint64_t SysPathPtr;
    uint64_t SysDecPtr;
    uint32_t Guid1;
    uint16_t Guid2;
    uint16_t Cuid3;
    uint8_t Guid4[8];
} BootConfigration;
