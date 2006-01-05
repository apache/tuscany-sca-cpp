################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
$(ROOT)/CalculatorImpl.cpp \
$(ROOT)/CalculatorImpl_CalculatorService_Proxy.cpp \
$(ROOT)/CalculatorImpl_CalculatorService_Wrapper.cpp 

OBJS += \
./CalculatorImpl.o \
./CalculatorImpl_CalculatorService_Proxy.o \
./CalculatorImpl_CalculatorService_Wrapper.o 

DEPS += \
${addprefix ./, \
CalculatorImpl.d \
CalculatorImpl_CalculatorService_Proxy.d \
CalculatorImpl_CalculatorService_Wrapper.d \
}


# Each subdirectory must supply rules for building sources it contributes
%.o: $(ROOT)/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	@echo g++ -D_DEBUG -I${SCA4CPP}/include -I${SDO4CPP}/include -O0 -g3 -Wall -c -fmessage-length=0 -o$@ $<
	@g++ -D_DEBUG -I${SCA4CPP}/include -I${SDO4CPP}/include -O0 -g3 -Wall -c -fmessage-length=0 -o$@ $< && \
	echo -n $(@:%.o=%.d) $(dir $@) > $(@:%.o=%.d) && \
	g++ -MM -MG -P -w -D_DEBUG -I${SCA4CPP}/include -I${SDO4CPP}/include -O0 -g3 -Wall -c -fmessage-length=0  $< >> $(@:%.o=%.d)
	@echo 'Finished building: $<'
	@echo ' '


