#include "video.h"
#include <Uefi.h>

static EFI_HANDLE ImageHandle;
static EFI_SYSTEM_TABLE  *SystemTable;

//static const UINT8 hextab[16] = {'0','1','2','3','4','5','6','7',
//				 '8','9','A','B','C','D','E','F'};

void video_usecolor(UINT8 fg, UINT8 bg)
{
  SystemTable->ConOut->SetAttribute(SystemTable->ConOut, (fg & 15) | (bg << 4));
};

void video_putchar(INT32 x, INT32 y, CHAR16 code)
{
  if ((x<0) || (y<0) || (x>=80) || (y>=25)) return;

  CHAR16 str[2] = { code, L'\0' };
  SystemTable->ConOut->SetCursorPosition(SystemTable->ConOut, x, y);
  SystemTable->ConOut->OutputString(SystemTable->ConOut, str);
};

void video_fill(INT32 x, INT32 y, INT32 width, INT32 height, CHAR16 code)
{
  INT32 a, b;

  for (a = x; a < x+width; ++a)
    for (b = y; b < y+height; ++b)
      video_putchar (a, b, code);
};

//void video_puthex8(INT32 x, INT32 y, UINT8 hex)
//{
//  video_putchar(x,y,hextab[hex>>4]);
//  video_putchar(x+1,y,hextab[hex&15]);
//};

void video_putstring(INT32 x, INT32 y, CHAR16 *str)
{
  SystemTable->ConOut->SetCursorPosition(SystemTable->ConOut, x, y);
  SystemTable->ConOut->OutputString(SystemTable->ConOut, str);
};

void video_update()
{
};

//void video_blinkchars(BOOLEAN onoff)
//{
//TODO
//};

static void video_visiblecursor (BOOLEAN onoff)
{
  SystemTable->ConOut->EnableCursor(SystemTable->ConOut, onoff);
};

void video_hidecursor()
{
  video_visiblecursor (FALSE);
};

void video_poscursor(INT32 x, INT32 y)
{
  if ((x<0) || (y<0) || (x>=80) || (y>=25)) {
    video_hidecursor ();
  }else{
    SystemTable->ConOut->SetCursorPosition(SystemTable->ConOut, x, y);
    video_visiblecursor(TRUE);
  };
};

void video_initialize(
  IN EFI_HANDLE        _ImageHandle,
  IN EFI_SYSTEM_TABLE  *_SystemTable)
{
  ImageHandle = _ImageHandle;
  SystemTable = _SystemTable;
}
