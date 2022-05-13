#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-XC16_dsPIC33EP512MU810.mk)" "nbproject/Makefile-local-XC16_dsPIC33EP512MU810.mk"
include nbproject/Makefile-local-XC16_dsPIC33EP512MU810.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=XC16_dsPIC33EP512MU810
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/Projekt_Template.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/Projekt_Template.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS
SUB_IMAGE_ADDRESS_COMMAND=--image-address $(SUB_IMAGE_ADDRESS)
else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=code/configuration_bits.c code/interrupts.c code/main.c code/system.c code/traps.c code/user.c code/I2C.c code/UART.c code/PMP.c code/eeprom_25xx-1.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/code/configuration_bits.o ${OBJECTDIR}/code/interrupts.o ${OBJECTDIR}/code/main.o ${OBJECTDIR}/code/system.o ${OBJECTDIR}/code/traps.o ${OBJECTDIR}/code/user.o ${OBJECTDIR}/code/I2C.o ${OBJECTDIR}/code/UART.o ${OBJECTDIR}/code/PMP.o ${OBJECTDIR}/code/eeprom_25xx-1.o
POSSIBLE_DEPFILES=${OBJECTDIR}/code/configuration_bits.o.d ${OBJECTDIR}/code/interrupts.o.d ${OBJECTDIR}/code/main.o.d ${OBJECTDIR}/code/system.o.d ${OBJECTDIR}/code/traps.o.d ${OBJECTDIR}/code/user.o.d ${OBJECTDIR}/code/I2C.o.d ${OBJECTDIR}/code/UART.o.d ${OBJECTDIR}/code/PMP.o.d ${OBJECTDIR}/code/eeprom_25xx-1.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/code/configuration_bits.o ${OBJECTDIR}/code/interrupts.o ${OBJECTDIR}/code/main.o ${OBJECTDIR}/code/system.o ${OBJECTDIR}/code/traps.o ${OBJECTDIR}/code/user.o ${OBJECTDIR}/code/I2C.o ${OBJECTDIR}/code/UART.o ${OBJECTDIR}/code/PMP.o ${OBJECTDIR}/code/eeprom_25xx-1.o

