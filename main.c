#include <Uefi.h>
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
  video_putstring(0,0, L"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
  status = key_initialize();
  if(status == EFI_SUCCESS)
      video_putstring(0,0, L"BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB");
  else
      video_putstring(0,0, L"IIIIIIIIIIIIIIIIIIIIIIIIIIIIIII");

  video_usecolor(5,0);

  CHAR16 key = 0;
  BOOLEAN pressed = FALSE;
  video_putstring(0,0, L"CCCCCCCCCCCCCCCCCC");

  while(key != L'q') {
    video_putstring(1,1, L"########################################################");
    status = key_decode(&key, &pressed);
    if(status == EFI_SUCCESS) {
        video_putstring(1,1, L"DDDDDDDDDDDDDD");
	video_putchar(2,2, key);

        if(key == L' ') {
            if(pressed)
                video_putstring(5,5, L"Hello friggen world!!!!!!!!!!!!!!!!");
            else
                video_fill(0,0,30,30, L' ');
        }
    }
    else if (status == EFI_NOT_READY)
        video_putstring(1,1, L"NNNNNNNNN");
    else
        break;

  }

  return EFI_SUCCESS;
}
