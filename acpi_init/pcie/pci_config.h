#include <stdint.h>

#define PCI_BDF(x,y,z) ((x<<20)|(y<<15)|(z<<12))

typedef struct _pci_config_spce_
{
  uint16_t DeviceID;
  uint16_t VendorID;
  uint16_t Status;
  uint16_t Command;
  uint16_t ClassCodeLower16bits;//only little endian(x86)
  uint8_t ClassCodeUpper8bits;//
  uint8_t RevisionID;
  uint8_t BIST;
  uint8_t HeadType;
  uint8_t LatencyTimer;
  uint8_t CacheLineSize;
} PciConfigSpaceHead;

typedef struct _pci_config_head_type_0_
{
  uint16_t DeviceID;
  uint16_t VendorID;
  uint16_t Status;
  uint16_t Command;
  uint16_t ClassCodeLower16bits;//only little endian(x86)
  uint8_t ClassCodeUpper8bits;//
  uint8_t RevisionID;
  uint8_t BIST;
  uint8_t HeadType;
  uint8_t LatencyTimer;
  uint8_t CacheLineSize;
  
  uint32_t BaseAddressRegister_1;
  uint32_t BaseAddressRegister_2;
  uint32_t BaseAddressRegister_3;
  uint32_t BaseAddressRegister_4;
  uint32_t BaseAddressRegister_5;
  uint32_t BaseAddressRegister_6;
  uint32_t CardbusCISPtr;
  uint16_t SubsystemID;
  uint16_t SubsystemVendorID;
  uint32_t ExpansionROMBaseAddress;
  uint16_t _Rsvd_;
  uint8_t _Rsvd1_;
  uint8_t CapabilitiesPtr;
  uint32_t _Rsvd2_;
  uint8_t MAX_Lat;
  uint8_t Min_Gnt;
  uint8_t InterruptPin;
  uint8_t InterruptLine;
} PciConfigSpaceDevice;

typedef struct _pci_config_head_type_1_
{
  uint16_t DeviceID;
  uint16_t VendorID;
  uint16_t Status;
  uint16_t Command;
  uint16_t ClassCodeLower16bits;//only little endian(x86)
  uint8_t ClassCodeUpper8bits;//
  uint8_t RevisionID;
  uint8_t BIST;
  uint8_t HeadType;
  uint8_t LatencyTimer;
  uint8_t CacheLineSize;
  
  uint32_t BaseAddressRegister_1;
  uint32_t BaseAddressRegister_2;
  uint8_t SecondaryLatencyTimer;
  uint8_t SubordinateBusNumber;
  uint8_t SecondaryBusNumber;
  uint8_t PrimaryBusNumber;
  uint16_t SecondaryStatus;
  uint8_t IOLimit;
  uint8_t IOBase;
  uint16_t MemoryLimit;
  uint16_t MemoryBase;
  uint16_t PrefetchableMemoryLimit;
  uint16_t PrefetchableMemoryBase;
  uint32_t PrefetchableBaseUpper32Bits;
  uint32_t PrefetchableLimitUpper32Bits;
  uint16_t IOBaseLimit16Bits;
  uint16_t IOBaseUpper16Bits;
  uint16_t _Rsvd_;
  uint8_t _Rsvd1_;
  uint8_t CapabilitiesPtr;
  uint32_t ExpansionROMBaseAddress;
  uint16_t BridgeControl;
  uint8_t InterruptPin;
  uint8_t InterruptLine;
} PciConfigSpaceBridge;
