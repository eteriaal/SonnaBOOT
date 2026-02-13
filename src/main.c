#include "efi.h"

EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
        (void)ImageHandle; // to prevent unused parameter

        EFI_SYSTEM_TABLE *ST = SystemTable;
        ST->ConOut->ClearScreen(ST->ConOut);
        ST->ConOut->SetAttribute(ST->ConOut, EFI_TEXT_ATTR(EFI_WHITE, EFI_BLACK));
        ST->ConOut->OutputString(ST->ConOut, u"SonnaBOOT\r\n");
        ST->ConOut->OutputString(ST->ConOut, u"Press any key to exit\r\n");

        EFI_INPUT_KEY key;
        while (ST->ConIn->ReadKeyStroke(ST->ConIn, &key) != EFI_SUCCESS);

        ST->RuntimeServices->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, NULL);

        return EFI_SUCCESS;
}