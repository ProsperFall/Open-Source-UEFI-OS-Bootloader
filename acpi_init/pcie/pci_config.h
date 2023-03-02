#include <stdint.h>

typedef struct _pci_config_spce_
{
  uint16_t DeviceID;
  uint16_t VendorID;
  uint16_t Status;
  uint16_t ClassCodelow16bit;//only little endian(x86)
  uint8_t ClassCodehigh8bit;//
  uint8_t RevisionID;
  uint8_t BIST;
  uint8_t HeadType;
  uint8_t LatencyTimer;
  uint8_t CacheLineSize;
} PciConfigSpaceHead;

typedef struct _pci_config_head_type_0_
{
  PciConfigSpaceHead pcihead;
  uint32_t BaseAddressRegister_1;
  uint32_t BaseAddressRegister_2;
  uint32_t BaseAddressRegister_3;
  uint32_t BaseAddressRegister_4;
  uint32_t BaseAddressRegister_5;
  uint32_t BaseAddressRegister_6;
} PciConfigSpaceDevice;

typedef struct _pci_config_head_type_1_
{
  PciConfigSpaceHead pcihead;
} PciConfigSpaceBridge;
