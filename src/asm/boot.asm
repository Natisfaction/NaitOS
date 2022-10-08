;Sir loader by Nait
;Booting NaitOS

[org 0x7c00]                        ;Indirizzo iniziale
KERNEL_LOCATION equ 0x1000          ;Kernel

%define ENDL 0x0D, 0x0A             ;Fine riga

mov [BOOT_DISK], dl                 ;Salvo l'indirizzo del disco iniziale

;FAT12

jmp short start
nop

bdb_oem:                    db 'MSWIN4.1'               ;OEM (8 bit)
bdb_bytes_per_sector:       dw 512                      ;512 byte per settore
bdb_sectors_per_cluster:    db 1                        ;1 settore per cluster
bdb_reserved_sectors:       dw 1                        ;1 settore riservato
bdb_fat_count:              db 2                        ;
bdb_dir_entries_count:      dw 0xE0                     ;
bdb_total_sectors:          dw 2880                     ;1.44MB / 512 = 2880
bdb_media_descriptor_type:  db 0xF0                     ;F0 per i floppy disk 3.5"
bdb_sectors_per_fat:        dw 9                        ;sectors per fat
bdb_sectors_per_track:      dw 18                       ;settori per traccia
bdb_heads:                  dw 2                        ;
bdb_hidden_sectors:         dd 0                        ;
bdb_large_sector_count:     dd 0                        ;

;Extended boot record

ebr_drive_number:           db 0x00                     ;0x00 per fdd, 0x80 per hdd
                            db 0                        ;riservato
ebr_signature:              db 0x29                     ;
ebr_volume_id:              db 0x69, 0x96, 0x69, 0x96   ;numero seriale
ebr_volume_label:           db 'NaitOS     '            ;nome del volume (11 bit)
ebr_system_id:              db 'FAT12   '               ;tipo di fat (8 bit)

;Codice del bootloader

start:
    jmp main

;Azzero i registri, e creo uno stack

main:
    xor ax, ax
    mov es, ax
    mov ds, ax
    mov bp, 0x8000
    mov sp, bp

    mov bx, KERNEL_LOCATION
    mov dh, 80                  ;Aumenta il numero progressivamente quando aggiungi file

    ;Inizializzazione del disco

    mov ah, 0x02
    mov al, dh 
    mov ch, 0x00
    mov dh, 0x00
    mov cl, 0x02
    mov dl, [BOOT_DISK]
    int 0x13

    mov ah, 0x0
    mov al, 0x3
    int 0x10                ;Clear Screen (text mode)

CODE_SEG equ GDT_code - GDT_start
DATA_SEG equ GDT_data - GDT_start

;Inizio della protected mode

cli
lgdt [GDT_descriptor]
mov eax, cr0
or eax, 1
mov cr0, eax
jmp CODE_SEG:start_protected_mode

jmp $

BOOT_DISK: db 0

GDT_start:
    GDT_null:
        dd 0x0
        dd 0x0

    GDT_code:
        dw 0xffff
        dw 0x0
        db 0x0
        db 0b10011010
        db 0b11001111
        db 0x0

    GDT_data:
        dw 0xffff
        dw 0x0
        db 0x0
        db 0b10010010
        db 0b11001111
        db 0x0

GDT_end:

GDT_descriptor:
    dw GDT_end - GDT_start - 1
    dd GDT_start

[bits 32]
start_protected_mode:
    mov ax, DATA_SEG
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	
	mov ebp, 0x90000		;Stack a 32 bit
	mov esp, ebp

    jmp KERNEL_LOCATION     ;Salta alla entry del kernel

msg_read_failed:        db 'Read from disk failed!', ENDL, 0

times 510-($-$$) db 0              
dw 0xaa55