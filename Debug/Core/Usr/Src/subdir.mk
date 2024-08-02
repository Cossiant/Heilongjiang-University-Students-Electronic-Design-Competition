################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Usr/Src/Robot.c \
../Core/Usr/Src/Usrmain.c \
../Core/Usr/Src/alpha_beta.c \
../Core/Usr/Src/board.c \
../Core/Usr/Src/data.c \
../Core/Usr/Src/font.c \
../Core/Usr/Src/get_angle.c \
../Core/Usr/Src/key.c \
../Core/Usr/Src/oled.c 

OBJS += \
./Core/Usr/Src/Robot.o \
./Core/Usr/Src/Usrmain.o \
./Core/Usr/Src/alpha_beta.o \
./Core/Usr/Src/board.o \
./Core/Usr/Src/data.o \
./Core/Usr/Src/font.o \
./Core/Usr/Src/get_angle.o \
./Core/Usr/Src/key.o \
./Core/Usr/Src/oled.o 

C_DEPS += \
./Core/Usr/Src/Robot.d \
./Core/Usr/Src/Usrmain.d \
./Core/Usr/Src/alpha_beta.d \
./Core/Usr/Src/board.d \
./Core/Usr/Src/data.d \
./Core/Usr/Src/font.d \
./Core/Usr/Src/get_angle.d \
./Core/Usr/Src/key.d \
./Core/Usr/Src/oled.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Usr/Src/%.o Core/Usr/Src/%.su Core/Usr/Src/%.cyclo: ../Core/Usr/Src/%.c Core/Usr/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"E:/Work/STM32/Robot2/esports-stm32-development-master/Core/Usr/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Usr-2f-Src

clean-Core-2f-Usr-2f-Src:
	-$(RM) ./Core/Usr/Src/Robot.cyclo ./Core/Usr/Src/Robot.d ./Core/Usr/Src/Robot.o ./Core/Usr/Src/Robot.su ./Core/Usr/Src/Usrmain.cyclo ./Core/Usr/Src/Usrmain.d ./Core/Usr/Src/Usrmain.o ./Core/Usr/Src/Usrmain.su ./Core/Usr/Src/alpha_beta.cyclo ./Core/Usr/Src/alpha_beta.d ./Core/Usr/Src/alpha_beta.o ./Core/Usr/Src/alpha_beta.su ./Core/Usr/Src/board.cyclo ./Core/Usr/Src/board.d ./Core/Usr/Src/board.o ./Core/Usr/Src/board.su ./Core/Usr/Src/data.cyclo ./Core/Usr/Src/data.d ./Core/Usr/Src/data.o ./Core/Usr/Src/data.su ./Core/Usr/Src/font.cyclo ./Core/Usr/Src/font.d ./Core/Usr/Src/font.o ./Core/Usr/Src/font.su ./Core/Usr/Src/get_angle.cyclo ./Core/Usr/Src/get_angle.d ./Core/Usr/Src/get_angle.o ./Core/Usr/Src/get_angle.su ./Core/Usr/Src/key.cyclo ./Core/Usr/Src/key.d ./Core/Usr/Src/key.o ./Core/Usr/Src/key.su ./Core/Usr/Src/oled.cyclo ./Core/Usr/Src/oled.d ./Core/Usr/Src/oled.o ./Core/Usr/Src/oled.su

.PHONY: clean-Core-2f-Usr-2f-Src

