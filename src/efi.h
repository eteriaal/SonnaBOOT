// SonnaBOOT efi.h
// From UEFI specification 2.11
// void* in structs = not implemented!!
#pragma once
#include <stdint.h>
#include <stddef.h>

// Calling convention
#define EFIAPI __attribute__((ms_abi))

// Data types
#define FALSE 0
#define TRUE  1
typedef uint8_t   BOOLEAN;

typedef int64_t   INTN;
typedef uint64_t  UINTN;
typedef uint8_t   UINT8;
typedef uint16_t  UINT16;
typedef uint32_t  UINT32;
typedef uint64_t  UINT64;
typedef char      CHAR8;
typedef uint_least16_t char16_t;
typedef char16_t CHAR16;

typedef void      VOID;

typedef struct {
    UINT32  Data1;
    UINT16  Data2;
    UINT16  Data3;
    UINT8   Data4[8];
} EFI_GUID;

typedef UINTN     EFI_STATUS;
typedef VOID*     EFI_HANDLE;
typedef VOID*     EFI_EVENT;

// Modifiers
#define IN
#define OUT
#define OPTIONAL
#define CONST const

#define EFI_SUCCESS 0

// Text attributes
#define EFI_TEXT_ATTR(Foreground, Background) ((Foreground) | ((Background) << 4))

#define EFI_BLACK                0x00
#define EFI_BLUE                 0x01
#define EFI_GREEN                0x02
#define EFI_CYAN                 0x03
#define EFI_RED                  0x04
#define EFI_MAGENTA              0x05
#define EFI_BROWN                0x06
#define EFI_LIGHTGRAY            0x07

#define EFI_DARKGRAY             0x08
#define EFI_LIGHTBLUE            0x09
#define EFI_LIGHTGREEN           0x0A
#define EFI_LIGHTCYAN            0x0B
#define EFI_LIGHTRED             0x0C
#define EFI_LIGHTMAGENTA         0x0D
#define EFI_YELLOW               0x0E
#define EFI_WHITE                0x0F

#define EFI_BACKGROUND_BLACK     0x00
#define EFI_BACKGROUND_BLUE      0x10
#define EFI_BACKGROUND_GREEN     0x20
#define EFI_BACKGROUND_CYAN      0x30
#define EFI_BACKGROUND_RED       0x40
#define EFI_BACKGROUND_MAGENTA   0x50
#define EFI_BACKGROUND_BROWN     0x60
#define EFI_BACKGROUND_LIGHTGRAY 0x70


// Forward declarations
typedef struct EFI_SYSTEM_TABLE EFI_SYSTEM_TABLE;
typedef struct EFI_SIMPLE_TEXT_INPUT_PROTOCOL  EFI_SIMPLE_TEXT_INPUT_PROTOCOL;
typedef struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;
typedef struct EFI_RUNTIME_SERVICES EFI_RUNTIME_SERVICES;


typedef struct {
    UINT16     ScanCode;
    CHAR16     UnicodeChar;
} EFI_INPUT_KEY;

#define SCAN_UP   0x01
#define SCAN_DOWN 0x02
#define SCAN_LEFT 0x03
#define SCAN_RIGHT 0x04

struct EFI_SIMPLE_TEXT_INPUT_PROTOCOL {
    void* Reset;
    EFI_STATUS (EFIAPI *ReadKeyStroke)(
        IN EFI_SIMPLE_TEXT_INPUT_PROTOCOL *This,
        OUT EFI_INPUT_KEY *Key
    );
    EFI_EVENT WaitForKey;
};

struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {
    void* Reset;
    EFI_STATUS (EFIAPI *OutputString)(
        IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
        IN CONST CHAR16 *String
    );
    void* TestString;
    void* QueryMode;
    void* SetMode;
    EFI_STATUS (EFIAPI *SetAttribute)(
        IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
        IN UINTN Attribute
    );
    EFI_STATUS (EFIAPI *ClearScreen)(
        IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This
    );
    void* SetCursorPosition;
    void* EnableCursor;
    void* Mode;
};

typedef enum {
    EfiResetCold,
    EfiResetWarm,
    EfiResetShutdown,
    EfiResetPlatformSpecific
} EFI_RESET_TYPE;

typedef
VOID
(EFIAPI *EFI_RESET_SYSTEM) (
    IN EFI_RESET_TYPE   ResetType,
    IN EFI_STATUS       ResetStatus,
    IN UINTN            DataSize,
    IN VOID             *ResetData OPTIONAL
);

typedef struct {
    UINT64  Signature;
    UINT32  Revision;
    UINT32  HeaderSize;
    UINT32  CRC32;
    UINT32  Reserved;
} EFI_TABLE_HEADER;

static const EFI_GUID gEfiGlobalVariableGuid = {
    0x8be4df61, 0x93ca, 0x11d2,
    { 0xaa, 0x0d, 0x00, 0xe0, 0x98, 0x03, 0x2b, 0x8c }
};

// stub EFI_ERROR
#define EFI_ERROR(Status)  ((Status) < 0)

#define EFI_OS_INDICATIONS_BOOT_TO_FW_UI      0x0000000000000001ULL

#define EFI_VARIABLE_NON_VOLATILE             0x00000001
#define EFI_VARIABLE_BOOTSERVICE_ACCESS       0x00000002
#define EFI_VARIABLE_RUNTIME_ACCESS           0x00000004

typedef
EFI_STATUS
(EFIAPI *EFI_GET_VARIABLE) (
    IN  CHAR16            *VariableName,
    IN  EFI_GUID          *VendorGuid,
    OUT UINT32            *Attributes     OPTIONAL,
    IN  OUT UINTN         *DataSize,
    OUT VOID              *Data           OPTIONAL
);

typedef
EFI_STATUS
(EFIAPI *EFI_SET_VARIABLE) (
    IN  CHAR16            *VariableName,
    IN  EFI_GUID          *VendorGuid,
    IN  UINT32            Attributes,
    IN  UINTN             DataSize,
    IN  VOID              *Data
);

typedef
EFI_STATUS
(EFIAPI *EFI_GET_NEXT_VARIABLE_NAME) (
    IN  OUT UINTN         *VariableNameSize,
    IN  OUT CHAR16        *VariableName,
    IN  OUT EFI_GUID      *VendorGuid
);


struct EFI_RUNTIME_SERVICES {
    EFI_TABLE_HEADER Hdr;

    void* GetTime;
    void* SetTime;
    void* GetWakeupTime;
    void* SetWakeupTime;

    void* SetVirtualAddressMap;
    void* ConvertPointer;

    EFI_GET_VARIABLE GetVariable;
    EFI_GET_NEXT_VARIABLE_NAME GetNextVariableName;
    EFI_SET_VARIABLE SetVariable;

    void* GetNextHighMonotonicCount;

    EFI_RESET_SYSTEM ResetSystem;

    void* UpdateCapsule;
    void* QueryCapsuleCapabilities;
    void* QueryVariableInfo;
};

struct EFI_SYSTEM_TABLE {
    EFI_TABLE_HEADER                     Hdr;

    CHAR16                               *FirmwareVendor;
    UINT32                               FirmwareRevision;

    EFI_HANDLE                           ConsoleInHandle;
    EFI_SIMPLE_TEXT_INPUT_PROTOCOL       *ConIn;

    EFI_HANDLE                           ConsoleOutHandle;
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL      *ConOut;

    EFI_HANDLE                           StandardErrorHandle;
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL      *StdErr;

    EFI_RUNTIME_SERVICES                 *RuntimeServices;
    void*                                BootServices;

    UINTN                                NumberOfTableEntries;
    void*                                ConfigurationTable;
};