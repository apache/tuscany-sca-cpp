################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
$(ROOT)/tuscany/sca/ws/SDOStub.cpp \
$(ROOT)/tuscany/sca/ws/WSServiceWrapper.cpp 

OBJS += \
./tuscany/sca/ws/SDOStub.o \
./tuscany/sca/ws/WSServiceWrapper.o 

DEPS += \
${addprefix ./tuscany/sca/ws/, \
SDOStub.d \
WSServiceWrapper.d \
}


# Each subdirectory must supply rules for building sources it contributes
tuscany/sca/ws/%.o: $(ROOT)/tuscany/sca/ws/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	@echo g++ -I${SCA4CPP_HOME}/runtime/core/src -I${SDO4CPP}/include -I${AXISCPP_DEPLOY}/include -O3 -Wall -c -fmessage-length=0 -o$@ $<
	@g++ -I${SCA4CPP_HOME}/runtime/core/src -I${SDO4CPP}/include -I${AXISCPP_DEPLOY}/include -O3 -Wall -c -fmessage-length=0 -o$@ $< && \
	echo -n $(@:%.o=%.d) $(dir $@) > $(@:%.o=%.d) && \
	g++ -MM -MG -P -w -I${SCA4CPP_HOME}/runtime/core/src -I${SDO4CPP}/include -I${AXISCPP_DEPLOY}/include -O3 -Wall -c -fmessage-length=0  $< >> $(@:%.o=%.d)
	@echo 'Finished building: $<'
	@echo ' '


