#include <Uefi.h>
#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Protocol/LoadedImage.h>
#include <Guid/Acpi.h>
#include <Library/BaseMemoryLib.h>

#include <Bootloader.h>
#include <FileSystem.h>
#include <acpi_init/system_table.h>

EFI_STATUS Status;
extern EFI_BOOT_SERVICES* gBS;
EFI_STATUS EFIAPI UefiMain ( IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable  )
{
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

  for(UINTN Index = 0;Index < EntryCount;Index++,EntryPtr++)
  {
    if(AcpiTableSignIs(*EntryPtr,&McfgSign)) break;
  }
  Print(L"%lx: ",*EntryPtr);
  Print(L"%lx\n",*(UINT64*)*EntryPtr);
  MCFG* Mcfg = (MCFG*)*EntryPtr;
  Print(L"0x%016%\n",(UINT64)*Mcfg->BassAdressOfEcam);
  //load acpi table


  EFI_LOADED_IMAGE* ImageInfo = NULL;
  Status = gBS->HandleProtocol(ImageHandle,&gEfiLoadedImageProtocolGuid,(VOID**)&ImageInfo);
  EFI_DEVICE_PATH* BootDevicePath = NULL;
  Status = gBS->HandleProtocol(ImageInfo->DeviceHandle,&gEfiDevicePathProtocolGuid,(VOID**)&BootDevicePath);
  ACPI_HID_DEVICE_PATH* PCI_ROOT = (ACPI_HID_DEVICE_PATH*)BootDevicePath;
  INT32 PCI_BUS = PCI_ROOT->UID;
  *(INT16*)&BootDevicePath += (UINT64)(UINT16)*BootDevicePath->Length;  //*(INT16*)& cancel displacement caused by cast.
  //EFI_DEVICE_PATH* BootDeviceRootNode = BootDevicePath; //save node
  PCI_DEVICE_PATH* PCI_BF = (PCI_DEVICE_PATH*)BootDevicePath;
  INT32 PCI_DEV =  PCI_BF->Device;
  INT32 PCI_FUN =  PCI_BF->Function;
  INT64 PCI_BDF_DISP = (PCI_BUS<<20)|(PCI_DEV<<15)|(PCI_FUN<<12);  //PCI Root
  INT32 PciNodeCount = 0;
  do
  {
    *(INT16*)&BootDevicePath += (UINT64)(UINT16)*BootDevicePath->Length;  //next node
    PciNodeCount++;
    /*
      record next bus;
    */
  } while ((BootDevicePath->Type==HARDWARE_DEVICE_PATH) && (BootDevicePath->SubType==HW_PCI_DP));
  Print(L"PCI node count: %d\n",PciNodeCount);

  if((BootDevicePath->Type==MESSAGING_DEVICE_PATH)&&(BootDevicePath->SubType==MSG_NVME_NAMESPACE_DP))
  {
    //NVME_NAMESPACE_DEVICE_PATH* NvmDevicePath = (NVME_NAMESPACE_DEVICE_PATH*)&BootDevicePath;
    Print(L"Yes!Yes!Yes!\n");
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
