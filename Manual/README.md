# SimpleStack Oficial Manual (S.O.M)
**Antes de continuar: Este manual puede recibir cambios o reescrituras, por favor si vas a usar SimpleStack (SS), lee constantemente o revisa los commits.**

**SimpleStack** es un lenguaje de arquitectura de pila, donde todas las operaciones se hacen con dos valores superiores de la pila. Sigue el principio LIFO (Last-In, First-Out).

# 1. Instrucciones de manipulacion de la pila:
# LOAD:
LOAD <argumento>, es la operacion fundamental para introducir datos en la ejecucion, crea un nuevo valor constantey lo coloca en la cima de la pila. 

# requisitos: 
El argumento <valor> debe ser un numero entero (constante inmediato)

# ejemplos de uso:
LOAD 5 //[5]
LOAD 12 //[12, 5]

# DISCARD:
DISCARD elimina el valor que se encuentre en cima de la pila. Es una operacion de "desapilar" (POP), pero el valor es ignorado o se pierde, sin ser guardado en ningun registro. Es util para limpiar la pila de resultados intermedios cuando ya no se nesesitan. 
(NOTA: DISCARD NO USA ARGUMENTOS)

# requisitos:
debe haber ya un valor en la cima de la pila.

# ejemplos de uso:
LOAD 10 [10] //cargamos el valor 1o en la cima de la pila
DISCARD [ ] //eliminamos el valor ubicado en la cima de la pila quedando vacia.

# DUPLICATE:
DUPLICATE toma el valor de la cima de la pila y crea una copia identica, apilando esa copia inmediatamente encima. Es util cuando el mismo valor debe ser utilizado por dos operaciones consecutivas.

# requisitos:
debe haber un valor ya en la cima de la pila.

# ejemplo de uso:
LOAD 10 [10] //cargamos el valor 10
DUPLICATE [10, 10] //duplicamos el valor

# SWAP:
SWAP intercambia las posiciones de los dos valores que se encuentran en la cima de la pila. El valor superior pasa a ser el segundo, y el segundo pasa a ser el superior.

# requisitos:
debe haber dos valores ya en la cima de la pila.

# ejemplo de uso:
LOAD 5 [5] //cargamos el valor 5
LOAD 10 [10, 5] //cargamos el valor 10
SWAP [5, 10] // 10 y 5 intercambian sus posiciones
 
# 2.Instrucciones Aritmeticas
Estas intrucciones sacan los dos valores superiores de la pila (A y B), realizan la operacion y colocan el resultado de nuevo en la cima de la pila.

# ADD:
ADD saca los dos valores superiores y los suma. El resultado A + B se apila.

# requisitos:
debe haber dos valores ya en la cima de la pila.

# ejemplo de uso:
LOAD 5 [5] //cargamos el valor 5
LOAD 4 [4,5] //cargamos el valor 4
ADD [9] //sumamos los dos valores dando 9

# SUB:
SUB saca los dos valores superiores y los resta. el resultado A - B se apila. 
(NOTA: El valor mas antiguo apilado (B) es siempre el minuidendo.)

# requisitos:
debe haber dos valores ya en la cima de la pila.

# ejemplos de uso: 
LOAD 5 [5]
LOAD 4 [4,5]
SUB [-1]

# MUL
MUL saca dos valores superiores de la pila y los multiplica. El resultado A * B se apila.

# requisitos:
debe haber ya dos valores en la cima de la pila.

# ejemplo de uso:
LOAD 2 [2]
LOAD 4 [4,2]
MUL [8]

# 3. instrucciones del sistema (OS):
**estas intrucciones gestionan la interracion de SS con el entorno de ejecucion (el sistema operativo)**

# EXIT:
EXIT finaliza la ejecucion del programa SS y devuelve el control al sistema operativo. (al shell o a la terminal que lo ejecuto)

# requisitos:
ser la ultima instruccion del programa.

# ejemplo de uso: 
primeer caso:
LOAD 7 [7]
LOAD 5 [5,7]
EXIT [5,7]

segundo caso:
LOAD 7 [7]
LOAD 5 [5,7]
DISCART [ ]
EXIT [ ]





