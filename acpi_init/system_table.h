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
  uint32_t Length;
  uint8_t Revision;
  uint8_t Checksum;
  uint8_t OemId[6];
  uint8_t OemTableId[8];
  uint32_t OemRevision;
  uint32_t CreatorId;
  uint32_t CreatorRevison;
}SystemDescripitionTableHeader;

typedef struct __Extended_System_Descripition_Table__
{
  SystemDescripitionTableHeader Header;
  void* Entry;
} XSDT;
