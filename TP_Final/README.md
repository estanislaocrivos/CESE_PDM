CESE 2023

Trabajo final para la materia de Programación de Microcontroladores

Autor: Estanislao Crivos

Fecha: Agosto 2023

Este proyecto se trata de un sistema de registro de ingreso y egreso de personal. Como periféricos utiliza un display 16X2 controlado a través del protocolo I2C y un lector RFID controlado a través del protocolo SPI. La idea es que el usuario que desee registrar su ingreso pase una tarjeta RFID compatible por el lector, el sistema autenticará su ID y registrará al usuario, iniciando un contador. Al egresar, el usuario deberá volver a escanear su tarjeta en el sensor y el sistema automáticamente lo dará de baja, mostrándole en pantalla cuánto tiempo pasó desde el registro.

El proyecto fue realizado sobre la plataforma STM32F401RE.