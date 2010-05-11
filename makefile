
CC=avr-gcc
CFLAGS=-g -Os -Wall -mcall-prologues -mmcu=attiny13
OBJ2HEX=avr-objcopy 
UISP=avrdude 
TARGET=Timer

program : $(TARGET).hex
	$(UISP) -c ponyser -p t13 -P /dev/ttyS0 -e
	$(UISP) -c ponyser -p t13 -P /dev/ttyS0 -U flash:w:$(TARGET).hex
%.obj : %.o
	$(CC) $(CFLAGS) $< -o $@

%.hex : %.obj
	$(OBJ2HEX) -R .eeprom -O ihex $< $@

clean :
	rm -f *.hex *.obj *.o
