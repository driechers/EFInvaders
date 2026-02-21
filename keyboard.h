#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include <Uefi.h>

/*

ESC     @
LEFT    <
RIGHT   >
UP      ^
DOWN    v
LSHIFT  l
RSHIFT  r
ENTER   e
F1-F10  0123456789
SPACE   ' '
NOTHING x

only marks will be returned, releases ignored.

*/

EFI_STATUS key_decode(CHAR16 *key, BOOLEAN *pressed);
EFI_STATUS key_initialize();

#endif
