################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/new_initialise.c \
../Core/Src/new_main.c \
../Core/Src/new_timer.c \
../Core/Src/part_d_serial.c 

OBJS += \
./Core/Src/new_initialise.o \
./Core/Src/new_main.o \
./Core/Src/new_timer.o \
./Core/Src/part_d_serial.o 

C_DEPS += \
./Core/Src/new_initialise.d \
./Core/Src/new_main.d \
./Core/Src/new_timer.d \
./Core/Src/part_d_serial.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F303xC -c -I../Core/Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/new_initialise.cyclo ./Core/Src/new_initialise.d ./Core/Src/new_initialise.o ./Core/Src/new_initialise.su ./Core/Src/new_main.cyclo ./Core/Src/new_main.d ./Core/Src/new_main.o ./Core/Src/new_main.su ./Core/Src/new_timer.cyclo ./Core/Src/new_timer.d ./Core/Src/new_timer.o ./Core/Src/new_timer.su ./Core/Src/part_d_serial.cyclo ./Core/Src/part_d_serial.d ./Core/Src/part_d_serial.o ./Core/Src/part_d_serial.su

.PHONY: clean-Core-2f-Src

