CROSS_COMPILE=
ASM_FLAGS= -m32 --pipe -Wall -fasm -g -O1 -fno-stack-protector	
C_FLAGS =  -m32 -fno-stack-protector -g -fno-builtin -std=c11
OS_INCLUDE_PATH = myOS/include 
USER_INCLUDE_PATH = userApp/include

.PHONY: all
all: output/myOS.elf

MULTI_BOOT_HEADER=output/multibootheader/multibootHeader.o
include myOS/Makefile
include userApp/Makefile

OS_OBJS       = ${MYOS_OBJS} ${USER_APP_OBJS}

output/myOS.elf: ${OS_OBJS} ${MULTI_BOOT_HEADER}
	@${CROSS_COMPILE}ld -n -T myOS/myOS.ld ${MULTI_BOOT_HEADER} ${OS_OBJS} -o output/myOS.elf
	@echo "Linking myOS.elf"
	@echo "Build finished successfully."

output/%.o : %.S
	@mkdir -p $(dir $@)
	@${CROSS_COMPILE}gcc ${ASM_FLAGS} -c -o $@ $<
	@echo "Assemblying "$<

output/%.o : %.c
	@mkdir -p $(dir $@)
	@${CROSS_COMPILE}gcc ${C_FLAGS} -I${OS_INCLUDE_PATH} -I ${USER_INCLUDE_PATH} -c -o $@ $<
	@echo "Compiling "$<

clean:
	@rm -rf output
	@echo "Build directory removed."
