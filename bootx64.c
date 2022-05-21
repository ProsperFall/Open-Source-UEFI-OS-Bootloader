#include <Uefi.h>
#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Guid/FileInfo.h>
#include <Protocol/BlockIo.h>
#include <Protocol/BootManagerPolicy.h>
#include <Protocol/DevicePathToText.h>
#include <Protocol/LoadedImage.h>

typedef struct {
  UINT8 Type;
  UINT8 SubType; 
  UINT16 Length;
} Device_Path;

extern EFI_BOOT_SERVICES *gBS;
EFI_STATUS Status;

EFI_MEMORY_DESCRIPTOR* MemMap = 0;
UINTN MemMapSiz = 0;
UINTN MapKey = 0;
UINTN DscSiz = 0;
UINT32 DscVer = 0;

EFI_STATUS EFIAPI SetUpThisDisk()
{
  Print(L"QAQ");
  Exit(Status);
  return EFI_SUCCESS;
}

EFI_STATUS EFIAPI UefiMain (  IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE  *SystemTable )
{
  EFI_LOADED_IMAGE* ImageInfo;//ImageInfo->DeviceHandle
  Status = gBS -> HandleProtocol(ImageHandle,&gEfiLoadedImageProtocolGuid,(VOID**)&ImageInfo);
  if(EFI_ERROR(Status))
  {
    SystemTable->ConOut->OutputString(SystemTable->ConOut,L"Boot Device not found\n");
    Exit(Status);
  }
  
  EFI_FILE_IO_INTERFACE* Volume;//Access ESP Volume
  Status = gBS -> HandleProtocol(ImageInfo->DeviceHandle,&gEfiSimpleFileSystemProtocolGuid,(VOID**)&Volume);
  if(EFI_ERROR(Status))
  {
    SystemTable->ConOut->OutputString(SystemTable->ConOut,L"Destroyed disk!\n");
    Exit(Status);
  }

  EFI_FILE_HANDLE FileSystemRootPath;
  Volume->OpenVolume(Volume,&FileSystemRootPath);
  
  EFI_FILE_HANDLE BootCacheFile;
  Status = FileSystemRootPath->Open(FileSystemRootPath,(EFI_FILE**)&BootCacheFile,L".iTache_",EFI_FILE_MODE_READ|EFI_FILE_MODE_WRITE|EFI_FILE_MODE_CREATE,0);
  //While the file does not exist Creat it.
  if(EFI_ERROR(Status))
  {
    SystemTable->ConOut->OutputString(SystemTable->ConOut,L"While booting Error Occur.\n");
    Exit(Status);
  }
  
  UINTN BufferSize = sizeof(EFI_FILE_INFO) + sizeof(CHAR16) * 100;
  EFI_FILE_INFO* FileInfo;
  Status = gBS->AllocatePool(EfiRuntimeServicesData,BufferSize,(VOID**)&FileInfo);
  if(EFI_ERROR(Status))
  {
    SystemTable->ConOut->OutputString(SystemTable->ConOut,L"Memory Allocate Error Occured.\n");
    Exit(Status);
  } 

  BootCacheFile->GetInfo(BootCacheFile,&gEfiFileInfoGuid,&BufferSize,FileInfo);
  BufferSize = FileInfo->FileSize;

ReadAgain:
  Status = BootCacheFile->Read(BootCacheFile,&BufferSize,(VOID*)0x80000000);//bootloader

  //EFI_DEVICE_PATH* DevicePath;
  HARDDRIVE_DEVICE_PATH* DevicePath;
  DevicePath = (HARDDRIVE_DEVICE_PATH*)0x80000000;

  EFI_DEVICE_PATH_TO_TEXT_PROTOCOL* DevicePathChecker;
  Status = gBS->LocateProtocol(&gEfiDevicePathToTextProtocolGuid,NULL,(VOID**)&DevicePathChecker);
  if(DevicePathChecker->ConvertDevicePathToText(DevicePath,TRUE,FALSE) == NULL)
  {
    SetUpThisDisk();
  }

  EFI_HANDLE DeviceHandle;
  Status = gBS -> LocateDevicePath(&gEfiBlockIoProtocolGuid,(EFI_DEVICE_PATH**)&DevicePath,&DeviceHandle);//means locate by DevPath
  if(EFI_ERROR(Status))
  {
    SetUpThisDisk();
  }
  //Open BlockProtocol
  EFI_BLOCK_IO_PROTOCOL* BlockIO;
  Status = gBS -> HandleProtocol(DeviceHandle,&gEfiBlockIoProtocolGuid,(VOID**)&BlockIO);
  if(EFI_ERROR(Status))
  {
    SystemTable->ConOut->OutputString(SystemTable->ConOut,L"Device not support BlockIO.\n");
    Exit(Status);
  }
UINT8* BootLoader;
  BootLoader = (UINT8*)0x80000000;//2,147,483,648 Bytes 2GB
  Status = BlockIO->ReadBlocks(
    BlockIO,
    BlockIO->Media->MediaId,
    0,//LBA
    BlockIO->Media->BlockSize,
    (VOID**)BootLoader
  );
  for(int n = 0; n < 4096 ; n++)
  {
    Print(L"%x ",*BootLoader);
    BootLoader++;
  }
  //Get Memory Map
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
  void (*bootloader)(void);
  bootloader = (void(*)(void))0x80000000;
  bootloader();
  goto ReadAgain;
  return EFI_SUCCESS;
}
