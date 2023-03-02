#include <stdint.h>

typedef struct __Root_system_description_ptr__
{
  char Signature[8];
  uint8_t Checksum;
  char OEMID[6];
  uint8_t Revision;
  uint32_t RsdtAddr;//32 bit physical address of the RSDT.
  uint32_t Length;
  uint64_t XsdtAddr;
  uint8_t ExtendedChecksum;
  uint8_t __Rsvd[3];
} RSDP;
