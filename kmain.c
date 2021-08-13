#include "drivers/frame_builder.h"
#include "drivers/serial_write.h"
#include "memory_segmnets.h"

int kmain()
{
	char heading[] = "Welcome to NOX";
	unsigned int len = sizeof(heading) / sizeof(heading[0]);
	
	// install segments
	segments_install_gdt();
	
	// frame builder writing
    	fb_write(0, heading, len);
    
    	// serial writing
    	serial_write(0x3F8, heading, len);
    	
	return 0;
}

