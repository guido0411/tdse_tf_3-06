# tdse_tf_3-06
FIUBA - Electrónica - Taller de Sistemas Embebidos - Trabajo Práctico Final

# El trabajo se presentara en febrero: 
# Introduccion
El trabajo consiste en desarrollar una pava electrica. Cuenta con dos modos de operacion predeterminados, modo MATE y modo Te, ademas el usuario podra seleccionar una temperatura deseada entre un rango de valores preestablecidos. 

# Controladores
El sistema incluira 3 controladores principales que permitiran un uso optimo de la pava:
*  Control de temperatura: El control de temperatura se realizara con un thermosensor que nos permitira hacer un seguimiento de la misma.
*  Control de agua: El control de la presencia de agua es necesario para dar inicio al ciclo de calentamiento. Este se realizara con un laser y fotosensor el cual estara activo siempre que la pava este conectada y cuando deje de detectar el laser por la refraccion del agua podra dar inicio.
*  Control de presencia: Tendremos un switch el cual nos indicara si la pava esta presente o no para poder iniciarla.

# Memoria
El sistema utilizara el reloj del microcontrolador para verificar la hora en tiempo real. De esta manera se podra guardar un tiempo (predetermindo) para activar la pava dadas las condiciones de presencia de pava y de agua. Asimismo, almacenara las temperaturas elegidas por el usuario para los dos modos y la ultima temperatura utilizada. 

# Bluetooth
Se implementara un modulo bluetooth mediante el cual el usuario podra hacer uso de la funcionalidades de la pava, ya sea programar una hora para tener el agua calentada o verificar si esta siendo utilizada. En caso de que no este activa, se podra cortar el suministro de energia para optimizar su consumo.
