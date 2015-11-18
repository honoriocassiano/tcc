################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/objectview/Color.cpp \
../src/objectview/Line.cpp \
../src/objectview/ObjectView.cpp 

OBJS += \
./src/objectview/Color.o \
./src/objectview/Line.o \
./src/objectview/ObjectView.o 

CPP_DEPS += \
./src/objectview/Color.d \
./src/objectview/Line.d \
./src/objectview/ObjectView.d 


# Each subdirectory must supply rules for building sources it contributes
src/objectview/%.o: ../src/objectview/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


