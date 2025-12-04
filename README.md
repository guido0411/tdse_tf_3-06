 <img src="https://github.com/user-attachments/assets/15600b18-f73b-4ba3-a959-47f0048a1ab6" alt="image2" width="50%">

# TDSE_TF_3-06: Jarra Eléctrica
FIUBA - Electrónica - Taller de Sistemas Embebidos - Trabajo Práctico Final

Autores: 
- Pauletich Matías 110892
- Pedranti Guido 111795
- Rivera Salvador 111091

**Fecha: 2do cuatrimestre 2025**


# Introduccion
El trabajo consiste en desarrollar una jarra electrica. Cuenta con dos modos de operacion predeterminados, modo MATE y modo TE, ademas el usuario podra seleccionar una temperatura deseada entre un rango de valores preestablecidos. El objetivo de este proyecto es proporcionar una alternativa a las pavas clasicas de manera tal que brinde mayor comodidad a la hora de usarla. Se propone una forma sencilla y optima para calentar el agua, evitando desperciar energía.


## Controladores
El sistema incluira 3 controladores principales que permitiran un uso optimo de la pava:
*  Control de temperatura: El control de temperatura se realizara con un thermosensor que nos permitira hacer un seguimiento de la misma.
*  Control de agua: El control de la presencia de agua es necesario para dar inicio al ciclo de calentamiento. Este se realizara con un laser y fotosensor el cual estara activo siempre que la pava este conectada y cuando deje de detectar el laser por la refraccion del agua podra dar inicio.
*  Control de presencia: Tendremos un switch el cual nos indicara si la pava esta presente o no para poder iniciarla.

## Memoria
El sistema utilizara el reloj del microcontrolador para verificar la hora en tiempo real. De esta manera se podra guardar un tiempo (predetermindo) para activar la pava dadas las condiciones de presencia de pava y de agua. Asimismo, almacenara las temperaturas elegidas por el usuario para los dos modos y la ultima temperatura utilizada. 

## Bluetooth
Se implementara un modulo bluetooth mediante el cual el usuario podra hacer uso de la funcionalidades de la pava, ya sea programar una hora para tener el agua calentada o verificar si esta siendo utilizada. En caso de que no este activa, se podra cortar el suministro de energia para optimizar su consumo.

## Diagrama en bloques

En la Figura 1 se muestra el diagrama en bloques del sistema con los principales módulos del proyecto
<p align="center">
  <img width="500" height="500" alt="diagrama_sist_emb" src="https://github.com/user-attachments/assets/078236f9-eeb4-4561-830b-e5ca84e10435" />
</p>

<p align="center"><em>Figura 1: Diagrama en bloques del sistema</em></p>

## Elicitación de requisitos y casos de uso
#### El trabajo se presentara en febrero
| Grupo | ID | Descripción |
| :---- | :---- | :---- |
| Indicadores | 1.1 | El sistema contará con LED's para indicar el modo |
|  | 1.2 | El sistema contará con un buzzer para indicar que termino de calentar |
| Sensores | 2.1 | El sistema contará con un sensor laser para detectar presencia de agua |
| | 2.2 | El sistema contará con un switch magnético para detectar presencia de la pava |
| | 2.3 | El sistema contará con un thermosensor para verificar la temperatura del agua |
| Comunicacion Bluetooth | 3.1 | El sistema se comunicará mediante Bluetooth con con la aplicación |
| | 3.2 | El sistema deberá comunicarle la temperatura a la aplicación |
| Aplicación | 4.1 |La aplicación podra programar el encendido del sistema en un horario programado |
| | 4.2 | La aplicación enviará una notificación al usuario cuando el agua este lista |
| | 4.3 | La aplicación enviará una notificación al usuario cuando este finalice su uso (para apagarla y evitar desperdicio de energía) |
| | 4.4 | La aplicación le permitirá al usuario elegir una temperatura personalizada (60-95°C) |
| Memoria | 5.1 | El sistema almacenará en memoria el último estado de la jarra (última temperatura utilzada) |
<p align="center"><em>Tabla 1: Requisitos del proyecto</em></p>

Este proyecto busca innovar en la manera de calentar agua. Cuando se busca una pava eléctrica, se tiene en cuenta el costo principalmente. Empresas como Philips o Samsung se remiten a lo simple, algo que pueda ser utilizado por cualquier persona. En nuestro caso, se busca minimizar el tiempo de espera que ocurre al calentar agua. Pudiendo programar una hora para calentar agua y utilizando energía solo cuando es necesario, se logra una eficiencia muy superior a cualquier producto que se pueda hallar.  

### Casos de Uso Principales

| Elemento | Definición |
| :---- | :---- |
| Disparador | Se quiere calentar agua para TE |
| Precondiciones | El sistema está encendido. La pava se encuentra en la base y está llena con agua. El sistema se encuentra en modo té |
| Flujo principal | El usuario elige el modo té. El sistema se encuentra encendido y la pava se encuentra en la base y con la cantidad necesaria de agua. Un led se verifica que el sistema se encuentra en modo té. Se acciona el dispositivo. Una vez finalizado, la aplicación envia una notificación de que la jarra termino de calentar. El usuario desconecta la jarra una vez finalizado el uso|
| Flujos alternativos | a. El usuario quiere iniciar el sistema pero no cargo la pava con agua, el buzzer inidicará que la acción no es posible.  b.El usuario deja encendido y por ende enchufado el sistema, la aplicación le envia una notificación indicando que se apagará el sistema y se cortará el suministro de energía |

<p align="center"><em>Tabla 2: Caso de uso 1: El usuario quiere calentar agua para un té</em></p>

| Elemento | Definición |
| :---- | :---- |
| Disparador | Se desea tener el agua caliente para un horario predeterminado |
| Precondiciones | El sistema está encendido. La pava se encuentra en la base y está llena con agua. El reloj del uC está activo|
| Flujo principal | El usuario mediante la aplicación programa un horario para calentar el agua. Previamente, coloca la jarra en la base y llena la misma con agua. Una vez finalizado, el usuario recibe una notificación de que la jarra termino de calentar. El usuario desconecta la jarra una vez finalizado el uso |
| Flujos alternativos | a. El usuario no colocó la jarra en la base previamente ni vierte el agua necesaria, el buzzer indicara que esta acción no es posible b.El reloj del uC no esta activo. Si el reloj no esta activo, no se podrá programar un horario para calentar agua |

<p align="center"><em>Tabla 3: Caso de uso 2: El usuario quiere programar un horario para calentar agua</em></p>

### Hardware a utilizar
#### Obligatorio:
- Dip Switches (selección modo)
- Botones (inicio/cancel)
- LEDs (estado)
- Buzzer (alarmas)
- Módulo HM-10 (Bluetooth)
- EEPROM externa o Flash interna
- Sensor de temperatura (termistor/DS18B20)
- Láser y fototransistor (detección agua)
- Switch magnético (detección pava)

### Software/Arquitectura
- Bare Metal, Event-Triggered System
- Super-Loop con polling e interrupciones
- Tick de 1ms (SysTick)
- Tareas no bloqueantes
- Mínimo 2 modos: NORMAL, SETUP
- Comunicación I2C/SPI para sensores
- Colas/estructuras para comunicación entre módulos
