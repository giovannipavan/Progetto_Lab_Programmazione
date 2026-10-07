//Alessandro Michelazzo 2111551
#include <iostream>
#include <sstream>
#include <vector>
#include <cctype> // Per la funzione isdigit
#include <algorithm> // Per std::any_of

#include "../include/Casa.h"



// funzione per gestire i singoli comandi di input del utente 
bool gestisciComando(const std::string& comando, Casa& home, std::ofstream& Ouput){

    std::istringstream ss(comando);   // crea un flusso di input dalla stringa
    std::string parola;
    std::vector<std::string> paroleInput;  // vettore per memorizzare le parole 
    std::string concatenazione; // nome del dispositivo
    bool primaParola = true; // flag per gestire la prima parola dato che verrà sempre aggiunta 

    std::cout<<std::endl<<"- "<<comando<<std::endl<<std::endl; //stampa il comando
    Ouput <<std::endl<<"- "<< comando << "\n\n";
    std::cout<<home.convertToOrarioStampa()<<std::endl;
    Ouput << home.convertToOrarioStampa() << "\n";

    // aggiungo le varie parole al vettore
    while (ss >> parola) {
        if (primaParola) {
            paroleInput.push_back(parola); // aggiungi la prima parola
            primaParola = false;
        } else {
            // Verifica se la parola è "on", "off" o la parola da concatenare ha almeno un numero (any_of)
            if (parola == "on" || parola == "off"  || std::any_of(parola.begin(), parola.end(), ::isdigit))  {
                // Aggiungi la concatenazione corrente se non è vuota
                if (!concatenazione.empty()) {
                    paroleInput.push_back(concatenazione);
                    concatenazione.clear();
                }
                // Aggiungi la parola corrente ("on", "off" o numero)
                paroleInput.push_back(parola);
            } else {
                // Concatena la parola alla stringa
                if (!concatenazione.empty()) {
                    concatenazione += " "; // aggiungi uno spazio prima di concatenare
                }
                concatenazione += parola;
            }
        }
    }

    // Aggiungi la concatenazione finale se non è vuota, nel caso non abbia trovato on o off o un numero 
    if (!concatenazione.empty()) {
        paroleInput.push_back(concatenazione);
    }
   
    // controllo se l'utente ha inserito qualcosa 
    if(paroleInput.size() == 0){ //salta perchè non ha scritto niente
        return true;
    }
    
    if (paroleInput[0] == "0") { // esce dal ciclo
       return false;
    }

    try{

        // show
        if (paroleInput[0] == "show" && paroleInput.size() == 1) {
            
            std::cout << "\nAttualmente il sistema ha prodotto " << home.energiaProdotta() << "kW"
                      << " e ha consumato " << home.energiaConsumata() << "kW"<<std::endl;
            Ouput << "\nAttualmente il sistema ha prodotto " << home.energiaProdotta() << "kW" << " e ha consumato " << home.energiaConsumata() << "kW\n";        
            std::string m = home.show() + '\n';
            std::cout<<m;
            Ouput << m <<std::endl;
        }

        //show ${DEVICENAME}
        else if(paroleInput[0] == "show" && paroleInput.size() == 2){
            std::string messaggio = home.show(paroleInput[1]); 
            std::cout<<messaggio<<std::endl;
            Ouput << messaggio <<std::endl;
        } 
        
        //set time ${TIME}
        else if(paroleInput[0] == "set" && paroleInput[1] == "time" && home.convertToOrario(paroleInput[2])){
            std::string messaggio = home.setOrario(home.convertToOrario(paroleInput[2]));
            std::cout<<messaggio<<std::endl<<std::endl;
            Ouput << messaggio <<std::endl<<std::endl;
        }

        //set ${DEVICENAME} ${START} se riesco a convertire start in minuti entra se no vuol dire che è on o off
        else if(paroleInput[0] == "set" && paroleInput.size() == 3 && home.isOrario(paroleInput[2])){
            std::string m = home.set(paroleInput[1],home.convertToOrario(paroleInput[2]));
            std::cout<<m<<std::endl;
            Ouput << m <<std::endl;
        }

        // set ${DEVICENAME} on / set ${DEVICENAME} off
        else if (paroleInput[0] == "set" && paroleInput.size() == 3){
            std::string m = home.set(paroleInput[1], paroleInput[2]);
            std::cout<<m<<std::endl;
            Ouput << m <<std::endl;
        } 

        // rm ${DEVICENAME} 
        else if (paroleInput[0] == "rm" && paroleInput.size() == 2){
            std::string m = home.rm(paroleInput[1]);
            std::cout<<m<<std::endl;
            Ouput << m <<std::endl;
        }

        //set ${DEVICENAME} ${START} [${STOP}]
        else if(paroleInput[0] == "set" && paroleInput.size() == 4){
            std::string m = home.set(paroleInput[1],home.convertToOrario(paroleInput[2]),home.convertToOrario(paroleInput[3]));
            std::cout<<m<<std::endl;
            Ouput << m <<std::endl;
        }

        // reset time
        else if(paroleInput[0] == "reset" && paroleInput[1] == "time" && paroleInput.size() == 2){
            home.resetTime();
            std::cout<<home.convertToOrarioStampa()<<std::endl;
            Ouput << home.convertToOrarioStampa() << "\n";
        }

        // reset timers 
        else if(paroleInput[0] == "reset" && paroleInput[1] == "timers" && paroleInput.size() == 2){
            std::string m = home.resetTimers();
            std::cout<<m<<std::endl;
            Ouput << m <<std::endl;
        }

        // reset all 
        else if(paroleInput[0] == "reset" && paroleInput[1] == "all" && paroleInput.size() == 2){
            home.resetTime();
            std::string messaggio = "il tempo è stato resettato \n" + home.resetTimers() +home.spegniCasa();
            std::cout<<messaggio <<std::endl;
            Ouput << messaggio <<std::endl;
        }

        else if (paroleInput[0] == "print" && paroleInput.size() == 1) {
            std::string m = home.print();
            std::cout<<m<<std::endl;
            Ouput << m <<std::endl;
        }
        // il comando non è riconosciuto
        else {
            std::cout << "Errore: comando non riconosciuto."<<std::endl;
            Ouput << "Errore: comando non riconosciuto. \n"; 
        }
        return true; 

    } catch (const std::exception& e) {
        // Gestione dell'eccezione nel main
        std::cerr << "Errore: " << e.what() << std::endl;
        Ouput << "Errore: " << e.what() << "\n";
        return true; 
    }
}

