.PHONY: all esp run clean

CC = clang

CFLAGS = -target x86_64-pc-windows-gnu \
         -std=c17 \
         -ffreestanding \
         -fno-stack-protector \
         -fshort-wchar \
         -mno-red-zone \
         -nostdlib \
         -Wall -Wextra


TARGET = BOOTX64.EFI
BUILD_DIR = build
TARGET_PATH = $(BUILD_DIR)/$(TARGET)

ifeq ($(OS),Windows_NT)

SOURCES := $(subst \,/,$(shell dir src\*.c /s /b 2>nul))
SOURCES := $(filter %.c,$(SOURCES))

SEP = \\
BUILD_DIR := build
ESP_DIR = $(BUILD_DIR)$(SEP)ESP$(SEP)EFI$(SEP)BOOT

MKDIR_BUILD = if not exist "$(BUILD_DIR)" mkdir "$(BUILD_DIR)"
MKDIR_ESP = if not exist "$(ESP_DIR)" mkdir "$(ESP_DIR)"
CP = copy /Y
RM = del /Q
RMDIR = rmdir /S /Q

TARGET_PATH = $(BUILD_DIR)$(SEP)$(TARGET)
TARGET_ESP  = $(ESP_DIR)$(SEP)$(TARGET)

LDFLAGS = -fuse-ld=lld \
          -Wl,/entry:efi_main \
          -Wl,/subsystem:efi_application \
          -Wl,/nodefaultlib \
          -Wl,--no-insert-timestamp

OVMF_CODE ?= OVMF_X64.fd

else

SOURCES := $(shell find src -type f -name '*.c')

SEP = /
ESP_DIR = $(BUILD_DIR)$(SEP)ESP$(SEP)EFI$(SEP)BOOT

MKDIR_BUILD = mkdir -p $(BUILD_DIR)
MKDIR_ESP = mkdir -p $(ESP_DIR)
CP = cp
RM = rm -f
RMDIR = rm -rf

TARGET_PATH = $(BUILD_DIR)$(SEP)$(TARGET)
TARGET_ESP = $(ESP_DIR)$(SEP)$(TARGET)

LDFLAGS = -fuse-ld=lld \
          -Wl,--entry,efi_main \
          -Wl,-subsystem,efi_application \
          -Wl,--no-insert-timestamp

OVMF_CODE ?= $(firstword \
    $(wildcard /usr/share/OVMF/OVMF_CODE_4M.fd) \
    $(wildcard /usr/share/OVMF/OVMF_CODE.fd) \
	$(wildcard /usr/share/OVMF/x64/OVMF_CODE.4m.fd) \
    $(wildcard /usr/share/edk2/ovmf/OVMF_CODE.fd) \
    $(wildcard /usr/share/edk2-ovmf/x64/OVMF_CODE.fd) \
    $(wildcard /usr/share/qemu/ovmf-x86_64-code.fd) \
    $(wildcard /opt/homebrew/share/qemu/edk2-x86_64-code.fd) \
    $(wildcard /usr/local/share/qemu/OVMF_CODE.fd) \
)

endif

all: $(TARGET_PATH)

$(TARGET_PATH): $(SOURCES) $(HEADERS)
	$(MKDIR_BUILD)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

esp: $(TARGET_PATH)
	$(MKDIR_ESP)
	$(CP) $(TARGET_PATH) $(TARGET_ESP)

run: esp
	qemu-system-x86_64 \
	-machine q35 \
	-drive if=pflash,format=raw,readonly=on,file=$(OVMF_CODE) \
	-drive format=raw,file=fat:rw:$(BUILD_DIR)/ESP \
	-net none


clean:
	-$(RM) $(BUILD_DIR)$(SEP)*.EFI
	-$(RMDIR) $(BUILD_DIR)
