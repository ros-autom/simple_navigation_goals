################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../2d_nav_goals.cpp \
../2d_nav_goals_2d_pose.cpp \
../2d_nav_goals_2d_pose_wh.cpp \
../defult_2d_nav_goals.cpp \
../free_nav_drive.cpp \
../pose_farm.cpp 

OBJS += \
./2d_nav_goals.o \
./2d_nav_goals_2d_pose.o \
./2d_nav_goals_2d_pose_wh.o \
./defult_2d_nav_goals.o \
./free_nav_drive.o \
./pose_farm.o 

CPP_DEPS += \
./2d_nav_goals.d \
./2d_nav_goals_2d_pose.d \
./2d_nav_goals_2d_pose_wh.d \
./defult_2d_nav_goals.d \
./free_nav_drive.d \
./pose_farm.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