int main(){

    // file di Output
    const std::string logFileName = "log.txt";                  // file log nel quale verranno scritte tutte le varie azioni 
    std::ofstream logFile(logFileName, std::ios::out);          // creo lo stream per scrivere in output e va a sovrascrivere il contenuto esistente 

    // provo ad aprire il file di testo appena creato 
    if (!logFile) {
        std::cerr << "Errore nell'apertura del file di log: " << logFileName << std::endl;
        return 1;
    }

    // lettura da un file input dove sono già scritte le funzioni da eseguire, DA COMMENTARE ALLA FINE 
    // std::ifstream inputFile("../input.txt"); // Apri il file in lettura
    // if (!inputFile.is_open()) {
    //     std::cerr << "Errore: Impossibile aprire il file input.txt" << std::endl;
    //     return 1; // se il file non può essere aperto esci
    // }

    //casa.print();
    std::string input; 
    bool continua = true; 

    // faccio inserire il consumo al utentene
    double consumoMax = 3.5;                              // consumo massimo
    std::string messaggioUtente;                        // messaggio del Utente

    while (true) {
        std::cout << "Inserisci il consumo massimo o premi Invio se vuoi che il consumo sia impostato in automatico: ";
        logFile << "Inserisci il consumo massimo o premi Invio se vuoi che il consumo sia impostato in automatico: ";
        std::getline(std::cin, messaggioUtente);

        // Controlla se l'input è vuoto
        if (messaggioUtente.empty()) {
            std::cout << "Valore del consumo impostato in automatico." << std::endl;
            logFile << "Valore del consumo impostato in automatico." << std::endl;
            break;
        }

        // provo a convertire l'input in un valore double
        try {
            consumoMax = std::stod(messaggioUtente);
        } catch (const std::invalid_argument& e) {
            std::cout << "Input non valido. Per favore inserisci un numero." << std::endl;
            logFile << "Input non valido. Per favore inserisci un numero." << std::endl;
            continue;
        }

        // Controlla se il consumo è maggiore di 0
        if (consumoMax <= 0) {
            std::cout << "Errore: il consumo massimo deve essere maggiore di 0." << std::endl;
            logFile << "Errore: il consumo massimo deve essere maggiore di 0." << std::endl;
        } else {
            break;
        }
    }

    // creo l'oggetto casa e gli passo il parametro del consumo massimo
    std::cout << "Il consumo massimo è stato impostato a " << consumoMax << " kW." << std::endl;
    logFile << "Il consumo massimo è stato impostato a " << consumoMax << " kW." << std::endl;
    Casa casa(consumoMax);                          

    std::cout << "Tutti i dispositivi sono pronti all' uso. \n\n";
    logFile << "Tutti i dispositivi sono pronti all' uso. \n\n";

    //ciclo while per permettere l'esecuzione dei comandi
    while(continua){
        std::cout << "Inserisci comando \n"; 
        logFile << "Inserisci comando \n"; 

        // SE USO un file di lettura di input 
        // if (!std::getline(inputFile, input)) { // Legge una riga dal file
        //     std::cout << "Fine del file raggiunta." << std::endl;
        //     logFile << "Fine del file raggiunta.\n";
        //     break; // Esce dal ciclo se non ci sono più righe da leggere
        // }

        // SE USO l'input da tastiera
        std::getline(std::cin, input);                  // legge la sgringa di Input
        
        if(casa.getTime()==1439){
            std::cout << "fine giornata.\n";
            break;
        }
        
        //logFile << casa.convertToOrarioStampa() << "\n";
        continua = gestisciComando(input, casa, logFile);        // gli passo la stringa di input e la reference di casa su cui operare 
       

    }
    // chiudo il file di Streaming 
    logFile.close();
    return 0;
}