# Source Files
SOURCEFILES=code/configuration_bits.c code/interrupts.c code/main.c code/system.c code/traps.c code/user.c code/I2C.c code/UART.c code/PMP.c code/eeprom_25xx-1.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-XC16_dsPIC33EP512MU810.mk ${DISTDIR}/Projekt_Template.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=33EP512MU810
MP_LINKER_FILE_OPTION=,--script=p33EP512MU810.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/code/configuration_bits.o: code/configuration_bits.c  .generated_files/flags/XC16_dsPIC33EP512MU810/d6506a1e3ee723c0bb9889aad485879f49cdac0 .generated_files/flags/XC16_dsPIC33EP512MU810/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}/code" 
	@${RM} ${OBJECTDIR}/code/configuration_bits.o.d 
	@${RM} ${OBJECTDIR}/code/configuration_bits.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  code/configuration_bits.c  -o ${OBJECTDIR}/code/configuration_bits.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/code/configuration_bits.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_XC16_dsPIC33EP512MU810=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/code/interrupts.o: code/interrupts.c  .generated_files/flags/XC16_dsPIC33EP512MU810/f1963837f8be9e19bc2c46031079893584047357 .generated_files/flags/XC16_dsPIC33EP512MU810/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}/code" 
	@${RM} ${OBJECTDIR}/code/interrupts.o.d 
	@${RM} ${OBJECTDIR}/code/interrupts.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  code/interrupts.c  -o ${OBJECTDIR}/code/interrupts.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/code/interrupts.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_XC16_dsPIC33EP512MU810=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/code/main.o: code/main.c  .generated_files/flags/XC16_dsPIC33EP512MU810/491014c37e5e02457ddabfef75d044255a1a4cca .generated_files/flags/XC16_dsPIC33EP512MU810/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}/code" 
	@${RM} ${OBJECTDIR}/code/main.o.d 
	@${RM} ${OBJECTDIR}/code/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  code/main.c  -o ${OBJECTDIR}/code/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/code/main.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_XC16_dsPIC33EP512MU810=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/code/system.o: code/system.c  .generated_files/flags/XC16_dsPIC33EP512MU810/cf8ccfd5c3c40dc43e8f28941f271bd5e1585979 .generated_files/flags/XC16_dsPIC33EP512MU810/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}/code" 
	@${RM} ${OBJECTDIR}/code/system.o.d 
	@${RM} ${OBJECTDIR}/code/system.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  code/system.c  -o ${OBJECTDIR}/code/system.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/code/system.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_XC16_dsPIC33EP512MU810=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/code/traps.o: code/traps.c  .generated_files/flags/XC16_dsPIC33EP512MU810/2814ea2969f51df37882b5359334de2914112578 .generated_files/flags/XC16_dsPIC33EP512MU810/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}/code" 
	@${RM} ${OBJECTDIR}/code/traps.o.d 
	@${RM} ${OBJECTDIR}/code/traps.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  code/traps.c  -o ${OBJECTDIR}/code/traps.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/code/traps.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_XC16_dsPIC33EP512MU810=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/code/user.o: code/user.c  .generated_files/flags/XC16_dsPIC33EP512MU810/a405169e9be444dc7ab1cb2f9d7b38992203b2e3 .generated_files/flags/XC16_dsPIC33EP512MU810/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}/code" 
	@${RM} ${OBJECTDIR}/code/user.o.d 
	@${RM} ${OBJECTDIR}/code/user.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  code/user.c  -o ${OBJECTDIR}/code/user.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/code/user.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_XC16_dsPIC33EP512MU810=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/code/I2C.o: code/I2C.c  .generated_files/flags/XC16_dsPIC33EP512MU810/cb81f74c8537ffd7c4ab9f3c2a2965cbac951551 .generated_files/flags/XC16_dsPIC33EP512MU810/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}/code" 
	@${RM} ${OBJECTDIR}/code/I2C.o.d 
	@${RM} ${OBJECTDIR}/code/I2C.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  code/I2C.c  -o ${OBJECTDIR}/code/I2C.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/code/I2C.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_XC16_dsPIC33EP512MU810=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/code/UART.o: code/UART.c  .generated_files/flags/XC16_dsPIC33EP512MU810/d55483b17c3673b5e47a394dfc154c938db8e2db .generated_files/flags/XC16_dsPIC33EP512MU810/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}/code" 
	@${RM} ${OBJECTDIR}/code/UART.o.d 
	@${RM} ${OBJECTDIR}/code/UART.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  code/UART.c  -o ${OBJECTDIR}/code/UART.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/code/UART.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_XC16_dsPIC33EP512MU810=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/code/PMP.o: code/PMP.c  .generated_files/flags/XC16_dsPIC33EP512MU810/f5d0d21e6ba0598a2e54a14d917eef736d5a7e6 .generated_files/flags/XC16_dsPIC33EP512MU810/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}/code" 
	@${RM} ${OBJECTDIR}/code/PMP.o.d 
	@${RM} ${OBJECTDIR}/code/PMP.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  code/PMP.c  -o ${OBJECTDIR}/code/PMP.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/code/PMP.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_XC16_dsPIC33EP512MU810=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/code/eeprom_25xx-1.o: code/eeprom_25xx-1.c  .generated_files/flags/XC16_dsPIC33EP512MU810/9607f672026ee0a77276d51ba60abf987cd7e9c9 .generated_files/flags/XC16_dsPIC33EP512MU810/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}/code" 
	@${RM} ${OBJECTDIR}/code/eeprom_25xx-1.o.d 
	@${RM} ${OBJECTDIR}/code/eeprom_25xx-1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  code/eeprom_25xx-1.c  -o ${OBJECTDIR}/code/eeprom_25xx-1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/code/eeprom_25xx-1.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_XC16_dsPIC33EP512MU810=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
