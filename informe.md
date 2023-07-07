Trabajo Práctico 2: 
Logística centralizada de primera milla








Participantes
	

	

	

	

	

	

	

	

	Tadeo Yapoudjian
	tadeyapu@gmail.com
	44788317
	21N719
	





























1. 1. Introducción
El crecimiento del comercio electrónico en los últimos años ha llevado a que empresas logísticas y marketplaces en diferentes regiones, como Latinoamérica y Asia, busquen reducir sus costos operativos. Para los vendedores grandes con altos volúmenes de ventas diarias, suele ser viable planificar visitas para recolectar sus productos. Sin embargo, para los vendedores pequeños que venden solo unos pocos artículos al día (menos de 10 o 15), esta operación logística resulta costosa. En estos casos, los vendedores son responsables de la logística necesaria para entregar los paquetes hasta el punto de recepción de la red, conocido como "primera milla".


Recientemente, hemos obtenido un contrato para ayudar a la startup ThunderPack. Esta empresa no solo brinda servicios logísticos, sino que también administra una red de comercios que ofrecen de manera limitada la posibilidad de almacenar temporalmente paquetes. Luego, las empresas logísticas recolectan los paquetes directamente desde estos puntos, que actúan como puntos de consolidación, simplificando así las operaciones. Nos referiremos a estos puntos de recepción como "depósitos".


Eventos especiales como el Black Friday, Hot Sale y campañas similares generan un aumento en la actividad que supera los niveles habituales, lo que pone a prueba toda la red logística. Para garantizar una buena experiencia a los vendedores que utilizan el servicio, es necesario tomar medidas preventivas para poder responder de la manera más efectiva posible.


Actualmente, los vendedores eligen a qué depósito llevar sus paquetes y pueden cambiar sin previo aviso. Sin embargo, estudios previos han demostrado que esta modalidad genera una utilización ineficiente de la capacidad de almacenamiento de la red, lo cual resulta en reclamos, costos adicionales y una mala experiencia para los vendedores. Por esta razón, ThunderPack está considerando migrar a una modalidad centralizada, donde se asignará a cada vendedor un depósito específico para su uso regular. Esto tiene como objetivo cumplir con las capacidades máximas de cada depósito y minimizar la distancia total recorrida por los vendedores.


En la actualidad, ThunderPack tiene algunas preguntas que busca responder idealmente:


1. ¿Es suficiente la capacidad de la red de depósitos o es necesario expandirla para atender a todos los vendedores?
2. ¿Es posible encontrar una asignación donde los vendedores recorran distancias razonables para entregar sus paquetes?
3. ¿Es factible desarrollar una herramienta que permita experimentar con distintos escenarios y obtener soluciones de buena calidad en pocos minutos? Esta herramienta podría utilizarse en contextos de alta demanda, posiblemente en tiempo real, a medida que se observa el volumen de ventas real.


Responder a estas preguntas ayudará a ThunderPack a mejorar su eficiencia logística y proporcionar un mejor servicio a sus clientes vendedores en situaciones de alta demanda.


2. Decisión 
El problema descrito anteriormente se puede modelar utilizando el Problema de Asignación Generalizada (GAP, Generalized Assignment Problem) en su versión más general. El problema se formula de la siguiente manera:


Se tiene un conjunto de vendedores N = {1, ..., n} y un conjunto de depósitos M = {1, ..., m}, donde cada depósito i ∈ M tiene una capacidad máxima de recepción ci, medida en cantidad de unidades. Para cada vendedor j ∈ N y depósito i ∈ M, se definen dij como la demanda (también en cantidad de unidades) a utilizar y cij como el costo incurrido si se asigna el vendedor j al depósito i.


Una solución se representa como una colección de conjuntos Γ1, ..., Γm ⊆ N, donde Γi es el subconjunto de vendedores asignados al depósito i ∈ M. El objetivo del GAP es:


