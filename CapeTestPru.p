// PRUSS program for Switch LED Cape
// This is for PRU 0
// Based on Molloy's Listing 13-2, ledButton.p
// 09/15/2016

.origin 0							// offset of start of program in PRU memory
.entrypoint START					// program entry point

// Inputs:
#define SWITCH			r31.t15		// normally high/set

// Outputs:
#define LED				r30.t5

// PRU Data memory:


#define DELAY 5000000				// 5e6 * 5e-9 * 22 = 550 ms

#define PRU0_R31_VEC_VALID	32		// for notification of program completion
#define PRU_EVTOUT_0		 3		// event number returned to Controller

// Registers:
//	r0 = delays
//	r1 = 
//	r2 = 
//	r3 = 
//	r4 = 
//	r5 = 
//	r6 = 
//	r7, r8, r9 = not used
//	r10 = 
//	r11 = 
//	r12 = 
//	r13 = 
//	r14 = 
//	r15 = 
//	r16, r17, r18, r19, r20, r21, r22, r23, r24, r25, r26, r27, r28, r29 = not used
//	r30 = outputs
//	r31 = inputs

//__________________________________________________________
START:
	SET		LED							// LED on
	
	MOV		r0, DELAY
DELAYON:
	SUB		r0, r0, 1
	AND		r0, r0, r0					// NOP
	AND		r0, r0, r0					// NOP
	AND		r0, r0, r0					// NOP
	AND		r0, r0, r0					// NOP
	AND		r0, r0, r0					// NOP
	AND		r0, r0, r0					// NOP
	AND		r0, r0, r0					// NOP
	AND		r0, r0, r0					// NOP
	AND		r0, r0, r0					// NOP
	AND		r0, r0, r0					// NOP
	AND		r0, r0, r0					// NOP
	AND		r0, r0, r0					// NOP
	AND		r0, r0, r0					// NOP
	AND		r0, r0, r0					// NOP
	AND		r0, r0, r0					// NOP
	AND		r0, r0, r0					// NOP
	AND		r0, r0, r0					// NOP
	AND		r0, r0, r0					// NOP
	AND		r0, r0, r0					// NOP
	AND		r0, r0, r0					// NOP
	QBNE	DELAYON, r0, 0
	
LEDOFF:
	CLR		LED							// LED off
	
	MOV		r0, DELAY
DELAYOFF:
	SUB		r0, r0, 1
	AND		r0, r0, r0					// NOP
	AND		r0, r0, r0					// NOP
	AND		r0, r0, r0					// NOP
	AND		r0, r0, r0					// NOP
	AND		r0, r0, r0					// NOP
	AND		r0, r0, r0					// NOP
	AND		r0, r0, r0					// NOP
	AND		r0, r0, r0					// NOP
	AND		r0, r0, r0					// NOP
	AND		r0, r0, r0					// NOP
	AND		r0, r0, r0					// NOP
	AND		r0, r0, r0					// NOP
	AND		r0, r0, r0					// NOP
	AND		r0, r0, r0					// NOP
	AND		r0, r0, r0					// NOP
	AND		r0, r0, r0					// NOP
	AND		r0, r0, r0					// NOP
	AND		r0, r0, r0					// NOP
	AND		r0, r0, r0					// NOP
	AND		r0, r0, r0					// NOP
	QBNE	DELAYOFF, r0, 0
	
	QBBS	START, SWITCH				// if button not pressed, loop

END:
	CLR		LED
	MOV		r31.b0, PRU0_R31_VEC_VALID | PRU_EVTOUT_0
	HALT
