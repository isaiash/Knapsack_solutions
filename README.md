# Knapsack_solutions

En *codes* están todos los .cpp y el makefile para *cplex*.

En *executables* están los ejecutables de los códigos compilados con: g++ -std=c++11 -O3 algoritmo.cpp -o algoritmo
Para *cplex* hay que hacer *make* dentro del directorio *cplex*. Para que funcione el makefile hay que indicarle la ruta del Cplex (se instala), pero para el caso, manteniendo la ruta desde tu usuario debería funcionar.

Para los scripts hay que cambiar las rutas y ademas cambiar la forma en que se guardan:
* Lo que hace ahora: Por cada instancia guarda un archivo en la carpeta del algoritmo ejecutado.
* Como deberia ser: En un archivo con el nombre de cada algoritmo guardar todas las salidas.

Dataset: https://drive.google.com/drive/folders/1UYukabXmJokAuTkE8b7QPAzC6oy3N62G?usp=sharing
Ahí iré subiendo todos los dataset, por ahora enfócate en el datasetlvl1 que es el más liviano, despues probamos con los siguientes

Tips:
Con qstat ves toda la cola de ejecución. 
Para ver sólamente los que se está ejecutando: qstat -s r.
