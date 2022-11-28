# Preguntas orientadoras
1. Describa brevemente los diferentes perfiles de familias de
microprocesadores/microcontroladores de ARM. Explique alguna de sus diferencias
características.

- **Perfil A**: *Diseñado para plataformas que ejecuten aplicaciones (por ej. SOs: iOS, Windows, Android, Linux, etc). Estas aplicaciones requieren la máxima capacidad de procesamiento posible y soporte para memoria virtual (MMU). Algunos ejemplos de productos que pueden utilizar este tipo de procesador son los smartphones, tablets, smart-tv, etc. Algo a tener en cuenta en estos casos es que no se necesita cumplir con exigencias de tiempo real. Este perfil tiene una arquitectura ARMv7-A.*
- **Perfil R**: *Diseñado para sistemas embebidos del tipo high-end en los que se necesite performance de tiempo real (por ej: controladores de disco duro, sistemas autónomos, etc.). En estos sistemas la baja latencia y el determinismo son lo más importante. Este perfil tiene una arquitectura ARMv7-R.*
- **Perfil M**: *Diseñado para sistemas del tipo microcontrolador, donde interesa cumplir con una seria de requisitos tales como el bajo costo, bajo consumo de energía, baja latencia de interrupciones. Al mismo tiempo se espera obtener un comportamiento determinístico. Este perfil tiene una arquitectura ARMv7-M y ARMv6-M.*


# Cortex M
1. Describa brevemente las diferencias entre las familias de procesadores Cortex M0, M3 y
M4.

Para describir las diferencias entre las distintas familias de procesadores se tuvieron en cuentas los siguientes aspectos:
- Set de instrucciones: Los Cortex-M0 y Cortex-M0+ soportan 56 instrucciones, la mayoría de 16 bits. Los cortex-M3 soportan más de 100 instrucciones y los cortex-M4 además tienen instrucciones de DSP y opcionalmente de punto flotante.
- Complejidad de la arquitectura: Los Cortex-M0 son los procesadores más pequeños (contienen aproximadamente 12000 compuertas lógicas). Los Cortex-M0+ son similares a los M0 pero con mejor eficiencia energética. Los Cortex-M3 y Cortex-M4 tienen mejores características de sistema y de debug, pero con una mayor complejidad, lo que implica un mayor número de compuertas lógicas (aproximadamente 40000).
- Soporte para interrupciones: Los Cortex-M0 y Cortex-M0+ soportan hasta 32 interrupciones con 4 niveles de prioridades, mientras que los Cortex-M3 y Cortex-M4 soportan hasta 240 interrupciones con hasta 256 niveles de prioridades.
- FPU: Los Cortex-M4 tienen soporte opcional para FPU de precisión simple.
- MPU: Los Cortex-M0 no tienen soporte para protección de memoria, mientras que los Cortex-M0+, Cortex-M3 y Cortex-M4 tienen la opción de incorporar MPU.



2. ¿Por qué se dice que el set de instrucciones Thumb permite mayor densidad de código?
Explique

- Porque al ser instrucciones de 16 bits, se necesita menos memoria para realizar una misma tarea que con instrucciones de 32 bits.

3. ¿Qué entiende por arquitectura load-store? ¿Qué tipo de instrucciones no posee este
tipo de arquitectura?
- Una arquitectura load store es aquella en la que los datos deben ser cargados desde la memoria a un registro antes de poder procesarlos, y deben volver a escribirse en la memoria una vez procesados.
4. ¿Cómo es el mapa de memoria de la familia? 
Los Cortex-M tienen una memoria con un espacio de direcciones lineal de 4GB (ya que cuentan con direccionamiento de 32bits). Este espacio de memoria se divide en varias regiones predefinidas para ser utilizadas como regiones de memoria o para periféricos. Esto permite que el procesador pueda ser diseñado para una mejor performance. Por ejemplo, los procesadores Cortex-M3 y Cortex-M4 tienen múltiples interfaces de bus para permitir el acceso simultáneo a la región de memoria de programa y a la región de memoria SRAM o de periféricos. También cuenta opcionalmente con una zona de memoria denominada bit banding. Cuando se incluye esta funcionalidad, aparecen 2 zonas de memoria de 1MB que son direccionables de a 1 bit. Los 4GB de memoria se particionan en:
- a. Acceso a código de programa (por ej. region CODE)
- b. Acceso a datos (por ej. region SRAM)
- c. Periféricos
- d. Registros de control internos y de debug del procesador (por ej. Private Peripheral Bus)


