#PICAXE 08M2
#no_data
#terminal 4800

;-55; +125 st.C (+-2 st.C)

symbol LED_GREEN = c.4

hi2csetup i2cmaster, %10010010, i2cslow, i2cbyte

do
	high LED_GREEN
	
	hi2cin 0,(b0,b1)
	
	if b0 > 128 then
		b0 = 255 - b0
		SerTxd("-")
		
		if b1=0 then
			b1=128
		else
			b1 = 0
		endif
	else
		SerTxd("+")
	endif
	
	if b1 = 0 then
		SerTxd( #b0, ".0 st.C ", CR )
	else
		SerTxd( #b0, ".5 st.C ", CR )
	endif
	
	LOW LED_GREEN
	
	pause 500
loop