/* Template for exercise 4: Interrupt Handling
 *
 * Jan. 2012, Philipp Kindt <kindt@rcs.ei.tum.de>
 */


#include <stdio.h>
#include "platform.h"
#include <libmps.h>
#include <inttypes.h>

void buttonPressed() {
	xil_printf("Button was pressed\n");
}

int main()
{
    init_platform();
    InterruptController_Init();
    ENABLE_INTERRUPTS;

    while(1)
    {
    	InterruptController_RegisterHandler(0, buttonPressed);
    	wait_ms(300);
    }

    cleanup_platform();

    return 0;
}
