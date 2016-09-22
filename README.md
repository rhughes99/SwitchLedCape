SwitchLedCape

Various routines to exercise the switch/LED BBB cape.

A schematic would be nice:
	Controller:
		P8_12	0x830/030	gpio1[12]	IN	(switch, normally 1)
		P8_10	0x898/098	gpio2[04]	OUT	(LED)
	PRU:
		P8_15	0x83C/03C	pr1_pru0_pru_r31_15	IN	(switch, normally 1)
		P8_11	0x834/034	pr1_pru0_pru_r30_15	OUT	(LED)


Ultimate goal is to experiment with communication between Controller and PRU. And interrupts.



