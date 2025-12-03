 <img src="https://github.com/user-attachments/assets/15600b18-f73b-4ba3-a959-47f0048a1ab6" alt="image2" width="50%">

# TDSE_TF_3-06: Pava Eléctrica
FIUBA - Electrónica - Taller de Sistemas Embebidos - Trabajo Práctico Final

Autores: 
- Pauletich Matías 110892
- Pedranti Guido 111795
- Rivera Salvador 111091

**Fecha: 2do cuatrimestre 2025**


## El trabajo se presentara en febrero

# Introduccion
El trabajo consiste en desarrollar una pava electrica. Cuenta con dos modos de operacion predeterminados, modo MATE y modo TE, ademas el usuario podra seleccionar una temperatura deseada entre un rango de valores preestablecidos. El objetivo de este proyecto es proporcionar una alternativa a las pavas clasicas de manera tal que brinde mayor comodidad a la hora de usarla. Se propone una forma sencilla y optima para calentar el agua, evitando desperciar energía. Por último, 


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

En la siguiente Figura se muestra el diagrama en bloques del sistema con los principales módulos del proyecto
<p align="center">
  <img width="500" height="500" alt="diagrama_sist_emb" src="https://github.com/user-attachments/assets/078236f9-eeb4-4561-830b-e5ca84e10435" />
</p>

<p align="center"><em>Figura 1: Diagrama en bloques del sistema</em></p>



## Requisitos Funcionales
- La pava debe calentar agua hasta temperaturas configurables (modo MATE: ~80°C, modo TÉ: ~100°C, modo personalizado: 60-95°C).
- Debe detectar presencia de agua mediante sensor láser.
- Debe detectar presencia física de la pava mediante switch magnético.
- Debe permitir programar el encendido mediante reloj en tiempo real (propio del uC).
- Debe comunicarse vía Bluetooth para configuración y monitoreo.
- Debe almacenar en memoria EEPROM/Flash las configuraciones de usuario.
- Debe indicar estados mediante LEDs y buzzer (ej: agua lista, error, etc.).

Este proyecto busca innovar en la manera de calentar agua. Cuando se busca una pava eléctrica, se tiene en cuenta el costo principalmente. Empresas como Philips o Samsung se remiten a lo simple, algo que pueda ser utilizado por cualquier persona. En nuestro caso, se busca minimizar el tiempo de espera que ocurre al calentar agua. Pudiendo programar una hora para calentar agua y utilizando energía solo cuando es necesario, se logra una eficiencia muy superior a cualquier producto que se pueda hallar.  

### Casos de Uso Principales
1. **Calentar agua en modo MATE/TÉ**
El usuario podra calentar el agua via Bluetooth, osea sin la necesidad de estar presente. Las precondiciones para que esto ocurra son simplemete que la pava se encuentre sobre la base, que tenga agua y que este enchufada. Una vez cumplidos estos requisitos el sistema podra activarse, en caso de que el usuario realice esta accion diariamente, se podra programar un horario fijo para calentar agua sin necesidad de la programacion manual del usuario. Una vez finalizado el proceso, mediante el buzzer/alarma/app se notificara al usuario que el agua esta lista. Adicionalmente, se guardara el estado (ultima temperatura utilizada) para su próximo uso. Por último, se espera un tiempo fijo desde el fin del uso para verificar si el usuario desea seguir calentando agua. Si no se utiliza mas, se espera que el usuario desenchufe la pava, de lo contrario se le notificará y se cortara el suministro de energía para evitar desperdicios.

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
