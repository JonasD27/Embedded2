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
SOURCEFILES_QUOTED_IF_SPACED=code/configuration_bits.c code/interrupts.c code/main.c code/system.c code/traps.c code/user.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/code/configuration_bits.o ${OBJECTDIR}/code/interrupts.o ${OBJECTDIR}/code/main.o ${OBJECTDIR}/code/system.o ${OBJECTDIR}/code/traps.o ${OBJECTDIR}/code/user.o
POSSIBLE_DEPFILES=${OBJECTDIR}/code/configuration_bits.o.d ${OBJECTDIR}/code/interrupts.o.d ${OBJECTDIR}/code/main.o.d ${OBJECTDIR}/code/system.o.d ${OBJECTDIR}/code/traps.o.d ${OBJECTDIR}/code/user.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/code/configuration_bits.o ${OBJECTDIR}/code/interrupts.o ${OBJECTDIR}/code/main.o ${OBJECTDIR}/code/system.o ${OBJECTDIR}/code/traps.o ${OBJECTDIR}/code/user.o

# Source Files
SOURCEFILES=code/configuration_bits.c code/interrupts.c code/main.c code/system.c code/traps.c code/user.c



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
${OBJECTDIR}/code/configuration_bits.o: code/configuration_bits.c  .generated_files/flags/XC16_dsPIC33EP512MU810/23e2e8965833648c633e321de41ab73df5879b9d .generated_files/flags/XC16_dsPIC33EP512MU810/168b633b52a46c5b8b8a05feadf3c077d6e15a18
	@${MKDIR} "${OBJECTDIR}/code" 
	@${RM} ${OBJECTDIR}/code/configuration_bits.o.d 
	@${RM} ${OBJECTDIR}/code/configuration_bits.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  code/configuration_bits.c  -o ${OBJECTDIR}/code/configuration_bits.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/code/configuration_bits.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_XC16_dsPIC33EP512MU810=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/code/interrupts.o: code/interrupts.c  .generated_files/flags/XC16_dsPIC33EP512MU810/85853100e7dab9b5ae50aa06f4b5e8f9bf927511 .generated_files/flags/XC16_dsPIC33EP512MU810/168b633b52a46c5b8b8a05feadf3c077d6e15a18
	@${MKDIR} "${OBJECTDIR}/code" 
	@${RM} ${OBJECTDIR}/code/interrupts.o.d 
	@${RM} ${OBJECTDIR}/code/interrupts.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  code/interrupts.c  -o ${OBJECTDIR}/code/interrupts.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/code/interrupts.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_XC16_dsPIC33EP512MU810=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/code/main.o: code/main.c  .generated_files/flags/XC16_dsPIC33EP512MU810/7fd87c6978413c0c334b93586591f0bab7c63714 .generated_files/flags/XC16_dsPIC33EP512MU810/168b633b52a46c5b8b8a05feadf3c077d6e15a18
	@${MKDIR} "${OBJECTDIR}/code" 
	@${RM} ${OBJECTDIR}/code/main.o.d 
	@${RM} ${OBJECTDIR}/code/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  code/main.c  -o ${OBJECTDIR}/code/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/code/main.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_XC16_dsPIC33EP512MU810=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/code/system.o: code/system.c  .generated_files/flags/XC16_dsPIC33EP512MU810/4427ed15a7b2312583446c8c5093bb37b995a93b .generated_files/flags/XC16_dsPIC33EP512MU810/168b633b52a46c5b8b8a05feadf3c077d6e15a18
	@${MKDIR} "${OBJECTDIR}/code" 
	@${RM} ${OBJECTDIR}/code/system.o.d 
	@${RM} ${OBJECTDIR}/code/system.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  code/system.c  -o ${OBJECTDIR}/code/system.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/code/system.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_XC16_dsPIC33EP512MU810=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/code/traps.o: code/traps.c  .generated_files/flags/XC16_dsPIC33EP512MU810/32b93763760c4b7ba0c6c0572002c7f51c4ae9f1 .generated_files/flags/XC16_dsPIC33EP512MU810/168b633b52a46c5b8b8a05feadf3c077d6e15a18
	@${MKDIR} "${OBJECTDIR}/code" 
	@${RM} ${OBJECTDIR}/code/traps.o.d 
	@${RM} ${OBJECTDIR}/code/traps.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  code/traps.c  -o ${OBJECTDIR}/code/traps.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/code/traps.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_XC16_dsPIC33EP512MU810=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/code/user.o: code/user.c  .generated_files/flags/XC16_dsPIC33EP512MU810/8dc95cf42d01c45a4200e6be45b4045f05285f4c .generated_files/flags/XC16_dsPIC33EP512MU810/168b633b52a46c5b8b8a05feadf3c077d6e15a18
	@${MKDIR} "${OBJECTDIR}/code" 
	@${RM} ${OBJECTDIR}/code/user.o.d 
	@${RM} ${OBJECTDIR}/code/user.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  code/user.c  -o ${OBJECTDIR}/code/user.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/code/user.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_XC16_dsPIC33EP512MU810=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
