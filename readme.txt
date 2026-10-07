# progetto Domotica

Progetto Finale 

Informazioni Utili per capire il programma
Creazione di una Casa Domotica

1. Lettura dei dispositivi:
  - Leggiamo i vari dispositivi da un file txt (Lista Elettrodomestici) dove si acquisisce la lista degli elettrodomestici, importante per il funzionamento.
  - Se si vuole aggiungere un nuovo elettrodomestico, bisogna separare le varie voci con ESATTAMENTE ',' e senza spazio.
  - Un dispositivo viene automaticamente riconosciuto come "Produttore di Energia" o "Consumatore di Energia" semplicemente inserendo il valore positivo o negativo di produzione di energia.
  - Abbiamo aggiunto un ulteriore parametro tra i dispostivi: 'Priorità'. Quest'ultimo è un numero che va da 1 a 3, dove 1 indica priorità massima, ovvero gli ultimi che vengono staccati in caso di superamento del consumo di energia, mentre 3 la minima. 


2. Conversione delle ore:
  - Tutte le ore che trattiamo sono automaticamente convertite in minuti.


3. Gestione degli oggetti:
  - Per la gestione dei vari oggetti all'interno della casa usiamo un vettore di puntatori, utile sia per poter eseguire il downcast sia per poter liberare la memoria quando viene chiamato il distruttore, evitando memory leak.


Politiche adottate


1. Spegnimento automatico:
  - Se accendo un dispositivo e mentre è acceso parte un timer, il dispositivo si spegnerà automaticamente alla fine del timer.


2. Orari di accensione per dispositivi CP:
  - È possibile inserire più orari di accensione per i dispositivi CP. Tutti gli orari di accensione, se sono dopo l'orario attuale della casa, vengono inseriti in un vector. Quando viene chiamata `set time`, il dispositivo imposta autonomamente il prossimo orario di inizio preso dal vettore tramite una funzione `prossimaAccensione()`. Se finisce il ciclo, invoca di nuovo la funzione per impostare l'orario seguente. In questo modo è possibile inserire più orari di accensione per uno stesso dispositivo, purché non si accavallino con la durata di un ciclo.


3. Timer per dispositivi M:
  - Si possono inserire più timer e più orari di accensione per i dispositivi M.


4. Rimozione dei timer:
  - La funzione `rm ${DEVICENAME}` elimina TUTTI i timer associati a un dispositivo se ve ne sono associati più di uno. (La specifica dice: "Rimuove il timer associato al dispositivo", ma non definisce cosa fare se ve ne sia più di uno).


5. Reset dei timer:
  - La funzione `reset timers` da specifica prevede: "Rimuove i timer di TUTTI i dispositivi. Tutti i dispositivi rimangono nel loro stato attuale (accesi o spenti)." Nella mail inviata il 27 dicembre 2024, 12:51, si dice: "Il timer ha senso solo per i dispositivi M, poiché i CP hanno un ciclo di durata prefissata (per questi dispositivi ha quindi senso solo il trigger di accensione, non quello di spegnimento)". Abbiamo deciso che il metodo `reset timers` viene applicato SOLO sui dispositivi M. Inoltre, dato che non è specificato quanti timer deve rimuovere, come per `rm ${DEVICENAME}`, rimuoviamo TUTTI i timer presenti su ogni singolo dispositivo se presenti.


6. Reset del tempo:
  - La funzione `reset time` spegne anche tutti i dispositivi. Nella specifica viene indicato "Riporta tutti i dispositivi alle condizioni iniziali", ma non è chiarito cosa si intende per "condizioni iniziali". Se si potessero impostare, dovrebbe essere scritto da qualche parte almeno il modo per poterle impostare, ma ciò non è presente.


7. Stato dei dispositivi:
  - Presenza di un metodo `print` che permette di sapere quali dispositivi sono accesi o spenti in un determinato momento.


8. Spegnimento del programma:
  - Un ulteriore metodo per spegnere il programma è digitando '0', così non si è obbligati ad arrivare fino a fine giornata.


9. File di input:
  - È possibile usare un file `../input.txt` dove si possono già mettere tutte le funzioni, una per ogni riga, le quali vengono acquisite autonomamente dal programma, senza essere obbligati a scriverle una ad una. Per usare questa funzione basta togliere i commenti dalle righe 175-179, 233-237 del Main.cpp e commentare la riga 240.


10. Valore di assorbimento massimo:
   - Il valore di assorbimento massimo lo si inserisce come prima istruzione quando si esegue il programma.

11. Priorità dispositivi:
   - Il parametro priorità è obbligatorio da inserire ma non da utilizzare, in caso si volesse ignorare, quindi spegnere i dispositivi nel ordine inverso rispetto all'accensione, basta commentare le righe 166-169 del file Casa_2.cpp
