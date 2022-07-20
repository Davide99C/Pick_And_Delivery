# Titolo: Pick_And_Delivery
# Progetto LABIAGI
# Descrizione:
Il progetto riguarda la gestione di un Robot che sia in grado di ricevere pacchi da utenti e consegnarli ad altri.
Per l'avvio del programma è sufficiente aprire l'interfaccia web (connessa alla porta 9002 del localhost) tramite il file html specifico e aprire un'altra interfaccia per la gestione dell'ambiente ROS (alla porta 9001 del localhost) dalla quale avviare:
  - roscore
  - stage
  - mapserver
  - rviz
  - localize (localizzando manualmente la posizione del robot su rviz basandosi sulla posizione del Robot in stage)
  - planner
  - follower(static)

Una volta avviati i nodi qui sopra riportati basterà avviare su tre terminali differenti il Main (che inizierà la comunicazione tra interfaccia utente e il Robot), il nodo Sender (che si metterà in ascolto dell'utente per l'invio delle coordinate) e il nodo Move (che riceverà le coordinate dal Sender e setterà il nuovo goal del Robot).

Fatto questo non resta che utilizzare l'interfaccia utente e vedere tramite rviz il lavoro del Robot!
  
// Progetto realizzato grazie all'ambiente ROS e l'utilizzo della libreria libwebsocket.
