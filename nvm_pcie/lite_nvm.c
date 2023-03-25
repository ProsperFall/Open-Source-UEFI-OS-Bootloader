#include <stdint.h>
#include <stdio.h>

#include <nvm_pcie/lite_nvm.h>

PciConfigSpaceDevice* PciConfig = NULL;

CommonCmd* AdminSubQueue = NULL;
uint16_t AdminSubQueueDoorBellId = 0;
uint16_t AdminSubQueueCommandLength = 0;    //  clean after used

CommonCmd* AdminComQueue = NULL;
uint16_t AdminComQueueDoorBellId = 0;//
uint16_t AdminComQueueCommandLength = 0;    //  clean after used

uint64_t NvmDeviceMmioBaseAddress = 0x0;

uint64_t AdminSubDoorBell = 0x0;
uint64_t AdminComDoorBell = 0x0;

uint8_t DoorBellStep = 0;
int /*bool*/ PhysicalAddressEnabled = 1;  //  true

uint64_t debug()
{
    return ((NvmController*)NvmDeviceMmioBaseAddress)->Capabilities;
}

void restDoorBell()
{
    AdminSubQueueCommandLength = 0;
    AdminComQueueCommandLength = 0;//
}

int setPageSize(unsigned int PageSize)
{
    unsigned int PageBitSign = -1;
    if(!(PageSize&(PageSize-1)))
    while(PageSize)
    {
        PageSize >>= 1;
        PageBitSign++;
    }
    if(PageBitSign != -1)
    {
        ((NvmController*)NvmDeviceMmioBaseAddress)->ControllerConfiguration |= PageBitSign << 7;
        //debug
        return 0;
    }
    return -1;
}

int DeletIoSubmissionQueue(uint16_t QueueId)
{   
    // Admin Queue Id = 0,
    AdminSubQueue->CommandDWord0 = 0;
    AdminSubQueue->NameSpaceID = 0;
    AdminSubQueue->CommandDWord2 = 0;
    AdminSubQueue->CommandDWord3 = 0;
    AdminSubQueue->MetadataPtr = 0;
    AdminSubQueue->DataPtr[0] = 0;
    AdminSubQueue->DataPtr[1] = 0;
    AdminSubQueue->CommandDWord10 = QueueId;
    AdminSubQueue->CommandDWord11 = 0;
    AdminSubQueue->CommandDWord12 = 0;
    AdminSubQueue->CommandDWord13 = 0;
    AdminSubQueue->CommandDWord14 = 0;
    AdminSubQueue->CommandDWord15 = 0;
    AdminSubQueueCommandLength = 1;
    NvmAdminSubKnock();
    AdminSubQueueCommandLength = 0;
    //delelt
    return 0;
}

int CreateIoSubmissionQueue(uint16_t QueueId,uint16_t QueueSize)  //  max 1024
{
    AdminSubQueue->CommandDWord0 = 0x1; //  Command Create IO Submission    PRPs
    AdminSubQueue->NameSpaceID = 0;     //  ignored
    AdminSubQueue->CommandDWord2 = 0;   //  not used
    AdminSubQueue->CommandDWord3 = 0;   //  not used
    AdminSubQueue->MetadataPtr = 0;     //  not used    sector  operate
    AdminSubQueue->DataPtr[0] = 0;      //  PRP2 Entry  not used
    AdminSubQueue->DataPtr[1] = 0;      
    ((uint16_t*)&AdminSubQueue->CommandDWord10)[1] = QueueSize;
    ((uint16_t*)&AdminSubQueue->CommandDWord10)[0] = QueueId;
    AdminSubQueue->CommandDWord11 = 0 | PhysicalAddressEnabled;  //disableInterrupt | Enable Physical Address
    AdminSubQueue->CommandDWord12 = 0;   //  not used
    AdminSubQueue->CommandDWord13 = 0;   //  not used
    AdminSubQueue->CommandDWord14 = 0;   //  not used
    AdminSubQueue->CommandDWord15 = 0;   //  not used
    AdminSubQueueCommandLength = 1;
    NvmAdminSubKnock();
    AdminSubQueueCommandLength = 0;
    //write command
    //IoSubmissionQueueExist
    //DeleltOldOne
    //Creat New
    return 0;
}

int NvmAdminSubKnock()
{
    uint32_t* DoorBell = (uint32_t*)(NvmDeviceMmioBaseAddress + 0x1000);
    AdminSubQueueDoorBellId += AdminSubQueueCommandLength;
    *DoorBell = AdminSubQueueDoorBellId;    //  knock
    return 0;
}

int NvmKnock(uint16_t QueueDoorBell)
{

    return 0;   //  if not all of nvm data should be clean and the drive should reboot.at least,Queue Should Be Re Creat.
}

int NvmSubsystemLevelReset()
{
    return 0;   //  if use in efi boot bootservices has been destoryed,Disk access methord should not be use again
}

uint8_t getDoorBellStep()
{
    return (((uint8_t*)&((NvmController*)NvmDeviceMmioBaseAddress)->Capabilities)[4] & 0xF);   //  CAP.DSTRD
}

uint64_t getAdminSubQueueBaseAddress()
{
    return ((NvmController*)NvmDeviceMmioBaseAddress)->AdminSubmissionQueueBaseAddress;
}

uint64_t getAdminComQueueBaseAddress()
{
    return ((NvmController*)NvmDeviceMmioBaseAddress)->AdminCompletionQueueBaseAddress;
}

int enablePhysicalAddress()
{
    PhysicalAddressEnabled = 1;
    return 0;
}

int disablePhysicalAddress()
{
    PhysicalAddressEnabled = 0;
    return 0;
}

int initNvm(PciConfigSpaceDevice* SoPciConfig)
{
    PciConfig = SoPciConfig;
    //set DoorBell Step
    NvmDeviceMmioBaseAddress = *(uint64_t*)&(PciConfig->BaseAddressRegister_0);    //  only x86_64
    //set base address
    AdminSubQueue = getAdminSubQueueBaseAddress();
    AdminComQueue = getAdminComQueueBaseAddress();
    DoorBellStep = getDoorBellStep();
    NvmSubsystemLevelReset();
    return 0;
}