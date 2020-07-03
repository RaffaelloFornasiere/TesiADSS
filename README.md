# Progetto ADSS

# All'interno della cartella "docs" è presente la relazione del progetto e la descrizione del 
funzionamento del risolutore. 

# Versione leggera (senza plotter)
 - aprire la directory src\Solver\CircuitSolver
 - editare il file exec.bat in base ai parametri e ai file con i quali si vuole eseguire 
   l'algoritmo genetico
 - eseguire da cmd il file .bat
In output si vedrà, generazione per generazione, la soluzione migliore dell'algoritmo genetico


# Versione pesante (con il plotter)
 - Aprire la directory src\SolverGrafico\release
 - Eseguire il file SolverGrafico.exe
 - Imettere i parametri e file con i quali si vuole eseguire l'algoritmo genetico
 - Cliccare su "Crea risolutore"
 - Cliccare "Avvia"
  - Se Appare una finestra che chiede di inserire la directory del risolutore
    è necessario immettere il percorso del file "main.exe" all'interno della directory
    src\Solver\CircuitSolver
 - Se nel grafico non si visualizza l'andamento del delay del circuito si provi ad zoommare indietro fino a che
   non appare la linea blu.
   Se invece la linea blu è costante, ingrandendola si potrà vedere meglio l'andamento del delay. In genere l'ottimizzazione è 
   dell'ordine di qualche decimo rispetto al valore di partenza. 
 - Per arrestare l'algoritmo premere "Arresta"

è possibile eseguire più istanze contemporaneamente