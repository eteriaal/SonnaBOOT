#include "efi.h"

static void draw_menu(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *cout, INTN selected) {
    cout->ClearScreen(cout);

    cout->SetAttribute(cout, EFI_TEXT_ATTR(EFI_WHITE, EFI_BLACK));
    cout->OutputString(cout, u"SonnaBOOT\r\n\r\n");

    const CHAR16* entries[] = {
        u"Boot kernel",
        u"Shutdown",
        u"Reboot"
    };

    for (INTN i = 0; i < 4; i++) {
        if (i == selected) {
            cout->OutputString(cout, u">");
        }
        cout->OutputString(cout, entries[i]);
        cout->OutputString(cout, u"\r\n");
    }
}

static INTN menu_loop(EFI_SYSTEM_TABLE *ST) {
    EFI_SIMPLE_TEXT_INPUT_PROTOCOL *cin = ST->ConIn;
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *cout = ST->ConOut;
    EFI_INPUT_KEY key;

    INTN selected = 0;

    draw_menu(cout, selected);

    while (1) {
        if (cin->ReadKeyStroke(cin, &key) == EFI_SUCCESS) {

            if (key.ScanCode == SCAN_UP) {
                if (selected > 0) selected--;
                draw_menu(cout, selected);
            }

            else if (key.ScanCode == SCAN_DOWN) {
                if (selected < 2) selected++;
                draw_menu(cout, selected);
            }

            else if (key.UnicodeChar == L'\r') {
                return selected;
            }
        }
    }
}


EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    (void)ImageHandle; // to prevent unused parameter

    EFI_SYSTEM_TABLE *ST = SystemTable;
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *cout = ST->ConOut;
    EFI_SIMPLE_TEXT_INPUT_PROTOCOL *cin = ST->ConIn;

    INTN choice = menu_loop(ST);

    switch (choice) {
        case 0:
            // stub
            cout->ClearScreen(cout);
            cout->OutputString(cout, u"Booting kernel...\r\n");
            while (1);
            break;

        case 1:
            ST->RuntimeServices->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, NULL);
            break;

        case 2:
            ST->RuntimeServices->ResetSystem(EfiResetCold, EFI_SUCCESS, 0, NULL);
            break;
    }

    return EFI_SUCCESS;
}