1. Asignar cada vendedor j ∈ N a exactamente un depósito i ∈ M, es decir, Γi ∩ Γk = ∅ si i ≠ k, para i, k ∈ M.
2. Garantizar que no se exceda la capacidad de cada depósito i ∈ M, es decir, ∑dij ≤ ci para i ∈ M.
3. Minimizar el costo total de la asignación, teniendo en cuenta los costos cij asociados a cada asignación.


En el caso específico de ThunderPack, se considerará cij como la distancia que debe recorrer el vendedor j si se asigna al depósito i. Es importante destacar que, dependiendo de las capacidades y las demandas, puede ser difícil encontrar una solución factible que asigne a todos los vendedores y respete las capacidades establecidas.


Además, se define cmax = max cij para i ∈ M, j ∈ N, que representa la máxima distancia posible que un vendedor puede recorrer. Si hay una solución parcial en la que algunos vendedores no pueden ser asignados a ningún depósito, se asumirá una penalización de 3 × cmax por cada uno de ellos.


En el caso de ThunderPack, la estimación de la demanda para cada vendedor (di) es la misma independientemente del depósito al que se le asigne. Sin embargo, se trabaja con la versión general del GAP para poder abordar casos particulares con datos reales, los cuales serán considerados como una instancia específica para el algoritmo.


2. Descripción de modelos propuestos


Heurísticas constructivas


1. Estrategia golosa (Greedy Min Cost)


La estrategia golosa propuesta consiste en recorrer los vendedores en un orden arbitrario y asignar al primero de ellos al depósito de menor costo. Luego, para el segundo vendedor, se asigna al depósito de menor costo que tenga capacidad disponible. Este proceso se repite para los vendedores restantes, asignándolos a los depósitos de menor costo y con capacidad suficiente.


La estrategia golosa sigue los siguientes pasos:


1. Seleccionar un orden arbitrario para recorrer los vendedores. El implementado es el orden dado por los archivos.
2. Asignar al primer vendedor el depósito de menor costo.
3. Para cada vendedor subsiguiente, asignar al depósito de menor costo que tenga capacidad disponible.
4. Repetir el paso 3 hasta asignar todos los vendedores posibles.
5. Finalmente se obtiene una solución factible.


Esta estrategia golosa busca minimizar los costos de asignación al seleccionar los depósitos de menor costo en cada etapa. Sin embargo, es importante tener en cuenta que esta estrategia no garantiza encontrar la solución óptima global ni una local para el problema.


1.5. Golosa Aleatoria (Greedy Randomized)


Para uso en la metaheurística a detallar más adelante, se desarrollo una variante de la estrategia golosa. A partir de un orden aleatorio de los vendedores, se llevan a cabo los pasos 2-5 de la estrategia golosa.


2. Bin Packing 


La estrategia de Bin Packing optimiza el uso de los depósitos en el contexto del problema de asignación. En este caso, el objetivo es maximizar la utilización de la capacidad de cada depósito asignando a los vendedores de manera eficiente. 
La estrategia sigue los siguientes pasos:
1. Seleccionar un orden arbitrario para recorrer los vendedores. Al igual que en la golosa se dicta por el orden de los vendedores en los archivos.
2. Para cada vendedor, evaluar en qué depósito se dejaría menos espacio libre si se asigna su carga.
3. Asignar al vendedor al depósito que tenga la menor cantidad de espacio libre después de agregar su carga.
4. Actualizar la capacidad del depósito asignado después de cada asignación.
5. Repetir los pasos 2, 3 y 4 hasta asignar todos los vendedores.
En esta estrategia, el enfoque principal es minimizar el espacio libre en los depósitos asignando a los vendedores de manera eficiente. No se tienen en cuenta los costos de asignación, sino únicamente la capacidad restante en cada depósito.
Esta estrategia tampoco garantiza encontrar la solución óptima global para el problema, pero se centra en utilizar de manera eficiente el espacio en los depósitos. De esta forma, reducimos la cantidad de vendedores que quedan sin asignar y reducimos las penalizaciones.
Operadores de búsqueda local
1. Swap
        El operador de búsqueda local Swap genera un vecindario de soluciones S’ a partir de una solución factible S, del cual se obtiene la mejor solución S’ cuyo costo sea mejor que S. Esto se repite hasta que no haya mejora posible. Los pasos a seguir son los siguientes:
