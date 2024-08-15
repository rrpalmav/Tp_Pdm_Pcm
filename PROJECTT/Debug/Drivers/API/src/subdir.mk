################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/API/src/API_debounce.c \
../Drivers/API/src/API_delay.c \
../Drivers/API/src/API_ds3231.c \
../Drivers/API/src/API_numpad_4x4.c \
../Drivers/API/src/API_ssd1306.c \
../Drivers/API/src/API_uart.c \
../Drivers/API/src/fonts.c 

OBJS += \
./Drivers/API/src/API_debounce.o \
./Drivers/API/src/API_delay.o \
./Drivers/API/src/API_ds3231.o \
./Drivers/API/src/API_numpad_4x4.o \
./Drivers/API/src/API_ssd1306.o \
./Drivers/API/src/API_uart.o \
./Drivers/API/src/fonts.o 

C_DEPS += \
./Drivers/API/src/API_debounce.d \
./Drivers/API/src/API_delay.d \
./Drivers/API/src/API_ds3231.d \
./Drivers/API/src/API_numpad_4x4.d \
./Drivers/API/src/API_ssd1306.d \
./Drivers/API/src/API_uart.d \
./Drivers/API/src/fonts.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/API/src/%.o Drivers/API/src/%.su Drivers/API/src/%.cyclo: ../Drivers/API/src/%.c Drivers/API/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/WORKSPACE_STM32/PROJECT/PROJECTT/Drivers/API/inc" -I"C:/WORKSPACE_STM32/PROJECT/PROJECTT/Drivers/API/src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-API-2f-src

clean-Drivers-2f-API-2f-src:
	-$(RM) ./Drivers/API/src/API_debounce.cyclo ./Drivers/API/src/API_debounce.d ./Drivers/API/src/API_debounce.o ./Drivers/API/src/API_debounce.su ./Drivers/API/src/API_delay.cyclo ./Drivers/API/src/API_delay.d ./Drivers/API/src/API_delay.o ./Drivers/API/src/API_delay.su ./Drivers/API/src/API_ds3231.cyclo ./Drivers/API/src/API_ds3231.d ./Drivers/API/src/API_ds3231.o ./Drivers/API/src/API_ds3231.su ./Drivers/API/src/API_numpad_4x4.cyclo ./Drivers/API/src/API_numpad_4x4.d ./Drivers/API/src/API_numpad_4x4.o ./Drivers/API/src/API_numpad_4x4.su ./Drivers/API/src/API_ssd1306.cyclo ./Drivers/API/src/API_ssd1306.d ./Drivers/API/src/API_ssd1306.o ./Drivers/API/src/API_ssd1306.su ./Drivers/API/src/API_uart.cyclo ./Drivers/API/src/API_uart.d ./Drivers/API/src/API_uart.o ./Drivers/API/src/API_uart.su ./Drivers/API/src/fonts.cyclo ./Drivers/API/src/fonts.d ./Drivers/API/src/fonts.o ./Drivers/API/src/fonts.su

.PHONY: clean-Drivers-2f-API-2f-src

