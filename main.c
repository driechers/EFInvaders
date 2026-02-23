#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>

#include "video.h"
#include "keyboard.h"
#include "game.h"

EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS status = EFI_SUCCESS;

  video_initialize(ImageHandle, SystemTable);
  status = key_initialize();

  game();

  return EFI_SUCCESS;
}