1. Dada una solución factible S. Se seleccionan todos los pares de vendedores v, w de forma tal que v y w sean distintos.
   1. A partir de cada par v, w, se determinan los depósitos dv y dw asignados a v y w respectivamente. Si v o w no estan asignados, dv = -1 o dw = -1. Si ambos no se encuentran asignados se corta la exploración para está rama. 
   2. Se determina si la capacidad del deposito dv al quitar la demanda de v permite alojar la demanda de w para dv, y viceversa.
   3. En este caso, se desasigna dv de v y dw de w, para luego hacer un enroque y asignar dv a w y dw a v.
   4. Se almacena la factibilidad del Swap, junto a los vendedores y deposito involucrados, y el costo de una posible nueva solución S’ producto de efectuar el Swap sobre S. Esto se hizo para reducir la complejidad algoritmica.
2. Dadas todas las posibles nuevas soluciones, se selecciona aquella que resulte en el menor costo. 
   1. Si dicho costo es menor que el de la solución S actual, se efectúa el Swap que produjo mejor costo sobre la solución actual y se repite el paso 1.
   2. Si dicho costo es mayor o igual, se detiene la exploración de vecindarios.
De está forma obtenemos una heurística de búsqueda local que explora vecindarios de forma exhaustiva hasta que no exista una mejora sobre el vecindario de mi solución, por lo que encontré un mínimo local.


2. Relocate
        Al igual que el operador de búsqueda local Swap, el operador Relocate genera un vecindario de soluciones S’ a partir de una solución factible S, del cual se obtiene la mejor solución S’ cuyo costo sea mejor que S. Esto se repite hasta que no haya mejora posible. Los pasos a seguir son los siguientes:
1. Dada una solución factible S se seleccionan todos los vendedores.
   1. Para cada vendedor, se evalúa si el costo de reasignar dicho vendedor a otro deposito del que esté asignado es menor, cuya capacidad tambien permita la demanda del vendedor. En el caso del vendedor no estar asignado, siempre el costo será al ser asignado que al no estarlo ya que: Si x \in \float, x \in c = {x1, x2, … xn} x < 3 * max{c}
   2. Se almacena la factibilidad y, si el costo es menor, tambien el vendedor, su nuevo deposito a ser asignado y su costo resultante.
2. Para cada reubicación posible, se obtiene aquella de menor costo resultante.
   1. Si dicho costo es menor que el de la solución S actual, se efectúa el Swap que produjo mejor costo sobre la solución actual y se repite el paso 1.
   2. Si dicho costo es mayor o igual, se detiene la exploración de vecindarios.
Otros operadores
1. Random Destroyer
        Para el desarrollo de la metaheurística se desarrolló un “Destructor” de soluciones factibles.
Primero, se genera un número aleatorio de veces que se realizará la “destrucción” que consiste en lo siguiente:
1. Se genera un nuevo número aleatorio que representa un cliente en el rango de clientes disponibles.
2. Se genera otro número aleatorio que representa un depósito en el rango de depósitos disponibles, incluyendo la opción de no asignar el cliente a ningún depósito (-1 representa esta opción).
3. Se obtiene el depósito previamente asignado al cliente.
4. Si el número generado para el depósito es -1, significa que el cliente no se asignará a ningún depósito en esta iteración y se continúa con la siguiente iteración del bucle. Existió la idea de desasignar vendedores cómo parte de la destrucción, sin embargo nos resultó muy contraproducente a la hora de explorar nuevos vecindarios.
5. Si el cliente no estaba previamente asignado al depósito generado:
a. Se verifica si el depósito tiene capacidad para atender la demanda del cliente.
           b. Si es así, se verifica si el cliente estaba asignado a otro depósito. En caso afirmativo, se desasigna el cliente de ese depósito.
           c. Luego, se asigna el cliente al depósito generado.
        Finalmente se obtiene una nueva solución factible, que se espera se encuentre en un nuevo vecindario.





