################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
$(ROOT)/MyValueImpl.cpp \
$(ROOT)/MyValueImpl_MyValueService_Proxy.cpp \
$(ROOT)/MyValueImpl_MyValueService_Wrapper.cpp \
$(ROOT)/MyValueImpl_customerInfo_Proxy.cpp \
$(ROOT)/MyValueImpl_stockQuote_Proxy.cpp 

OBJS += \
./MyValueImpl.o \
./MyValueImpl_MyValueService_Proxy.o \
./MyValueImpl_MyValueService_Wrapper.o \
./MyValueImpl_customerInfo_Proxy.o \
./MyValueImpl_stockQuote_Proxy.o 

DEPS += \
${addprefix ./, \
MyValueImpl.d \
MyValueImpl_MyValueService_Proxy.d \
MyValueImpl_MyValueService_Wrapper.d \
MyValueImpl_customerInfo_Proxy.d \
MyValueImpl_stockQuote_Proxy.d \
}


# Each subdirectory must supply rules for building sources it contributes
%.o: $(ROOT)/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	@echo g++ -D_DEBUG -I${SCA4CPP_HOME}/runtime/core/src -I${SCA4CPP_HOME}/runtime/core/test/CustomerInfo -I${SDO4CPP}/include -O0 -g3 -Wall -c -fmessage-length=0 -o$@ $<
	@g++ -D_DEBUG -I${SCA4CPP_HOME}/runtime/core/src -I${SCA4CPP_HOME}/runtime/core/test/CustomerInfo -I${SDO4CPP}/include -O0 -g3 -Wall -c -fmessage-length=0 -o$@ $< && \
	echo -n $(@:%.o=%.d) $(dir $@) > $(@:%.o=%.d) && \
	g++ -MM -MG -P -w -D_DEBUG -I${SCA4CPP_HOME}/runtime/core/src -I${SCA4CPP_HOME}/runtime/core/test/CustomerInfo -I${SDO4CPP}/include -O0 -g3 -Wall -c -fmessage-length=0  $< >> $(@:%.o=%.d)
	@echo 'Finished building: $<'
	@echo ' '


