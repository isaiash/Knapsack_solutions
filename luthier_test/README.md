#Test para correr ejecutables en luthier.

Para correr los ejecutables en *executables*, con los directorios dados, se puede usar ./runtest.sh.
Para ejecutar los experimentos especificando otros directorios, correr ./launchExps.sh directamente, con los argumentos correspondientes, especificados en el script.
También se puede modificar el timeout, que está en 5m, en *call_solver.sh*.

**Los directorios de salida (qsubout y results por defecto) se borran para no tener resultados anteriores, tener cuidado!**

Actualmente el script copia las instancias a la máquina donde se van a ejecutar, obtiene los resultados y los copia de vuelta.
Los resultados se guardan en un archivo distinto para cada algoritmo. Por hacer: 

* Copiar las instancias comprimidas (.zip) y descomprimir en la máquina que va a correr los executables. Debería andar más rápido al usar un número más grande de instancias ya que se copian para cada trabajo (1 trabajo por algoritmo).

