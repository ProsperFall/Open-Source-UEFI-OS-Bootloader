#include <stdint.h>
const int UEFI_APPLICATION = 1;
const int OPERATE_SYS = 2;
typedef struct __BootConfigrationHead
{
    uint16_t type;
    uint8_t partition;
    uint8_t number;
    uint16_t language;
    uint8_t select;
    uint8_t version;
    uint8_t FileSysId[4];
    uint8_t InfoSysReq;
    uint8_t _Rsvd[3];
    uint8_t _Rsvd1[16];
    uint64_t SysPathPtr;
    uint64_t SysDecPtr;
    uint32_t Guid1;
    uint16_t Guid2;
    uint16_t Guid3;
    uint8_t Guid4[8];
} BootConfigrationHead;

typedef struct __BootConfigration
{
    uint16_t type;
    uint8_t partition;  //  0001 0000
    uint8_t number; //  0010 0000
    uint16_t language;  //  0100 0000
    uint8_t timeout;
    uint8_t version;
    uint8_t FileSysId[4];
    uint8_t InfoSysReq;
    /*
        0000 0001 SystemTable
        0000 0010 MemoryMap
        0000 0100 BootDeviceBDF
        0000 1000 BootDeviceType
        type:0x02 OSU-OL spec
        ext : typspec
    */
    uint8_t _Rsvd[3];
    uint8_t _Rsvd1[16];
    uint64_t SysPathPtr;
    uint64_t SysDecPtr;
    uint32_t Guid1;
    uint16_t Guid2;
    uint16_t Guid3;
    uint8_t Guid4[8];
} BootConfigration;
