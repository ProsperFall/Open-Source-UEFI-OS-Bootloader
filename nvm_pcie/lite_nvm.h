#include <stdint.h>

#ifndef __PCI_e__
#define __PCI_e_CRS__
#include <acpi_init/pcie/pci_config.h>
#endif

#ifndef __NVM_LITE__
#define __NVM_LITE__

#pragma pack(1)
typedef struct __Nvm_Controller__
{
    uint64_t Capabilities;
    uint32_t Version;
    uint32_t InterruptMaskSet;
    uint32_t InterruptMaskClear;
    uint32_t ControllerConfiguration;
    uint32_t ControllerStatus;
    uint32_t NvmSubsystemReset;
    uint32_t AdminQueueAttributes;
    uint64_t AdminSubmissionQueueBaseAddress;
    uint64_t AdminCompletionQueueBaseAddress;
    uint32_t ControllerMemoryBufferLocation;
    uint32_t ControllerMemoryBufferSize;
    uint32_t BootPartitionInformation;
    uint32_t BootPartitionReadSelect;   //  Optional
    uint32_t BootPartitionMemoryBufferLocation; //  Optional
    uint64_t ControllerMemoryBufferMemorySpaceControl;
    uint32_t ContrillerMemoryBufferStatus;
    uint32_t ControllerMemoryBufferElasticityBufferSize;
    uint32_t ControllerMemoryBufferSustainedWriteThroughput;
    uint32_t NvmSubsystemShutdown;
    uint32_t ControllerReadyTimeouts;       //  68H
} NvmController;

#pragma pack(1)
typedef struct __Persistent_Memory_Region__     //  offset E00H
{
    uint32_t PersistentMemoryRegionCapabilities;
    uint32_t PersistentMemoryRegionControl;
    uint32_t PersistentMemoryRegionStatus;
    uint32_t PersistentMemoryRegionElasticityBufferSize;
    uint32_t PersistentMemoryRegionSustainedWriteThroughput;
    uint32_t PersistentMemoryRegionMemorySpaceControlLower;
    uint32_t PersistentMemoryRegionMemorySpaceControlUpper;
} PersistentMemoryRegion;


#pragma pack(1)
typedef struct __Common_Command_Format__
{
    uint32_t CommandDWord0;
    uint32_t NameSpaceID;   //  CDW1
    uint32_t CommandDWord2;
    uint32_t CommandDWord3;
    uint64_t MetadataPtr;   //  CDW4 CDW5
    uint64_t DataPtr[2];    //  CDW6 CDW7 CDW8 CDW9
    uint32_t CommandDWord10;
    uint32_t CommandDWord11;
    uint32_t CommandDWord12;
    uint32_t CommandDWord13;
    uint32_t CommandDWord14;
    uint32_t CommandDWord15;
} CommonCmd;

#pragma pack(1)
typedef struct __Origin_Command_Format__
{
    uint32_t CommandDWord0;
    uint32_t CommandDWord1;
    uint32_t CommandDWord2;
    uint32_t CommandDWord3;
    uint32_t CommandDWord4;
    uint32_t CommandDWord5;
    uint32_t CommandDWord6;
    uint32_t CommandDWord7;
    uint32_t CommandDWord8;
    uint32_t CommandDWord9;
    uint32_t CommandDWord10;
    uint32_t CommandDWord11;
    uint32_t CommandDWord12;
    uint32_t CommandDWord13;
    uint32_t CommandDWord14;
    uint32_t CommandDWord15;
} OgiCmd;

int initNvm(PciConfigSpaceDevice* SoPciConfig);
int setPageSize(unsigned int PageSize);
uint8_t getDoorBellStep();
uint64_t getAdminSubQueueBaseAddress();
uint64_t debug();
#endif