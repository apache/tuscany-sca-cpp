################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
$(ROOT)/tuscany/sca/model/Binding.cpp \
$(ROOT)/tuscany/sca/model/CPPImplementation.cpp \
$(ROOT)/tuscany/sca/model/CPPInterface.cpp \
$(ROOT)/tuscany/sca/model/Component.cpp \
$(ROOT)/tuscany/sca/model/EntryPoint.cpp \
$(ROOT)/tuscany/sca/model/ExternalService.cpp \
$(ROOT)/tuscany/sca/model/Implementation.cpp \
$(ROOT)/tuscany/sca/model/Interface.cpp \
$(ROOT)/tuscany/sca/model/ModelLoader.cpp \
$(ROOT)/tuscany/sca/model/Module.cpp \
$(ROOT)/tuscany/sca/model/Service.cpp \
$(ROOT)/tuscany/sca/model/ServiceReference.cpp \
$(ROOT)/tuscany/sca/model/Subsystem.cpp \
$(ROOT)/tuscany/sca/model/System.cpp \
$(ROOT)/tuscany/sca/model/WSBinding.cpp \
$(ROOT)/tuscany/sca/model/Wire.cpp \
$(ROOT)/tuscany/sca/model/WireTarget.cpp \
$(ROOT)/tuscany/sca/model/Wsdl.cpp \
$(ROOT)/tuscany/sca/model/WsdlOperation.cpp 

OBJS += \
./tuscany/sca/model/Binding.o \
./tuscany/sca/model/CPPImplementation.o \
./tuscany/sca/model/CPPInterface.o \
./tuscany/sca/model/Component.o \
./tuscany/sca/model/EntryPoint.o \
./tuscany/sca/model/ExternalService.o \
./tuscany/sca/model/Implementation.o \
./tuscany/sca/model/Interface.o \
./tuscany/sca/model/ModelLoader.o \
./tuscany/sca/model/Module.o \
./tuscany/sca/model/Service.o \
./tuscany/sca/model/ServiceReference.o \
./tuscany/sca/model/Subsystem.o \
./tuscany/sca/model/System.o \
./tuscany/sca/model/WSBinding.o \
./tuscany/sca/model/Wire.o \
./tuscany/sca/model/WireTarget.o \
./tuscany/sca/model/Wsdl.o \
./tuscany/sca/model/WsdlOperation.o 

DEPS += \
${addprefix ./tuscany/sca/model/, \
Binding.d \
CPPImplementation.d \
CPPInterface.d \
Component.d \
EntryPoint.d \
ExternalService.d \
Implementation.d \
Interface.d \
ModelLoader.d \
Module.d \
Service.d \
ServiceReference.d \
Subsystem.d \
System.d \
WSBinding.d \
Wire.d \
WireTarget.d \
Wsdl.d \
WsdlOperation.d \
}


# Each subdirectory must supply rules for building sources it contributes
tuscany/sca/model/%.o: $(ROOT)/tuscany/sca/model/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	@echo g++ -D_DEBUG -I${SCA4CPP_HOME}/runtime/core/src -I${SDO4CPP}/include -I${AXISCPP_DEPLOY}/include -O0 -g3 -Wall -c -fmessage-length=0 -o$@ $<
	@g++ -D_DEBUG -I${SCA4CPP_HOME}/runtime/core/src -I${SDO4CPP}/include -I${AXISCPP_DEPLOY}/include -O0 -g3 -Wall -c -fmessage-length=0 -o$@ $< && \
	echo -n $(@:%.o=%.d) $(dir $@) > $(@:%.o=%.d) && \
	g++ -MM -MG -P -w -D_DEBUG -I${SCA4CPP_HOME}/runtime/core/src -I${SDO4CPP}/include -I${AXISCPP_DEPLOY}/include -O0 -g3 -Wall -c -fmessage-length=0  $< >> $(@:%.o=%.d)
	@echo 'Finished building: $<'
	@echo ' '


