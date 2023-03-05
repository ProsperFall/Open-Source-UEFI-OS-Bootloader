#include <stdint.h>

typedef struct __Root_System_Description_Ptr__
{
  uint8_t Signature[8]; //char
  uint8_t Checksum;
  uint8_t OEMID[6];  //char
  uint8_t Revision;
  uint32_t RsdtAddr;  //32 bit physical address of the RSDT.
  uint32_t Length;
  uint64_t XsdtAddr;
  uint8_t ExtendedChecksum;
  uint8_t __Rsvd[3];
} RSDP;

typedef struct __System_Descripition_Table_Header__
{
  uint8_t Signature[4];
  uint32_t Length;  //bytes,include header
  uint8_t Revision;
  uint8_t Checksum;
  uint8_t OemId[6];
  uint8_t OemTableId[8];
  uint32_t OemRevision;
  uint32_t CreatorId;
  uint32_t CreatorRevison;
}SystemDescripitionTableHeader;
//Length - 36(sizeof(Header)) = EntryCount * 8

typedef struct __Extended_System_Descripition_Table__
{
  SystemDescripitionTableHeader Header;
  uint32_t Entry[2];
} XSDT;

typedef struct __Memort_mapped_Config__
{
  SystemDescripitionTableHeader Header;
  uint8_t _Rsvd[8];
  uint8_t BassAdressOfEcam[8];  //uint64_t
  uint8_t PciSegmentGroupNumber[2]; //uint16_t
  uint8_t StartPciBusNum;
  uint8_t EndPciBusNum;
  uint8_t _Rsved[4];
} MCFG;
typedef struct __Memort_mapped_Config_ECAM_Sub__
{
  uint64_t BassAdressOfEcam;  //uint64_t
  uint16_t PciSegmentGroupNumber; //uint16_t
  uint8_t StartPciBusNum;
  uint8_t EndPciBusNum;
  uint8_t _Rsved[4];
} ECAM;

int AcpiTableSignIs(char* ptr1,const char* Sign);
const CHAR8 McfgSign[4] = {'M','C','F','G'};
