#include <Uefi.h>
#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/LoadedImage.h>
#include <Guid/Acpi.h>
#include <Guid/FileInfo.h>

#include <Bootloader.h>
#include <FileSystem.h>
#include <acpi_init/system_table.h>
#include <acpi_init/pcie/pci_config.h>

#include <Imageloader.h>
#include <nvm_pcie/lite_nvm.h>

#define __VM__ 1
#define __ORGIN_BOOT__ 1

EFI_STATUS Status;
extern EFI_BOOT_SERVICES* gBS;
EFI_STATUS EFIAPI UefiMain ( IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable  )
{
  EFI_LOADED_IMAGE* ImageInfo = NULL;
  Status = gBS->HandleProtocol(ImageHandle,&gEfiLoadedImageProtocolGuid,(VOID**)&ImageInfo);
  EFI_DEVICE_PATH* BootDevicePath = NULL;
  Status = gBS->HandleProtocol(ImageInfo->DeviceHandle,&gEfiDevicePathProtocolGuid,(VOID**)&BootDevicePath);
#ifdef __ORGIN_BOOT__
  goto BootDriectly;
#endif
  EFI_FILE_IO_INTERFACE* EfiSystemPartition = NULL;
  Status = gBS->HandleProtocol(ImageInfo->DeviceHandle,&gEfiSimpleFileSystemProtocolGuid,(VOID**)&EfiSystemPartition);

  EFI_FILE_HANDLE EspRootFolder,BcdFile;
  Status = EfiSystemPartition->OpenVolume(EfiSystemPartition,&EspRootFolder);
  Status = EspRootFolder->Open(EspRootFolder,(VOID**)&BcdFile,L"EFI\\Tinsoft\\BCD",EFI_FILE_MODE_READ,0);
  EFI_FILE_INFO* BcdFileInfo = NULL;
  UINTN BufSiz = (sizeof(EFI_FILE_INFO)+sizeof(CHAR16) * 100 /*sizeof(L"EFI\\Tinsoft\\BCD")*/ );
  Status = gBS->AllocatePool(EfiRuntimeServicesData,BufSiz,(VOID*)&BcdFileInfo);
  Status = EspRootFolder->GetInfo(EspRootFolder,&gEfiFileInfoGuid,&BufSiz,BcdFileInfo);
  BootConfigration* BootConfigrations = NULL;
#ifndef __VM__
  Status = gBS->AllocatePool(EfiRuntimeServicesData,BcdFileInfo->FileSize,(VOID*)&BootConfigrations);
  Status = BcdFile->Read(BcdFile,&BcdFileInfo->FileSize,(VOID*)BootConfigrations);
#endif
#ifdef __VM__
  Status = gBS->AllocatePool(EfiRuntimeServicesData,608,(VOID*)&BootConfigrations);
  UINTN aiohjfioaw = 32768; //  32kb
  Status = BcdFile->Read(BcdFile,&aiohjfioaw,(VOID*)BootConfigrations);
#endif
  BootConfigrationHead* BootConfigBaseAddr = (BootConfigrationHead*)BootConfigrations;
  UINT8* p = (UINT8*)BootConfigrations;
  for(int n = 0; n<4;n++)
  {
    for(int i = 0;i < 15;i++)
    {
      Print(L"%02x ",*p);
      p++;
    }
    Print(L"\n");
    p++;
  }
  CHAR16* ExcFile = NULL;
  EFI_DEVICE_PATH* EfiImage = NULL;
  EFI_HANDLE NewHandle = NULL;
  if((BootConfigBaseAddr->select ==0)||(BootConfigBaseAddr->select > BootConfigBaseAddr->number)) goto WalkThroughBCD;//bad BCD Head
  BootConfigrations += (UINT64)BootConfigBaseAddr->select;
  ExcFile = WideChar8((CHAR8*)(BootConfigrations->SysPathPtr + (UINT64)BootConfigBaseAddr));
  //test sepc
  if(FileExist(ExcFile,EspRootFolder))
  {
    switch (BootConfigrations->type)
    {
    case UEFI_APPLICATION:
      Print(L"%s\n",ExcFile);
      Status = EfiImage = FilePathToDevicePath(ImageInfo->DeviceHandle,ExcFile);
      Status = gBS->FreePool((VOID*)ExcFile);
      Status = gBS->LoadImage(FALSE,ImageHandle,EfiImage,NULL,0,&NewHandle);
      Status = gBS->StartImage(NewHandle,0,NULL);
      return EFI_SUCCESS;
      break;
    case OPERATE_SYS:
        goto BootDriectly;
      break;
    default:
      goto WalkThroughBCD; //bad BCD head
      break;
    }
  }
WalkThroughBCD:
  BootConfigrations = (BootConfigration*)BootConfigBaseAddr;
  BootConfigrations++;
  //find closest
  for(UINTN Index = 1;Index < BootConfigBaseAddr->number;Index++,BootConfigrations++)
  {
    if(BootConfigrations->type)
    {
      switch (BootConfigrations->type)
      {
      case UEFI_APPLICATION:
        ExcFile = WideChar8((CHAR8*)(BootConfigrations->SysPathPtr + (UINT64)BootConfigBaseAddr));
        Print(L"%s\n",ExcFile);
        if(FileExist(ExcFile,EspRootFolder))
        {
          Status = EfiImage = FilePathToDevicePath(ImageInfo->DeviceHandle,ExcFile);
          Status = gBS->FreePool((VOID*)ExcFile);
          Status = gBS->LoadImage(FALSE,ImageHandle,EfiImage,NULL,0,&NewHandle);
          Status = gBS->StartImage(NewHandle,0,NULL);
        }
        return EFI_SUCCESS;
        break;
      case OPERATE_SYS:
        goto BootDriectly;
        break;
      default:
        //rsvd typ
        break;
      }
    }
  }
  return EFI_NOT_FOUND; //debug
BootDriectly:
  EFI_CONFIGURATION_TABLE  *EfiConfigurationTable = NULL;
  BOOLEAN FoundAcpiTable = FALSE;
  for (UINTN Index = 0; Index < SystemTable->NumberOfTableEntries; Index++) 
  {
    if (CompareGuid (&gEfiAcpiTableGuid,&(SystemTable->ConfigurationTable[Index].VendorGuid)))
    {
      EfiConfigurationTable = &SystemTable->ConfigurationTable[Index];
      FoundAcpiTable = TRUE;
      break;
    }
  }
  if(!FoundAcpiTable)
  {
    return EFI_UNSUPPORTED;
  }
  RSDP* Rsdp = (RSDP*)EfiConfigurationTable->VendorTable;
  if(Rsdp->Revision < 2)
  {
    return EFI_UNSUPPORTED;
  }
  Print(L"XSDT addr:%x\n",Rsdp->XsdtAddr);
  XSDT* ExtSysDecTable = (XSDT*)(Rsdp->XsdtAddr);
  INT32 EntryCount = (ExtSysDecTable->Header.Length-36)>>3;
  Print(L"EntryCount: %d\n",EntryCount);
  UINT64* EntryPtr = (UINT64*)ExtSysDecTable->Entry;
  BOOLEAN FoundMcfg = FALSE;
  for(UINTN Index = 0;Index < EntryCount;Index++,EntryPtr++)
  {
    if(AcpiTableSignIs(*EntryPtr,&McfgSign))
    {
      FoundMcfg = TRUE;
      break;
    }
  }
  if(!FoundMcfg)
  {
    return EFI_UNSUPPORTED;
  }
  Print(L"%lx: ",*EntryPtr);
  Print(L"%lx\n",*(UINT64*)*EntryPtr);
  MCFG* Mcfg = (MCFG*)*EntryPtr;
  Print(L"0x%016lx%\n",(UINT64)*(UINT64*)Mcfg->BaseAddressOfEcam);
  INT32 EcamCount = (Mcfg->Header.Length-44)>>4;
  ECAM* EcamPool = (ECAM*)Mcfg->BaseAddressOfEcam;

//
  ACPI_HID_DEVICE_PATH* PCI_ROOT = (ACPI_HID_DEVICE_PATH*)BootDevicePath;
  INT32 PCI_BUS = PCI_ROOT->UID;
  *(INT16*)&BootDevicePath += (UINT64)(UINT16)*BootDevicePath->Length;  //*(INT16*)& cancel displacement caused by cast.
  PCI_DEVICE_PATH* PCI_BF;
  INT32 PCI_DEV;
  INT32 PCI_FUN;
  INT64 PCI_BDF_DISP;
  INT32 PciNodeCount = 0;
  PciConfigSpaceBridge* PciConfig1 = NULL;
  //get Disk BDF
  do
  {
    PCI_BF = (PCI_DEVICE_PATH*)BootDevicePath;
    PCI_DEV =  PCI_BF->Device;
    PCI_FUN =  PCI_BF->Function;
    PCI_BDF_DISP = (PCI_BUS<<20)|(PCI_DEV<<15)|(PCI_FUN<<12);
    for(UINTN Index =0; Index<EcamCount; Index++)
    {
      if((EcamPool[Index].StartPciBusNum <= PCI_BUS) && (PCI_BUS<=EcamPool[Index].EndPciBusNum))
      {
        PciConfig1 = (UINT64)EcamPool[Index].BassAdressOfEcam;
        Print(L"ecam::0x%016lx\n",(UINT64)EcamPool[Index].BassAdressOfEcam);
        PciConfig1 = (UINT64)PciConfig1 | PCI_BDF_DISP;
        PciConfig1->Command |=( MEM_SPACE | BUS_MASTER );  // enable PCIe Bus Function
        Print(L"PciConfigAddr:0x%016lx\n",PciConfig1);
        PCI_BUS = PciConfig1->SubordinateBusNumber;
        break;
      }
    }
    *(INT16*)&BootDevicePath += (UINT64)(UINT16)*BootDevicePath->Length;  //next node
    PciNodeCount++;
  } while ((BootDevicePath->Type==HARDWARE_DEVICE_PATH) && (BootDevicePath->SubType==HW_PCI_DP));
  PciConfig1->Command |=( MEM_SPACE | BUS_MASTER | INTERRUPT_DISABLE);  //enable PCIe Device Function
  Print(L"PCI node count: %d\n",PciNodeCount);

  if((BootDevicePath->Type==MESSAGING_DEVICE_PATH)&&(BootDevicePath->SubType==MSG_NVME_NAMESPACE_DP))
  {
    //NVME_NAMESPACE_DEVICE_PATH* NvmDevicePath = (NVME_NAMESPACE_DEVICE_PATH*)&BootDevicePath;
    //UEFI BootServices may not useable if execute this
    initNvm(PciConfig1);
    setPageSize(4096);
    CreateIoSubmissionQueue(1,64);
    Print(L"AdminSubQueueBaseAddress:%016lx\nAdminComQueueBaseAddress:%016lx\nController Capabilities:%016lx\nDoorBellStep:%016lx\n",getAdminSubQueueBaseAddress(),getAdminComQueueBaseAddress(),debug(),getDoorBellStep());
    Print(L"Nvm!\n");
  }
  else if(0)
  {
    //if is removable stroge media.
  }
  else
  {
    //not support
  }

  Print(L"0x%016lx\n",PCI_BDF_DISP);
  //EFI_FILE_IO_INTERFACE* Volume;
  return EFI_SUCCESS;
}
