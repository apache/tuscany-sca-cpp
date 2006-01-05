################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
$(ROOT)/osoa/sca/ComponentContext.cpp \
$(ROOT)/osoa/sca/ComponentContextImpl.cpp \
$(ROOT)/osoa/sca/ModuleContext.cpp \
$(ROOT)/osoa/sca/ModuleContextImpl.cpp \
$(ROOT)/osoa/sca/SCA.cpp \
$(ROOT)/osoa/sca/ServiceList.cpp \
$(ROOT)/osoa/sca/ServiceRuntimeException.cpp 

OBJS += \
./osoa/sca/ComponentContext.o \
./osoa/sca/ComponentContextImpl.o \
./osoa/sca/ModuleContext.o \
./osoa/sca/ModuleContextImpl.o \
./osoa/sca/SCA.o \
./osoa/sca/ServiceList.o \
./osoa/sca/ServiceRuntimeException.o 

DEPS += \
${addprefix ./osoa/sca/, \
ComponentContext.d \
ComponentContextImpl.d \
ModuleContext.d \
ModuleContextImpl.d \
SCA.d \
ServiceList.d \
ServiceRuntimeException.d \
}


# Each subdirectory must supply rules for building sources it contributes
osoa/sca/%.o: $(ROOT)/osoa/sca/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	@echo g++ -I${SCA4CPP_HOME}/runtime/core/src -I${SDO4CPP}/include -I${AXISCPP_DEPLOY}/include -O3 -Wall -c -fmessage-length=0 -o$@ $<
	@g++ -I${SCA4CPP_HOME}/runtime/core/src -I${SDO4CPP}/include -I${AXISCPP_DEPLOY}/include -O3 -Wall -c -fmessage-length=0 -o$@ $< && \
	echo -n $(@:%.o=%.d) $(dir $@) > $(@:%.o=%.d) && \
	g++ -MM -MG -P -w -I${SCA4CPP_HOME}/runtime/core/src -I${SDO4CPP}/include -I${AXISCPP_DEPLOY}/include -O3 -Wall -c -fmessage-length=0  $< >> $(@:%.o=%.d)
	@echo 'Finished building: $<'
	@echo ' '


