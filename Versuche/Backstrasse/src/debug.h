/*
 * debug.h - some macros to to debugging by printing something to the console.
 * WARNING: writing to the console via the JTAG UART is very slow and will ruin your shedule.
 *
 *  Created on: Jan 5, 2012
 *      Author: kindt
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#define ERROR(...) xil_printf("[ERROR @ "__FILE__);;xil_printf(__FILE__);xil_printf(", ");xil_printf(__FUNCTION__);xil_printf("(),L %d]: ",__LINE__); xil_printf(__VA_ARGS__);xil_printf("\n");
#define WARN(...) xil_printf("[WARN @ "__FILE__);xil_printf(__FILE__);xil_printf(", ");xil_printf(__FUNCTION__);xil_printf("(),L %d]: ",__LINE__); xil_printf(__VA_ARGS__);xil_printf("\n");
#define DEBUG(...) xil_printf("[DEBUG @ ");xil_printf(__FILE__);xil_printf(", ");xil_printf(__FUNCTION__);xil_printf("(),L %d]: ",__LINE__); xil_printf(__VA_ARGS__);xil_printf("\n")
#define DEBUG2(...) xil_printf(__VA_ARGS__);xil_printf("\n")

//overwrite the macro to be empty to avoid hazardous time-wasting console-writings

//#define DEBUG(...)
//#define DEBUG2(...)
#endif /* DEBUG_H_ */
