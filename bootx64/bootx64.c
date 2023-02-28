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
  EFI_LOADED_IMAGE* ImageInfo;
  Status = gBS -> HandleProtocol(ImageHandle,&gEfiLoadedImageProtocolGuid,(VOID**)&ImageInfo);
  
  EFI_FILE_IO_INTERFACE* Volume;//Access ESP Volume
  Status = gBS -> HandleProtocol(ImageInfo->DeviceHandle,&gEfiSimpleFileSystemProtocolGuid,(VOID**)&Volume);

  EFI_FILE_HANDLE FileSystemRootPath;
  Volume->OpenVolume(Volume,&FileSystemRootPath);
  EFI_FILE_HANDLE BootConfigData;
  Status = FileSystemRootPath->Open(FileSystemRootPath,(EFI_FILE**)&BootConfigData,L"BCD",EFI_FILE_MODE_READ/*|EFI_FILE_MODE_WRITE|EFI_FILE_MODE_CREATE*/,0);
  
  UINTN BufferSize = sizeof(EFI_FILE_INFO) + sizeof(CHAR16) * 100;
  EFI_FILE_INFO* FileInfo;
  Status = gBS->AllocatePool(EfiRuntimeServicesData,BufferSize,(VOID**)&FileInfo);

  BootConfigData->GetInfo(BootConfigData,&gEfiFileInfoGuid,&BufferSize,FileInfo);
  CHAR8* LoadedBootConfigData;
  Status = gBS->AllocatePool(EfiRuntimeServicesData,FileInfo->FileSize,(VOID**)&LoadedBootConfigData);

  Status = BootConfigData->Read(BootConfigData,&BufferSize,(VOID*)&LoadedBootConfigData);

  EFI_DEVICE_PATH_TO_TEXT_PROTOCOL* DevicePathChecker;

  EFI_HANDLE DeviceHandle;

  //Open BlockProtocol
  EFI_BLOCK_IO_PROTOCOL* BlockIO;
  Status = gBS -> HandleProtocol(DeviceHandle,&gEfiBlockIoProtocolGuid,(VOID**)&BlockIO);

  //Get Memory Map
  gBS->GetMemoryMap(&MemMapSiz,MemMap,&MapKey,&DscSiz,&DscVer);//get length
  gBS->AllocatePool(EfiReservedMemoryType,MemMapSiz,(VOID**)&MemMap);
  gBS->GetMemoryMap(&MemMapSiz,MemMap,&MapKey,&DscSiz,&DscVer);//save msg
  gBS->FreePool(MemMap);
  gBS->GetMemoryMap(&MemMapSiz,MemMap,&MapKey,&DscSiz,&DscVer);//fresh MemMap
  Status = gBS->ExitBootServices(ImageHandle,MapKey);

  void (*bootloader)(void);
  bootloader = (void(*)(void))0x80000000;
  bootloader();
  return EFI_SUCCESS;
}
