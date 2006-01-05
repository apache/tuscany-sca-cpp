################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
$(ROOT)/tuscany/sca/core/ComponentServiceWrapper.cpp \
$(ROOT)/tuscany/sca/core/ExternalServiceWrapper.cpp \
$(ROOT)/tuscany/sca/core/Operation.cpp \
$(ROOT)/tuscany/sca/core/SCAEntryPoint.cpp \
$(ROOT)/tuscany/sca/core/SCARuntime.cpp \
$(ROOT)/tuscany/sca/core/ServiceProxy.cpp \
$(ROOT)/tuscany/sca/core/ServiceWrapper.cpp 

OBJS += \
./tuscany/sca/core/ComponentServiceWrapper.o \
./tuscany/sca/core/ExternalServiceWrapper.o \
./tuscany/sca/core/Operation.o \
./tuscany/sca/core/SCAEntryPoint.o \
./tuscany/sca/core/SCARuntime.o \
./tuscany/sca/core/ServiceProxy.o \
./tuscany/sca/core/ServiceWrapper.o 

DEPS += \
${addprefix ./tuscany/sca/core/, \
ComponentServiceWrapper.d \
ExternalServiceWrapper.d \
Operation.d \
SCAEntryPoint.d \
SCARuntime.d \
ServiceProxy.d \
ServiceWrapper.d \
}


# Each subdirectory must supply rules for building sources it contributes
tuscany/sca/core/%.o: $(ROOT)/tuscany/sca/core/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	@echo g++ -D_DEBUG -I${SCA4CPP_HOME}/runtime/core/src -I${SDO4CPP}/include -I${AXISCPP_DEPLOY}/include -O0 -g3 -Wall -c -fmessage-length=0 -o$@ $<
	@g++ -D_DEBUG -I${SCA4CPP_HOME}/runtime/core/src -I${SDO4CPP}/include -I${AXISCPP_DEPLOY}/include -O0 -g3 -Wall -c -fmessage-length=0 -o$@ $< && \
	echo -n $(@:%.o=%.d) $(dir $@) > $(@:%.o=%.d) && \
	g++ -MM -MG -P -w -D_DEBUG -I${SCA4CPP_HOME}/runtime/core/src -I${SDO4CPP}/include -I${AXISCPP_DEPLOY}/include -O0 -g3 -Wall -c -fmessage-length=0  $< >> $(@:%.o=%.d)
	@echo 'Finished building: $<'
	@echo ' '


