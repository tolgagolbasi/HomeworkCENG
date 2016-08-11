################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Elliptic\ Curve\ Method.cpp 

OBJS += \
./src/Elliptic\ Curve\ Method.o 

CPP_DEPS += \
./src/Elliptic\ Curve\ Method.d 


# Each subdirectory must supply rules for building sources it contributes
src/Elliptic\ Curve\ Method.o: ../src/Elliptic\ Curve\ Method.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"src/Elliptic Curve Method.d" -MT"src/Elliptic\ Curve\ Method.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


