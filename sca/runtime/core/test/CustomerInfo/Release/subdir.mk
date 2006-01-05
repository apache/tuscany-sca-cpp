################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
$(ROOT)/CustomerInfoImpl.cpp \
$(ROOT)/CustomerInfoImpl_CustomerInfoService_Proxy.cpp \
$(ROOT)/CustomerInfoImpl_CustomerInfoService_Wrapper.cpp 

OBJS += \
./CustomerInfoImpl.o \
./CustomerInfoImpl_CustomerInfoService_Proxy.o \
./CustomerInfoImpl_CustomerInfoService_Wrapper.o 

DEPS += \
${addprefix ./, \
CustomerInfoImpl.d \
CustomerInfoImpl_CustomerInfoService_Proxy.d \
CustomerInfoImpl_CustomerInfoService_Wrapper.d \
}


# Each subdirectory must supply rules for building sources it contributes
%.o: $(ROOT)/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	@echo g++ -I${SCA4CPP_HOME}/runtime/core/src -I${SDO4CPP}/include -O3 -Wall -c -fmessage-length=0 -o$@ $<
	@g++ -I${SCA4CPP_HOME}/runtime/core/src -I${SDO4CPP}/include -O3 -Wall -c -fmessage-length=0 -o$@ $< && \
	echo -n $(@:%.o=%.d) $(dir $@) > $(@:%.o=%.d) && \
	g++ -MM -MG -P -w -I${SCA4CPP_HOME}/runtime/core/src -I${SDO4CPP}/include -O3 -Wall -c -fmessage-length=0  $< >> $(@:%.o=%.d)
	@echo 'Finished building: $<'
	@echo ' '


