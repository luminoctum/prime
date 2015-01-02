################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Configure.cpp \
../src/Configure_test.cpp \
../src/DistGrid.cpp \
../src/DistVariable.cpp \
../src/Grid.cpp \
../src/Grid_test.cpp \
../src/Halo.cpp \
../src/Halo_test.cpp \
../src/PatchGrid.cpp \
../src/PatchVariable.cpp \
../src/Variable.cpp \
../src/Variable_test.cpp \
../src/boundary.cpp \
../src/multithreading_test.cpp \
../src/prime.cpp 

OBJS += \
./src/Configure.o \
./src/Configure_test.o \
./src/DistGrid.o \
./src/DistVariable.o \
./src/Grid.o \
./src/Grid_test.o \
./src/Halo.o \
./src/Halo_test.o \
./src/PatchGrid.o \
./src/PatchVariable.o \
./src/Variable.o \
./src/Variable_test.o \
./src/boundary.o \
./src/multithreading_test.o \
./src/prime.o 

CPP_DEPS += \
./src/Configure.d \
./src/Configure_test.d \
./src/DistGrid.d \
./src/DistVariable.d \
./src/Grid.d \
./src/Grid_test.d \
./src/Halo.d \
./src/Halo_test.d \
./src/PatchGrid.d \
./src/PatchVariable.d \
./src/Variable.d \
./src/Variable_test.d \
./src/boundary.d \
./src/multithreading_test.d \
./src/prime.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	mpic++ -D__GXX_EXPERIMENTAL_CXX0X__ -I/opt/mpich2/gnu/include -O0 -g3 -Wall -c -fmessage-length=0 -std=c++0x -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


