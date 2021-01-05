adc 25 ;There is no error in this code
sp2a	;object file and listing file

call loop	;calling loop
add 
sub
loop:
ldnl 0x10	;code doesnt fail on hexadecimal
adj var1
var1: data 10 