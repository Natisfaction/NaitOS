#include <stdbool.h>
#include "../Drivers/keyboard.h"
#include "../header/stdio.h"

//SCANCODE SET 1

unsigned char kbdus[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
    '9', '0', '\'', '`', '\21',	/* Backspace */
    '\t',			/* Tab */
    'q', 'w', 'e', 'r',	/* 19 */
    't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\20',	/* Enter key */
    0,			/* 29   - Control */
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', '@',	/* 39 */
    '#', '\\',   0,		/* Left shift */
    '§', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
    'm', ',', '.', '-',   0,				/* Right shift */
    '*',
    0,	/* Alt */
    ' ',/* Space bar */
    0,	/* Caps lock */
    '\1',	/* 59 - F1 key ... > */
    '\2', '\3', '\4', '\5', '\6', '\7', '\18', '\19',
    '\10',	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
    '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
    '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    '\11',	/* F11 Key */
    '\12',	/* F12 Key */
    0,	/* All other keys are undefined */
};

bool shift = false, ctrl = false;
bool caps_lock = false;

void keyboard_handler(struct regs *r){
    unsigned char scancode;

    scancode = inb(0x60);
    
    if (scancode & 0x80){

        //Controllo se alcuni tasti speciali sono stati rilasciati

        switch(scancode){

    		case 0xaa:
                shift = false;
                break;

            case 0x9D:
                ctrl = false;
                break;

            case 0xB6:
                shift = false;
                break;
        }

	} else {
    	
    	switch(scancode){

            //4 frecce
            
    	  	case 0x4b:
                Sx();
                break;

    	  	case 0x4d: 
                Dx();
                break;

    	  	case 0x48: 
                Up();
                break;

    	  	case 0x50: 
                Down();
                break;
            
            //Esc

            case 0x01:
                break;           
            
            //Il ctrl

            case 0x1d:
                ctrl = true;
                break;

            //I due shift

    	  	case 0x2a: 
                shift = true;
                break;

    	  	case 0x36: 
                shift = true;
                break;

            //Qui il caps va acceso e spento

    	  	case 0x3a: 
                caps_lock = !caps_lock;
                break;
            
            //Altrimenti esegui la conversione scancode / carattere

    	  	default:
                computate(kbdus[scancode]);
                break;
    	}
    }
}

int computate(int digit){
    if ((shift || caps_lock) && (digit >= 97 && digit <= 122)){
        putc(digit-32);
    } else if (shift){
        switch (digit){
            case '0':
                putc('=');
                break;

            case '1':
                putc('!');
                break;

            case '2':
                putc('"');
                break;

            case '3':
                putc('£');
                break;

            case '4':
                putc('$');
                break;

            case '5':
                putc('%');
                break;

            case '6':
                putc('&');
                break;

            case '7':
                putc('/');
                break;

            case '8':
                putc('(');
                break;

            case '9':
                putc(')');
                break;

            case '\'':
                putc('?');
                break;

            case '`':
                putc('^');
                break;

            case ',':
                putc(';');
                break;

            case '.':
                putc(':');
                break;

            case '-':
                putc('_');
                break;

            case '\\':
                putc('|');
                break;

            default:
                break;
        }
    } else if (ctrl) {

        //Uncomment if you need to use Ctrl, but do not use puts

        /* switch (digit){
            case 'Something...':
                putc('Something...');
                break;

            default:
                break;
        } */
    } else {
        putc(digit);
    }
    
}

void keyboard_install(){
    irq_install_handler(1, keyboard_handler);
}