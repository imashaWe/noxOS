#include "framebuffer.h"
#include "serial.h"
#include "memory_segments.h"
#include "interrupts.h"
#include "keyboard.h"

int kmain()
{
	char title[] = "Welcome to NOX";
	
    	// move cursor
    	fb_move_cursor(6*80);
    	
    	// frame builder writing
	fb_write_str(title,sizeof(title));
	
	// serial writing
	serial_write(title,sizeof(title));
	
	// install segments
	segments_install_gdt();
	
	// install interrupt handler
	interrupts_install_idt();
    	
	return 0;
}

