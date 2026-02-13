#include "efi.h"

#define UNSELECTED_COL EFI_TEXT_ATTR(EFI_LIGHTGRAY, EFI_BLACK)
#define SELECTED_COL   EFI_TEXT_ATTR(EFI_WHITE, EFI_BLACK)

static void draw_menu(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *cout, INTN selected) {
    cout->ClearScreen(cout);
    cout->SetAttribute(cout, EFI_TEXT_ATTR(EFI_CYAN, EFI_BLACK));
    cout->OutputString(cout, u"SonnaBOOT\r\n\r\n");

    const CHAR16* entries[] = {
        u"Boot kernel",
        u"Shutdown",
        u"Reboot",
        u"Reboot to UEFI Firmware"
    };

    cout->SetAttribute(cout, UNSELECTED_COL);

    for (INTN i = 0; i < 4; i++) {
        cout->OutputString(cout, u"  ");
        if (i == selected) {
            // selected entry
            cout->SetAttribute(cout, SELECTED_COL);
            cout->OutputString(cout, entries[i]);
            cout->OutputString(cout, u"\r\n");

            // back default color!
            cout->SetAttribute(cout, UNSELECTED_COL);
        }
        else {
            // unselected entry
            cout->OutputString(cout, entries[i]);
            cout->OutputString(cout, u"\r\n");
        }
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
                if (selected < 3) selected++;
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

    INTN choice = menu_loop(ST);

    switch (choice) {
        case 0: // boot to kernel
            // stub
            cout->ClearScreen(cout);
            cout->OutputString(cout, u"Booting kernel...\r\n");
            while (1);
            break;

        case 1: // shutdown
            ST->RuntimeServices->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, NULL);
            break;

        case 2: // reboot
            ST->RuntimeServices->ResetSystem(EfiResetWarm, EFI_SUCCESS, 0, NULL);
            break;
        case 3: { // reboot to UEFI Firmware
            EFI_STATUS Status;
            UINT64 OsInd = 0;
            UINTN Size = sizeof(UINT64);
            UINT32 Attr;

            // read OsIndicators from NVRAM (if not found = 0)
            Status = ST->RuntimeServices->GetVariable(
                L"OsIndications",
                (EFI_GUID*)&gEfiGlobalVariableGuid,
                &Attr,
                &Size,
                &OsInd
            );

            // set flag to boot in fw
            OsInd |= EFI_OS_INDICATIONS_BOOT_TO_FW_UI;

            // write back to NVRAM
            Status = ST->RuntimeServices->SetVariable(
                L"OsIndications",
                (EFI_GUID*)&gEfiGlobalVariableGuid,
                EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                sizeof(UINT64),
                &OsInd
            );

            if (EFI_ERROR(Status)) {
                cout->OutputString(cout, L"Cannot set OsIndications\r\n");
                while(1);
            }

            ST->RuntimeServices->ResetSystem(EfiResetWarm, EFI_SUCCESS, 0, NULL);
            break;
        }
    }

    return EFI_SUCCESS;
}