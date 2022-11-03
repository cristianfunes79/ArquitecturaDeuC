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

- Los Cortex-M tienen una memoria con un espacio de direcciones lineal de 4GB (ya que cuentan con direccionamiento de 32bits). Este espacio de memoria se divide en varias regiones predefinidas para ser utilizadas como regiones de memoria o para periféricos. Esto permite que el procesador pueda ser diseñado para una mejor performance. Por ejemplo, los procesadores Cortex-M3 y Cortex-M4 tienen múltiples interfaces de bus para permitir el acceso simultáneo a la región de memoria de programa y a la región de memoria SRAM o de periféricos. También cuenta opcionalmente con una zona de memoria denominada bit banding. Cuando se incluye esta funcionalidad, aparecen 2 zonas de memoria de 1MB que son direccionables de a 1 bit. Los 4GB de memoria se particionan en:
- a. Acceso a código de programa (por ej. region CODE)
- b. Acceso a datos (por ej. region SRAM)
- c. Periféricos
- d. Registros de control internos y de debug del procesador (por ej. Private Peripheral Bus)


5. ¿Qué ventajas presenta el uso de los “shadowed pointers” del PSP y el MSP?
6. Describa los diferentes modos de privilegio y operación del Cortex M, sus relaciones y
como se conmuta de uno al otro. Describa un ejemplo en el que se pasa del modo
privilegiado a no priviligiado y nuevamente a privilegiado.
7. ¿Qué se entiende por modelo de registros ortogonal? Dé un ejemplo
8. ¿Qué ventajas presenta el uso de intrucciones de ejecución condicional (IT)? Dé un
ejemplo
9. Describa brevemente las excepciones más prioritarias (reset, NMI, Hardfault).
10. Describa las funciones principales de la pila. ¿Cómo resuelve la arquitectura el llamado
a funciones y su retorno?
11. Describa la secuencia de reset del microprocesador.
12. ¿Qué entiende por “core peripherals”? ¿Qué diferencia existe entre estos y el resto de
los periféricos?
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
