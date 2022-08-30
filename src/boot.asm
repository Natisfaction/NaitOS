;Sir Loader by Nait

[ORG 7C00H]                            ;Indirizzo iniziale
[BITS 16]                               ;Real mode = 16 bit
KERNEL_LOCATION equ 0x1000              ;Indirizzo del kernel

mov [BOOT_DISK], dl                     ;Salviamo l'indirizzo del disco         

;Azzero alcuni registri e creo uno stack

xor ax, ax
mov es, ax
mov ds, ax
mov bp, 0x8000
mov sp, bp

;Inizializzo il disco

mov bx, KERNEL_LOCATION
mov dh, 2

mov ah, 2
mov al, dh
mov ch, 0
mov dh, 0
mov cl, 2
mov dl, [BOOT_DISK]
int 13h

;Clearscreen (passando in text mode)

mov ah, 0
mov al, 3
int 10h

;E ora si passa in protected mode (32 bit)

CODE_SEG equ GDT_code - GDT_start
DATA_SEG equ GDT_data - GDT_start

cli                                         ;Spengo gli interrupt
lgdt [GDT_descriptor]                       ;Global Decryptor Table
mov eax, cr0                                ;Devo fare un or del registo CR0
or eax, 1
mov cr0, eax
jmp CODE_SEG:start_protected_mode           ;E da qui, siamo in protected mode
                                    
BOOT_DISK: db 0                             ;Disco di boot (disco 0)

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

;E qui siamo ufficialmente a 32 bit

[bits 32]
start_protected_mode:
    mov ax, DATA_SEG
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	
    ;Quindi uno stack a 32 bit

	mov ebp, 0x90000
	mov esp, ebp

    jmp KERNEL_LOCATION                             ;E qui si va al kernel

times 510-($-$$) db 0              
dw 0xaa55