5. ¿Qué ventajas presenta el uso de los “shadowed pointers” del PSP y el MSP?
La ventaja de usar "shadowed pointers" es que tenemos 2 stack pointers disponibles: 
- El MSP (Main Stack Pointer): es el stack pointer por defecto. Se usa en Thread mode cuando el bit CONTROL[1] (SPSEL) = 0, y se usa siempre en modo Handler.
- El PSP (Processor Stack Pointer): se usa en modo Thread cuando SPSEL = 1.\

En sistemas con un SO o RTOS, los exception handlers usan el MSP, mientras que las tareas de aplicacion usan el PSP. Cada tarea de aplicación tiene su propio espacio de stack, y el codigo encargado del cambio de contexto en el SO actualiza el PSP cada vez que se produce un cambio de contexto. En estos casos, el uso de shadowed pointers tiene los siguientes beneficios:
- Si una aplicacion tiene algún problema que corrompe el stack, el stack utilizado por el SO y por otras tareas permanece intacto, mejorando la robustez del sistema.
- El espacio de stack de cada tarea solo tiene que cubrir el máximo stack + 1 nivel de stack frame (maximo 9 words incluyendo padding en Cortex-M3 o Cortex-M4 sin FPU, o máximo 27 words en Cortex-M4 con FPU). El stack necesario para las ISR y nested interrupt handling se reserva en el main stack solamente.
- Hace más fácil crear un SO eficiente para los procesadores Cortex-M.
- Un SO también puede utilizar la MPU para definir la region del stack que puede usar una tarea de aplicacion.

6. Describa los diferentes modos de privilegio y operación del Cortex M, sus relaciones y
como se conmuta de uno al otro. Describa un ejemplo en el que se pasa del modo
privilegiado a no priviligiado y nuevamente a privilegiado.
Los Cortex-M tienen 2 estados de operación y 2 modos. Además, los procesadores pueden tener niveles de acceso privilegiado y no provilegiado. El nivel de acceso privilegiado puede acceder a todos los recursos del procesador, mientras que el no privilegiado tiene algunas zonas de memoria no accesibles, y algunas operaciones no disponibles. El nivel de acceso no privilegiado no está disponible en los Cortex-M0 y es opcional en los Cortex-M0+.
* Estados de operación: 
    - Estado de Debug: Cuando el procesador es detenido (por ejemplo por un debuggger, o porque alcanza un breakpoint), entra en estado de Debug y deja de ejecutar instrucciones.
    - Estado Thumb: Si el procesador está ejecutando código de programa (instrucciones Thumb), está en estado Thumb. A diferencia de otros procesadores ARM, no hay estado ARM porque los Cortex-M no soportan el set de insrucciones ARM.
* Modos de operación:
    - Modo Handler: Cuando se ejecuta un exception handler tal como una ISR. Cuando el procesador está en handler mode, siempre tiene nivel de acceso privilegiado.
    - Modo Thread: Cuando se ejecuta código de programa de aplicación, el procesador puede estar con nivel de acceso privilegiado o no privilegiado. Esto se controla mediante un registro especial llamado CONTROL (más específicamente, el bit0 [nPRIV]). El programa puede cambiar de nivel de acceso privilegiado a no privilegiado, pero no puede pasar de no privilegiado a privilegiado. Si es necesario, el procesador tiene que usar un mecanismo de excepciones para manejar el cambio.
Por defecto, los procesadores Cortex-M arrancan en modo Thread privilegiado y en estado Thumb. 

* Ejemplo:
    - Luego de un reset, se ejecuta código de programa en modo Thread privilegiado.
    - Se realiza un cambio a modo Thread no privilegiado escribiendo en 1 el bit0 del registro CONTROL. Entonces: CONTROL:nPRIV = 1
    - Se continua ejecutando el programa en modo Thread no privilegiado.
    - Se produce una excepción, lo que produce un cambio a modo Handler privilegiado.
    - Una vez que se atiende la excepcion, se retorna a modo Thread no privilegiado.

7. ¿Qué se entiende por modelo de registros ortogonal? Dé un ejemplo
Los registros son ortogonales cuando cualquier instrucción aplicable a un registro es igualmente aplicable a otro registro. En los ARMv7, los registros r0 a r12 son ortogonales. 
    - Ej: MOV R4, R0; Copy value from R0 to R4
    
8. ¿Qué ventajas presenta el uso de intrucciones de ejecución condicional (IT)? Dé un ejemplo
El uso de instrucciones IT (IF-THEN) puede ayudar a mejorar la performance del código significativamente porque evita algunas de las penalidades de las instrucciones de salto, y también reduce el número de instrucciones de salto. Por ejemplo, un bloque corto de código IF-THEN-ELSE que normalmente requiere de un salto condicional puede ser reemplazado por una simple instrucción IT.

