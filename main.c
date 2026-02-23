#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>

#include "video.h"
#include "keyboard.h"

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

  video_usecolor(5,0);

  CHAR16 key = 0;
  BOOLEAN pressed = FALSE;

  video_fill(0,0,30,30, L' ');
  while(key != L'q') {
    status = key_decode(&key, &pressed);
    if(status == EFI_SUCCESS) {
	video_putchar(2,2, key);

        if(key == L' ') {
            if(pressed)
                video_putstring(5,5, L"Hello friggen world!!!!!!!!!!!!!!!!");
            else
                video_fill(0,0,30,30, L' ');
        }
	else if(key == L'c')
            video_fill(0,0,30,30, L' ');
    }
    else {
	Print(L"\nkey_decode %r\n", status);
        break;
    }
    gBS->Stall(1000);  // 1 millisecond

  }

  return EFI_SUCCESS;
}
