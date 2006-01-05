################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
$(ROOT)/tuscany/sca/util/DefaultLogWriter.cpp \
$(ROOT)/tuscany/sca/util/File.cpp \
$(ROOT)/tuscany/sca/util/Library.cpp \
$(ROOT)/tuscany/sca/util/LogWriter.cpp \
$(ROOT)/tuscany/sca/util/Logger.cpp \
$(ROOT)/tuscany/sca/util/Utils.cpp 

OBJS += \
./tuscany/sca/util/DefaultLogWriter.o \
./tuscany/sca/util/File.o \
./tuscany/sca/util/Library.o \
./tuscany/sca/util/LogWriter.o \
./tuscany/sca/util/Logger.o \
./tuscany/sca/util/Utils.o 

DEPS += \
${addprefix ./tuscany/sca/util/, \
DefaultLogWriter.d \
File.d \
Library.d \
LogWriter.d \
Logger.d \
Utils.d \
}


# Each subdirectory must supply rules for building sources it contributes
tuscany/sca/util/%.o: $(ROOT)/tuscany/sca/util/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	@echo g++ -I${SCA4CPP_HOME}/runtime/core/src -I${SDO4CPP}/include -I${AXISCPP_DEPLOY}/include -O3 -Wall -c -fmessage-length=0 -o$@ $<
	@g++ -I${SCA4CPP_HOME}/runtime/core/src -I${SDO4CPP}/include -I${AXISCPP_DEPLOY}/include -O3 -Wall -c -fmessage-length=0 -o$@ $< && \
	echo -n $(@:%.o=%.d) $(dir $@) > $(@:%.o=%.d) && \
	g++ -MM -MG -P -w -I${SCA4CPP_HOME}/runtime/core/src -I${SDO4CPP}/include -I${AXISCPP_DEPLOY}/include -O3 -Wall -c -fmessage-length=0  $< >> $(@:%.o=%.d)
	@echo 'Finished building: $<'
	@echo ' '