9. Describa brevemente las excepciones más prioritarias (reset, NMI, Hardfault).
- Reset: La excepción de reset se ejecuta tan pronto como el procesador sale del estado de reset. En algunos casos el reset handler puede contener código de inicialización de hardware.
- NMI: es similar a una IRQ, pero no puede ser deshabilitada y tiene el nivel más alto de prioridad (después de la excepción de reset). Es muy útil en sistemas críticos como control industrial o automotive. Dependiendo del diseño del microcontrolador, el NMI puede ser usado para power failure handling, o puede estar conectado a un watchdog para resetear el sistema si el mismo dejó de responder. Como el NMI no puede ser deshabilitado mediante ningún registro de control, la responsividad está garantizada.
- HardFault: es una excepción dedicada al manejo de condiciones de falla durante la ejecución de un programa. Esta condición de falla puede ser tratar de ejecutar un op code desconocido, una falla en el bus o en la memoria, u operaciones no permitidas.

10. Describa las funciones principales de la pila. ¿Cómo resuelve la arquitectura el llamado a funciones y su retorno?


11. Describa la secuencia de reset del microprocesador.

En un microcontrolador Cortex-M, puede haber 3 tipos de reset:
- Power on reset: se resetea todo en el microcontrolador. Esto incluye el procesador y su bloque de debug y periféricos.
- Sytem reset: se resetea solamente el procesador y los periféricos, pero no el bloque de debug del procesador.
- Processor reset: se resetea el procesador solamente.
Luego de un reset y antes de que el procesador comience a ejecutar código, el Cortex-M lee las primeras 2 palabras de la memoria. El comienzo de la memoria contiene la tabla de vectores, y los primeros 2 valores de la tabla de vectores son el valor inicial del MSP y el vector de reset, que es la dirección inicial del reset handler. Luego de que estas 2 palabras son leídas por el procesador, el mismo setea el MSP y el PC con estos valores.
El seteo del MSP es necesario porque algunas excepciones como la NMI o el HardFault se pueden producir apenas se salga del reset, y el MSP va a ser necesario en ese caso para pushear algunos datos del estado del procesador antes de atender la excepción.

12. ¿Qué entiende por “core peripherals”? ¿Qué diferencia existe entre estos y el resto de
los periféricos?\
Los core peripherals son los periféricos propios del procesador y son definidos por ARM. Los "device peripherals" son periféricos del SoC y dependen de cada fabricante (múltiples dispositivos de un mismo fabricante pueden o no tener los mismos periféricos).


13. ¿Cómo se implementan las prioridades de las interrupciones? Dé un ejemplo
14. ¿Qué es el CMSIS? ¿Qué función cumple? ¿Quién lo provee? ¿Qué ventajas aporta?
15. Cuando ocurre una interrupción, asumiendo que está habilitada ¿Cómo opera el
microprocesador para atender a la subrutina correspondiente? Explique con un ejemplo
17. ¿Cómo cambia la operación de stacking al utilizar la unidad de punto flotante?
16. Explique las características avanzadas de atención a interrupciones: tail chaining y late
arrival.
17. ¿Qué es el systick? ¿Por qué puede afirmarse que su implementación favorece la
portabilidad de los sistemas operativos embebidos?
18. ¿Qué funciones cumple la unidad de protección de memoria (MPU)?
19. ¿Cuántas regiones pueden configurarse como máximo? ¿Qué ocurre en caso de haber
solapamientos de las regiones? ¿Qué ocurre con las zonas de memoria no cubiertas por las
regiones definidas?
20. ¿Para qué se suele utilizar la excepción PendSV? ¿Cómo se relaciona su uso con el resto
de las excepciones? Dé un ejemplo
21. ¿Para qué se suele utilizar la excepción SVC? Expliquelo dentro de un marco de un
sistema operativo embebido.
# ISA
1. ¿Qué son los sufijos y para qué se los utiliza? Dé un ejemplo
2. ¿Para qué se utiliza el sufijo ‘s’? Dé un ejemplo
3. ¿Qué utilidad tiene la implementación de instrucciones de aritmética saturada? Dé un
ejemplo con operaciones con datos de 8 bits.
4. Describa brevemente la interfaz entre assembler y C ¿Cómo se reciben los argumentos
de las funciones? ¿Cómo se devuelve el resultado? ¿Qué registros deben guardarse en la
pila antes de ser modificados?
5. ¿Qué es una instrucción SIMD? ¿En qué se aplican y que ventajas reporta su uso? Dé un
ejemplo.
