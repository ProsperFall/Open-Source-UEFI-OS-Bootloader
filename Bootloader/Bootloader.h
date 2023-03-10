include <stdint.h>

typedef struct __BootConfigration
{
    uint16_t TYPE;
    uint8_t PARTITION;
    /*
        0~127, Boot an system without Open another UEFI_APPLICATION, THIS CAN'T BE '0'
        <0 Unuseable Boot Configration
    */
    CHAR8 LANG;
    /*
        VersionSpec
    */
    CHAR8 VERSION;
    CHAR8 TIMEOUT;/*Seconds*/
    CHAR8 METHOD;/*Reserved*/
    CHAR8 NUMBER;/*Pointer/64*/
    CHAR8 FileSystemID;
    INT16 _Rsvd_;
} BootConfigration;

typedef struct __BootConfigration_EFI  //TYPE 1110 0000B
{
    BootConfigration Head;
    CHAR8 BootCongigrationCount;//useable Count,the Count must 128
    CHAR8 _Rsvd_0;
    CHAR8 ParameterRequirements;
    /*
      1--- ----,SystemTable
      -1-- ----,ImageHandle
      ---- ---1,MemoryInfo
      ---1 ----,ExtParameter
    */
    CHAR8 _Rsvd_1;
    CHAR16 _Rsvd_2;
    INT32 SystemPathPointer;//(CHAR8*)
    INT32 DescriptionPointer;
    INT32 GUID_POINTER;
    INT32 _Rsvd_;
} BootConfigration_EFI;

typedef struct __BootConfigration_APP  //TYPE 1110 0001B
{
    BootConfigration Head;
    CHAR8 _Rsvd_0;
    CHAR8 ExternParameterPtrA8bit;
    CHAR8 ParameterRequirements;
    CHAR8 ExternParameterPtrB8bit;
    CHAR16 ExternParameterPtrC16bit;
    INT32 SystemPathPointer;//(CHAR8*)
    INT32 DescriptionPointer;
    INT32 GUID_POINTER;
    INT32 _Rsvd_;
} BootConfigration_APP;

typedef struct __BootConfigration_SYS  //TYPE 0000 0001B
{
    BootConfigration Head;
    CHAR8 _Rsvd_0;
    CHAR8 ExternParameterPtrA8bit;//low8bit
    CHAR8 ParameterRequirements;
    CHAR8 ExternParameterPtrB8bit;//high8bit of low16bit
    CHAR16 ExternParameterPtrC16bit;//high16bit
    INT32 SystemPathPointer;//(CHAR8*)
    INT32 DescriptionPointer;
    INT32 GUID_POINTER;
    INT32 _Rsvd_;
} BootConfigration_SYS;
