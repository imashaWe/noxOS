#include "framebuffer.h"
#include "serial.h"
#include "memory_segments.h"
#include "interrupts.h"
#include "keyboard.h"
#include "multiboot.h"
#include "paging.h"
#include "hardware_interrupt_enabler.h"
#include "user_mode.h"

int run_user_mode(unsigned int ebx)
{
	multiboot_info_t* mbinfo = (multiboot_info_t*) ebx;
  	multiboot_module_t* modules = (multiboot_module_t*) mbinfo->mods_addr; 
  	unsigned int address_of_module = modules->mod_start;
  	
  	if((mbinfo->mods_count) == 1){
  		char message[] = "ONE module loaded successfully!";
  		serial_write(message,sizeof(message));
  		
  		typedef void (*call_module_t)(void);
        	/* ... */
        	call_module_t start_program = (call_module_t) address_of_module;
        	start_program();
        	/* we'll never get here, unless the module code returns */

  	}else{
  		char message[] = "Error: More than ONE module loaded";
  		serial_write(message,sizeof(message));
  	}
  	 
  	return 0;
}

int kmain(unsigned int ebx)
{
	char title[] = "Welcome to NOX";
	
    	/* move cursor */
    	fb_move_cursor(6*80);
    	
    	/* frame builder writing */
	fb_write_str(title,sizeof(title));
	
	
	/* install segments */
	segments_install_gdt();
    
	/* install interrupt handler */
	interrupts_install_idt();
	
	/* call user mode */
	run_user_mode(ebx);
    	
    	/* install paging */
    	init_paging();
    	
    	disable_hardware_interrupts();

  	/* Switch to User mode */
   	switch_to_user_mode();
	return 0;
}


