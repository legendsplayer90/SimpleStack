//example1.ss

LOAD 9        //Cargamos el valor 9 en la pila                        [9]
LOAD 10       //cargamos el valor 10 en la pila                       [10,9]
ADD           //sumamos los dos valores de la cima de la pila         [19]
LOAD 2        //cargamos el valor 2 en la pila                        [2,19]
MUL           //multiplicamos los dos valores de la cima de la pila   [38]
LOAD 11       //cargamos 11 en la pila                                [11,38]
SUB           //restamos los dos valores de la cima de la pila        [-27]
DISCARD       //borramos los valores dentro de la pila                [ ]
EXIT          //finalizamos el programa                               [ ]
