# DistanciasSeguras_STM32
Este proyecto fue montado en una tarjeta STM32 NUCLEO-F401RE. 

El entorno de programación utilizado fue Arduino IDE. Para que el entorno de Arduino pueda reconocer la tarjeta se deben seguir los siguientes pasos.

1. Ir a File > Preferences
2. En la sección Settings, buscar "Additional boards manager URLs"
3. Añadir el link al repositorio de tarjetas de STM32: 
´´´
https://github.com/stm32duino/BoardManagerFiles/raw/main/package_stmicroelectronics_index.json
´´´

4. En Select Board debería aparecer el modelo de tarjeta STM32 con el que se desea trabajar. 
