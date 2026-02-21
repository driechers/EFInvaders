#include "keyboard.h"

#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/SimpleTextInEx.h>


static EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *TextInEx;

/*
TODO:
LSHIFT  l
RSHIFT  r
ENTER   e
*/
static CHAR16 decode(EFI_KEY_DATA KeyData)
{
    if(KeyData.Key.ScanCode == SCAN_UP) return L'^';
    if(KeyData.Key.ScanCode == SCAN_DOWN) return L'v';
    if(KeyData.Key.ScanCode == SCAN_LEFT) return L'<';
    if(KeyData.Key.ScanCode == SCAN_RIGHT) return L'>';

    return KeyData.Key.UnicodeChar;
}

EFI_STATUS key_decode(CHAR16 *key, BOOLEAN *pressed)
{
    static CHAR16 latest_key = 0;
    UINTN index;

    EFI_STATUS status = EFI_SUCCESS;
    EFI_KEY_DATA KeyData;

    gBS->WaitForEvent(1, &TextInEx->WaitForKeyEx, &index);
    status = TextInEx->ReadKeyStrokeEx(TextInEx, &KeyData);

    if(status == EFI_SUCCESS) {
        *key = decode(KeyData);
        latest_key = *key;
	*pressed = TRUE;
	return EFI_SUCCESS;
    }
    else if (status == EFI_NOT_READY) {
        *key = latest_key;
	*pressed = FALSE;
	return EFI_SUCCESS;
    }

    return status;
}

EFI_STATUS key_initialize()
{
    EFI_STATUS status;

    // Locate the Protocol
    status = gBS->LocateProtocol(&gEfiSimpleTextInputExProtocolGuid, NULL, (VOID **)&TextInEx);
    return status;
}
