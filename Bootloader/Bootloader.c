#include <Uefi.h>
#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Protocol/LoadedImage.h>

#include <Bootloader.h>
#include <FileSystem.h>

EFI_STATUS Status;
extern EFI_BOOT_SERVICES* gBS;

EFI_STATUS EFIAPI UefiMain ( IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable  )
{
  EFI_LOADED_IMAGE* ImageInfo = NULL;
  Status = gBS->HandleProtocol(ImageHandle,&gEfiLoadedImageProtocolGuid,(VOID**)&ImageInfo);
  EFI_DEVICE_PATH* BootDevicePath = NULL;
  Status = gBS->OpenProtocol(ImageInfo->DeviceHandle,&gEfiDevicePathProtocolGuid,(VOID**)&BootDevicePath,ImageHandle,NULL,EFI_OPEN_PROTOCOL_GET_PROTOCOL);
  //RootPath is ACPI_HID
  Print(L"%d\n%d\n",BootDevicePath->Type,BootDevicePath->SubType);
  ACPI_HID_DEVICE_PATH* PCI_ROOT = (ACPI_HID_DEVICE_PATH*)BootDevicePath;
  //PCI_bus_num is PCI_ROOT->UID
  Print(L"%d\n%d\n",PCI_ROOT->HID,PCI_ROOT->UID);
  //EFI_FILE_IO_INTERFACE* Volume;
  return EFI_SUCCESS;
}