else
${OBJECTDIR}/code/configuration_bits.o: code/configuration_bits.c  .generated_files/flags/XC16_dsPIC33EP512MU810/f6900a1b8d9880cfbf696eb65502478336a82fc3 .generated_files/flags/XC16_dsPIC33EP512MU810/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}/code" 
	@${RM} ${OBJECTDIR}/code/configuration_bits.o.d 
	@${RM} ${OBJECTDIR}/code/configuration_bits.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  code/configuration_bits.c  -o ${OBJECTDIR}/code/configuration_bits.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/code/configuration_bits.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_XC16_dsPIC33EP512MU810=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/code/interrupts.o: code/interrupts.c  .generated_files/flags/XC16_dsPIC33EP512MU810/bd875b5b3b54484c69c6ff1b34108192817ea2e5 .generated_files/flags/XC16_dsPIC33EP512MU810/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}/code" 
	@${RM} ${OBJECTDIR}/code/interrupts.o.d 
	@${RM} ${OBJECTDIR}/code/interrupts.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  code/interrupts.c  -o ${OBJECTDIR}/code/interrupts.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/code/interrupts.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_XC16_dsPIC33EP512MU810=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/code/main.o: code/main.c  .generated_files/flags/XC16_dsPIC33EP512MU810/68dc5f90f87bcb82f707829c6a59bb8f60c4d995 .generated_files/flags/XC16_dsPIC33EP512MU810/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}/code" 
	@${RM} ${OBJECTDIR}/code/main.o.d 
	@${RM} ${OBJECTDIR}/code/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  code/main.c  -o ${OBJECTDIR}/code/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/code/main.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_XC16_dsPIC33EP512MU810=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/code/system.o: code/system.c  .generated_files/flags/XC16_dsPIC33EP512MU810/c9a3aef87c94fe8fc2121ac02ccd4f429bc3784f .generated_files/flags/XC16_dsPIC33EP512MU810/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}/code" 
	@${RM} ${OBJECTDIR}/code/system.o.d 
	@${RM} ${OBJECTDIR}/code/system.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  code/system.c  -o ${OBJECTDIR}/code/system.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/code/system.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_XC16_dsPIC33EP512MU810=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/code/traps.o: code/traps.c  .generated_files/flags/XC16_dsPIC33EP512MU810/54d6a0d613319437373e893470d70d5e623a416a .generated_files/flags/XC16_dsPIC33EP512MU810/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}/code" 
	@${RM} ${OBJECTDIR}/code/traps.o.d 
	@${RM} ${OBJECTDIR}/code/traps.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  code/traps.c  -o ${OBJECTDIR}/code/traps.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/code/traps.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_XC16_dsPIC33EP512MU810=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/code/user.o: code/user.c  .generated_files/flags/XC16_dsPIC33EP512MU810/57286c41835274fdbfcd10747cb5e9460a9c22ee .generated_files/flags/XC16_dsPIC33EP512MU810/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}/code" 
	@${RM} ${OBJECTDIR}/code/user.o.d 
	@${RM} ${OBJECTDIR}/code/user.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  code/user.c  -o ${OBJECTDIR}/code/user.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/code/user.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_XC16_dsPIC33EP512MU810=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/code/I2C.o: code/I2C.c  .generated_files/flags/XC16_dsPIC33EP512MU810/f3f3c13c8d25102a4a22e047fc2791da9f14163c .generated_files/flags/XC16_dsPIC33EP512MU810/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}/code" 
	@${RM} ${OBJECTDIR}/code/I2C.o.d 
	@${RM} ${OBJECTDIR}/code/I2C.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  code/I2C.c  -o ${OBJECTDIR}/code/I2C.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/code/I2C.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_XC16_dsPIC33EP512MU810=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/code/UART.o: code/UART.c  .generated_files/flags/XC16_dsPIC33EP512MU810/ff2f602f70ef53901a52388ec4337434a5f90066 .generated_files/flags/XC16_dsPIC33EP512MU810/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}/code" 
	@${RM} ${OBJECTDIR}/code/UART.o.d 
	@${RM} ${OBJECTDIR}/code/UART.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  code/UART.c  -o ${OBJECTDIR}/code/UART.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/code/UART.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_XC16_dsPIC33EP512MU810=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/code/PMP.o: code/PMP.c  .generated_files/flags/XC16_dsPIC33EP512MU810/1e95a449ff299498d5c6ae0e11753e9879cac184 .generated_files/flags/XC16_dsPIC33EP512MU810/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}/code" 
	@${RM} ${OBJECTDIR}/code/PMP.o.d 
	@${RM} ${OBJECTDIR}/code/PMP.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  code/PMP.c  -o ${OBJECTDIR}/code/PMP.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/code/PMP.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_XC16_dsPIC33EP512MU810=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/code/eeprom_25xx-1.o: code/eeprom_25xx-1.c  .generated_files/flags/XC16_dsPIC33EP512MU810/7573abe6e6b850c3de5d04ec8541f3a5e80891e5 .generated_files/flags/XC16_dsPIC33EP512MU810/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}/code" 
	@${RM} ${OBJECTDIR}/code/eeprom_25xx-1.o.d 
	@${RM} ${OBJECTDIR}/code/eeprom_25xx-1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  code/eeprom_25xx-1.c  -o ${OBJECTDIR}/code/eeprom_25xx-1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/code/eeprom_25xx-1.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_XC16_dsPIC33EP512MU810=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/Projekt_Template.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/Projekt_Template.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -omf=elf -DXPRJ_XC16_dsPIC33EP512MU810=$(CND_CONF)    $(COMPARISON_BUILD)   -mreserve=data@0x1000:0x101B -mreserve=data@0x101C:0x101D -mreserve=data@0x101E:0x101F -mreserve=data@0x1020:0x1021 -mreserve=data@0x1022:0x1023 -mreserve=data@0x1024:0x1027 -mreserve=data@0x1028:0x104F   -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,--defsym=__MPLAB_DEBUGGER_PK3=1,$(MP_LINKER_FILE_OPTION),--heap=1024,--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	
else
${DISTDIR}/Projekt_Template.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/Projekt_Template.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DXPRJ_XC16_dsPIC33EP512MU810=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--heap=1024,--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	${MP_CC_DIR}\\xc16-bin2hex ${DISTDIR}/Projekt_Template.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf   -mdfp="${DFP_DIR}/xc16" 
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
