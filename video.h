#ifndef __VIDEO_H
#define __VIDEO_H

#include <Uefi.h>

// effect on virtual buffer only
void video_usecolor(UINT8 fg, UINT8 bg);
void video_putchar(INT32 x, INT32 y, CHAR16 code);
void video_fill(INT32 x, INT32 y, INT32 width, INT32 height, CHAR16 code);
//void video_puthex8(INT32 x, INT32 y, UINT8 hex);
void video_putstring(INT32 x, INT32 y, CHAR16 *str);

// immediate effect
void video_update();
//void video_blinkchars(BOOLEAN onoff);
void video_hidecursor();
void video_poscursor(INT32 x, INT32 y); // out of screen -> video_hidecursor
void video_initialize(
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  );

#endif