else
${OBJECTDIR}/code/configuration_bits.o: code/configuration_bits.c  .generated_files/flags/XC16_dsPIC33EP512MU810/3fe02131a9fa3121ff1f735fba929361eceddd93 .generated_files/flags/XC16_dsPIC33EP512MU810/168b633b52a46c5b8b8a05feadf3c077d6e15a18
	@${MKDIR} "${OBJECTDIR}/code" 
	@${RM} ${OBJECTDIR}/code/configuration_bits.o.d 
	@${RM} ${OBJECTDIR}/code/configuration_bits.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  code/configuration_bits.c  -o ${OBJECTDIR}/code/configuration_bits.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/code/configuration_bits.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_XC16_dsPIC33EP512MU810=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/code/interrupts.o: code/interrupts.c  .generated_files/flags/XC16_dsPIC33EP512MU810/da21b7248522d179a66cd004dd021d0c582c952c .generated_files/flags/XC16_dsPIC33EP512MU810/168b633b52a46c5b8b8a05feadf3c077d6e15a18
	@${MKDIR} "${OBJECTDIR}/code" 
	@${RM} ${OBJECTDIR}/code/interrupts.o.d 
	@${RM} ${OBJECTDIR}/code/interrupts.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  code/interrupts.c  -o ${OBJECTDIR}/code/interrupts.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/code/interrupts.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_XC16_dsPIC33EP512MU810=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/code/main.o: code/main.c  .generated_files/flags/XC16_dsPIC33EP512MU810/4c74f4bb0752e9ff87f42f4ad093aa0cbc17a3d3 .generated_files/flags/XC16_dsPIC33EP512MU810/168b633b52a46c5b8b8a05feadf3c077d6e15a18
	@${MKDIR} "${OBJECTDIR}/code" 
	@${RM} ${OBJECTDIR}/code/main.o.d 
	@${RM} ${OBJECTDIR}/code/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  code/main.c  -o ${OBJECTDIR}/code/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/code/main.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_XC16_dsPIC33EP512MU810=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/code/system.o: code/system.c  .generated_files/flags/XC16_dsPIC33EP512MU810/94dd22012a863a99730b98a7895299653279a8c2 .generated_files/flags/XC16_dsPIC33EP512MU810/168b633b52a46c5b8b8a05feadf3c077d6e15a18
	@${MKDIR} "${OBJECTDIR}/code" 
	@${RM} ${OBJECTDIR}/code/system.o.d 
	@${RM} ${OBJECTDIR}/code/system.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  code/system.c  -o ${OBJECTDIR}/code/system.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/code/system.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_XC16_dsPIC33EP512MU810=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/code/traps.o: code/traps.c  .generated_files/flags/XC16_dsPIC33EP512MU810/1b9e73e12b2300ccefc015c71e9b196abbdd5cd8 .generated_files/flags/XC16_dsPIC33EP512MU810/168b633b52a46c5b8b8a05feadf3c077d6e15a18
	@${MKDIR} "${OBJECTDIR}/code" 
	@${RM} ${OBJECTDIR}/code/traps.o.d 
	@${RM} ${OBJECTDIR}/code/traps.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  code/traps.c  -o ${OBJECTDIR}/code/traps.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/code/traps.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_XC16_dsPIC33EP512MU810=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/code/user.o: code/user.c  .generated_files/flags/XC16_dsPIC33EP512MU810/a8e9626b866da9a5865a64fc9b026fe8835b8ebe .generated_files/flags/XC16_dsPIC33EP512MU810/168b633b52a46c5b8b8a05feadf3c077d6e15a18
	@${MKDIR} "${OBJECTDIR}/code" 
	@${RM} ${OBJECTDIR}/code/user.o.d 
	@${RM} ${OBJECTDIR}/code/user.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  code/user.c  -o ${OBJECTDIR}/code/user.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/code/user.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_XC16_dsPIC33EP512MU810=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
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
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/Projekt_Template.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -omf=elf -DXPRJ_XC16_dsPIC33EP512MU810=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)   -mreserve=data@0x1000:0x101B -mreserve=data@0x101C:0x101D -mreserve=data@0x101E:0x101F -mreserve=data@0x1020:0x1021 -mreserve=data@0x1022:0x1023 -mreserve=data@0x1024:0x1027 -mreserve=data@0x1028:0x104F   -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,--defsym=__MPLAB_DEBUGGER_PK3=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	
else
${DISTDIR}/Projekt_Template.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/Projekt_Template.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DXPRJ_XC16_dsPIC33EP512MU810=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
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
