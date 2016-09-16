SwitchLedCape

Various routines to exercise the switch/LED BBB cape.

A schematic would be nice:
	Controller:
		P8_12	gpio12	IN (switch, normally 1)
		P8_10	gpio4	OUT (LED)
	PRU:
		P8_15	r31_15	IN (switch, normally 1)
		P8_11	r30_15	OUT (LED)


Ultimate goal is to experiment with communication between Controller and PRU. And interrupts.