________________

Metaheuristica
1. Se inicializan algunas variables, como `iter_count` (número máximo de iteraciones), `max_tries` (número máximo de intentos), `cutoff` (límite de iteraciones después del cual se comprueba si detenerse), y `neighbourhood_count` (número de vecinos a explorar en cada iteración).
2. Se crea un vector llamado `solutions` para almacenar los minimos locales encontrados durante el proceso.
3. Inicialmente se plantea una solución 


5. Se establecen variables relacionadas con el seguimiento de intentos fallidos y el tamaño del vecindario explorado en la iteración anterior.


6. Se inicia un bucle principal que itera `iter_count` veces.


7. Dentro del bucle principal, se realiza una comprobación para determinar si se debe detener la búsqueda. Si el número de iteraciones es mayor que `cutoff`, se verifica si las últimas tres soluciones encontradas tienen un costo mayor que las tres soluciones anteriores. Si se cumple esta condición, se incrementa el contador de intentos (`tries`) y se comprueba si se han superado los intentos máximos permitidos (`max_tries`). Si se han superado los intentos máximos, se sale del bucle principal. De lo contrario, se muestra un mensaje indicando el reinicio del intento y se realiza una nueva inicialización utilizando el algoritmo GreedyRandomized.


8. Se genera un número aleatorio para decidir si se debe realizar una destrucción aleatoria. Si el número aleatorio está dentro de un rango especificado, se crea un objeto `RandomDestroyer` y se llama a su método `solve()` para destruir parte de la solución actual. Luego, se obtiene la solución modificada mediante el método `get_solution()` y se asigna a `this->_solution`. Se muestran mensajes de información sobre el costo de la solución destruida y el tiempo transcurrido.


9. Se crea un objeto `Relocate` y se llama a su método `solve()` para realizar una operación de reubicación en la solución actual. Luego, se obtiene la solución modificada mediante el método `get_solution()` y se asigna a `this->_solution`. Se muestra el costo de la solución reubicada y el tiempo transcurrido.


10. Se crea un objeto `Swap` y se establece la solución actual mediante el método `set_solution()`. Luego, se genera un vecindario de soluciones utilizando el método `neighbourhood()` y se ordena el vecindario según el costo de las soluciones.


11. Se selecciona un índice aleatorio del vecindario y se obtiene el intercambio correspondiente. Se actualiza el tamaño del vecindario explorado.


12. Se realiza una comprobación para determinar si el intercambio seleccionado es lo suficientemente bueno como para ser aceptado. Si el costo de la mejor solución encontrada es más del 40% mayor que el costo de la solución actual, se descarta el intercambio y se continúa con el siguiente vecino.


13. Si el intercambio es aceptado, se realiza el intercambio en la solución actual y se recalcula el costo. Se muestran mensajes informativos sobre el intercambio realizado y el tiempo transcurrido.


14. Después de explorar todos los vecinos, se asigna la solución modificada a `this->_solution` y se recalcula su costo.


15. Se guarda la solución actual en el vector `solutions` para su posterior análisis y comparación.


16. Una vez finalizado el bucle principal, se ordenan las soluciones almacenadas en `solutions` en orden ascendente según el costo.


17. La solución con el costo más bajo se asigna a `this->_solution`.


18. Se detiene el temporizador y se calcula el tiempo total de ejecución.


19. La función `Meta::solve()` ha completado su ejecución y se ha encontrado la mejor solución posible dentro de los límites establecidos por los parámetros de la metaheurística.