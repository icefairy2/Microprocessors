//this assembly stub needs to be completed.

.extern bus					//make bus-ponter from C-Code known to assembly

.text						//indicate the following statements as executable segment (program code, text)
.global switch_lights		//declare the symbol "swicht_lights" as global to make it accessible from outside

.global determine_lights

switch_lights:
	//insert your code here
	lbui r7, r5, 0
	lwi r8, r0, bus
	shi r7, r8, 0



	//the following two lines are needed to jump back
	rtsd r15,8				//jump to linker address contained in r15 plus 8 bytes to skip the brlid and the following nop instruction
	nop						//burn one clock cycle as tehre is a delay slot after the rtsd-instruction. Note that the nop-instruction is a "virtual" instruction that is not contained in the microblaze

determine_lights:


	//****parameter pass*****
	//take value currently on bus
	lwi r11, r0, bus
	lhui r9, r11, 0
	//branch on corresponding value
	xori r10, r9, 1
	beqi r10, isPattern001
	nop
	xori r10, r9, 2
	beqi r10, isPattern010
	nop
	xori r10, r9, 4
	beqi r10, isPattern100
	nop

	isPattern001:
		addi r5, r0, pattern010
		brid end_if
		nop
	isPattern010:
		addi r5, r0, pattern100
		brid end_if
		nop
	isPattern100:
		addi r5, r0, pattern001
	//************************

	end_if:
	//stack pointer
	addik r1, r1, -4
	//store jump adress on stack
	swi r15, r1, 0
	//brach and link
	brlid r15, switch_lights
	//delay slot
	nop
	//store jump address in (subroutine jump address register) r15
	lwi r15, r1, 0
	//increment stack pointer
	addik r1, r1, 4

	rtsd r15,8
	nop

//data section
.data
//make symbols global
.global pattern001
.global pattern010
.global pattern100
.align 4	//align to word boundary
pattern001:
	.byte 0b00000001
.align 4	//align to word boundary
pattern010:
	.byte 0b00000010
.align 4	//align to word boundary
pattern100:
	.byte 0b00000100
