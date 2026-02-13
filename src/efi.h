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

#define EFI_BLACK     0x00
#define EFI_BLUE      0x01
#define EFI_GREEN     0x02
#define EFI_RED       0x04
#define EFI_YELLOW    0x0E
#define EFI_WHITE     0x0F

// Forward declarations
typedef struct EFI_SYSTEM_TABLE          EFI_SYSTEM_TABLE;
typedef struct EFI_SIMPLE_TEXT_INPUT_PROTOCOL  EFI_SIMPLE_TEXT_INPUT_PROTOCOL;
typedef struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;
typedef struct EFI_RUNTIME_SERVICES      EFI_RUNTIME_SERVICES;


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


struct EFI_RUNTIME_SERVICES {
    EFI_TABLE_HEADER Hdr;

    void* GetTime;
    void* SetTime;
    void* GetWakeupTime;
    void* SetWakeupTime;

    void* SetVirtualAddressMap;
    void* ConvertPointer;

    void* GetVariable;
    void* GetNextVariableName;
    void* SetVariable;

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