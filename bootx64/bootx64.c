#include <Uefi.h>
#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Guid/FileInfo.h>
#include <Protocol/BlockIo.h>
#include <Protocol/BootManagerPolicy.h>
#include <Protocol/DevicePathToText.h>
#include <Protocol/LoadedImage.h>

extern EFI_BOOT_SERVICES *gBS;
EFI_STATUS Status;

EFI_MEMORY_DESCRIPTOR* MemMap = 0;
UINTN MemMapSiz = 0;
UINTN MapKey = 0;
UINTN DscSiz = 0;
UINT32 DscVer = 0;

EFI_STATUS EFIAPI UefiMain (  IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE  *SystemTable )
{
  //get boot device PCI BDF(0:0:0)
  //load BCD（BootConfigurationData）
  //load basic fileSystem drive
  //send read kenrel instruction to media
  //report to kenrel SystemTable and MemoryMapping
  UINT8* BootLoader;
  BootLoader = (UINT8*)0x80000000;//set kenrel base address
  //Free
  gBS->GetMemoryMap(&MemMapSiz,MemMap,&MapKey,&DscSiz,&DscVer);//get length
  gBS->AllocatePool(EfiReservedMemoryType,MemMapSiz,(VOID**)&MemMap);
  gBS->GetMemoryMap(&MemMapSiz,MemMap,&MapKey,&DscSiz,&DscVer);//save msg
  gBS->FreePool(MemMap);
  gBS->GetMemoryMap(&MemMapSiz,MemMap,&MapKey,&DscSiz,&DscVer);//fresh MemMap
  Status = gBS->ExitBootServices(ImageHandle,MapKey);
  if(EFI_ERROR(Status))
  {
    SystemTable->ConOut->OutputString(SystemTable->ConOut,L"Invaid Memory Map.\n");
    Exit(EFI_INVALID_PARAMETER);
  }
  //call Kenrel
  void (*bootloader)(void);
  bootloader = (void(*)(void))0x80000000;
  bootloader();
  return EFI_SUCCESS;
}
