//Giovanni Pavan 2101742
#include "../include/Casa.h"
#include "../include/Manuale.h"
#include "../include/CP.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <vector>
#include <string>
#include <exception>


// costruttore 
Casa::Casa(double maxConsumo): MAX_ASSORBIMENTO(maxConsumo){
    orario = 0;                                                     // imposto l'ora a 0
    insertElettrodomestici("../Lista Elettrodomestici.txt");        // inserisco gli elettrodomestici
}

// inserire gli elettrodomestici nella lista 
void Casa::insertElettrodomestici(const std::string& filePath){
    
    // crea uno stream per la lettura del file di input 
    std::ifstream file(filePath);                               
    std::string line;
    int idCounter = 1;

    if (!file.is_open()) {
        std::cerr << "Errore: non si trova il file " << filePath << "\n";
        return;
    }

    // Salta la prima riga (intestazione)
    std::getline(file, line);

    // Leggi tutte le righe del file
    while (std::getline(file, line)) {
        // Usa stringstream per dividere la riga in campi separati da tabulazioni
        std::istringstream ss(line);
        std::string nome, tipo, durataStr, consumoStr, prioritaStr;
        
        // Legge ogni campo separato da virgola
        std::getline(ss, nome, ',');          // Nome dispositivo
        std::getline(ss, tipo, ',');          // Tipo (Manuale/CP)
        std::getline(ss, durataStr, ',');     // Durata in minuti (se applicabile)
        std::getline(ss, consumoStr, ',');    // Consumo
        std::getline(ss, prioritaStr, ',');   // Priorità


        // Gestione della durata, consumo e priorità
        int durata = 0;
        if (tipo == "CP") {
            durata = std::stoi(durataStr);  // Converte la durata solo se è di tipo "CP"
        }
        
        double consumo = 0.0;
        try { 
            consumo = std::stod(consumoStr);  // Converte consumo in double (gestisce anche il segno)
        } catch (const std::invalid_argument& e) {
            std::cerr << "Errore nella conversione del consumo per " << nome << ": " << e.what() << std::endl;
            continue; // Passa alla prossima riga in caso di errore
        }

        int priorita = 0;
        try {
            priorita = std::stoi(prioritaStr);  // Converte la priorità
        } catch (const std::invalid_argument& e) {
            std::cerr << "Errore nella conversione della priorità per " << nome << ": " << e.what() << std::endl;
            continue; // Passa alla prossima riga in caso di errore
        }

        // Aggiungi l'elettrodomestico alla lista
        if (tipo == "Manuale") {
            listaElettrodomestici.push_back(new Manuale(idCounter++, nome, consumo, priorita));
        } else if (tipo == "CP") {  
            listaElettrodomestici.push_back(new CP(idCounter++, nome, consumo, priorita, durata));
        } else {
            std::cerr << "Tipo sconosciuto per il dispositivo " << nome << ": " << tipo << std::endl;
        }
    }

    file.close();
}

// stampa la lista di elettrodomestici inseriti dal utente 
std::string Casa::print() const {
    std::string messaggio = "";
    for (Elettrodomestico* elettrodomestico : listaElettrodomestici) {
        // std::cout << "ID: " << elettrodomestico->getId() << ", "
        //           << "Nome: " << elettrodomestico->getNome() << ", "
        //           << "Consumo: " << elettrodomestico->getConsumoProduzione() << ", "
        //           << "Priorità: " << elettrodomestico->getPriority() << ", "
        //           << "Stato: " << (elettrodomestico->getStato() ? "acceso" : "spento");
        messaggio += "ID: " + std::to_string(elettrodomestico->getId()) + ", "
          + "Nome: " + elettrodomestico->getNome() + ", "
          + "Consumo: " + std::to_string(elettrodomestico->getConsumoProduzione()) + ", "
          + "Priorità: " + std::to_string(elettrodomestico->getPriority()) + ", "
          + "Stato: " + (elettrodomestico->getStato() ? "acceso" : "spento") + "\n";
    }

    return messaggio;
}

// ritorna la somma delle energie prodotte Istantanee
double Casa::energiaProdottaInstantanea() const{

    double eProdotta = 0; 
    for (Elettrodomestico* elettrodomestico : listaElettrodomestici) {
        if (elettrodomestico->getConsumoProduzione() > 0 && elettrodomestico->getStato()) {
            eProdotta += elettrodomestico->getConsumoProduzione();
        }
    }
    return eProdotta; 
}

// ritorna la somma delle energie consumate da inizio giornata
double Casa::energiaProdotta() const{

    double eProdotta = 0; 
    for (Elettrodomestico* elettrodomestico : listaElettrodomestici) {
        if (elettrodomestico->getConsumoProduzione() > 0) {
            eProdotta += elettrodomestico->getConsumoTotale();
        }
    }
    return eProdotta; 
}

// ritorna la somma delle energie consumate da inizio giornata
double Casa::energiaConsumata() const{

    double eConsumata = 0; 
    for (Elettrodomestico* elettrodomestico : listaElettrodomestici) {
        if (elettrodomestico->getConsumoProduzione() < 0) {
            eConsumata += elettrodomestico->getConsumoTotale();
        }
    }
    return eConsumata;
}

// ritorna il consumo di un dispositivo da inizio giornata
std::string Casa::show(std::string nome) const{
        
    Elettrodomestico* device = checkDispositivo(nome);
    return ("il dispositivo "+nome+" ha attualmente consumato "+std::to_string(device->getConsumoTotale())+"kWh");
}

// stampa i vari consumi dei vari apparati
std::string Casa::show() const {
    std::string messaggio = "";
    for (Elettrodomestico* elettrodomestico : listaElettrodomestici) {
        messaggio += "Il dispositivo " + elettrodomestico->getNome() + " ha consumato: " 
                    + std::to_string(elettrodomestico->getConsumoTotale()) + "kW\n";
    }
    return messaggio;
}

// controlla se il dispositivo è presente nella lista dei dispositivi 
Elettrodomestico* Casa::checkDispositivo(std::string nome ) const {

    for(Elettrodomestico * elettrodomestico : listaElettrodomestici){
        if(elettrodomestico->getNome() == nome){
            return elettrodomestico;
        }
    }

    // se non trovo un dispositivo lancio una eccezione 
    throw DispositivoAssente(nome);

}

// Set device on/off
std::string Casa::set(std::string nome, std::string stato) {
    Elettrodomestico* device = checkDispositivo(nome);

    if (stato == "on"){

        // controllo se il dispositivo è già acceso
        if (device->getStato()) {
            return ("Il dispositivo '"+device->getNome()+"' è già acceso \n");
        }   

        device->setStatoDispositivo("on", getTime());
        return ("Il dispositivo '"+device->getNome()+"' si è acceso \n");
        
    } else if (stato == "off"){

        // controllo se il dispositivo è già spento
        if (!device->getStato()) {
            return ("Il dispositivo '"+device->getNome()+"' è già spento \n");
        }
        device->setStatoDispositivo("off");
        
        // in caso il dispositivo avesse un timer impostato vado al prossimo timer
        // prova ad eseguire un down cast a Manuale, se riesce esegue il comando avanzo al prossimo timer
        Manuale* manualeDevice = dynamic_cast<Manuale*>(device);
        if (manualeDevice) {
           manualeDevice->impostaOrarioInizio(getTime());
        }

        return ("Il dispositivo '"+device->getNome()+"' si è spento \n");

    } else {
        throw ComandoNonRiconosciuto();
    }
}

// set ${DEVICENAME} ${START}
std::string Casa::set(std::string nome,int start) const{
    if (start < 0 || start >= 1440) {
        throw OrarioNonRiconosciuto();
    }
    if (start < getTime()) {
        throw OrarioNonConcesso();
    }
    Elettrodomestico* device = checkDispositivo(nome);
    device->set(start);    
    return "Il dispositivo '"+device->getNome()+"' si accenderà alle "+convertToOrarioStampa(start)+"\n";
}

// set ${DEVICENAME} ${START} ${STOP}
std::string Casa::set(std::string nome,int start, int stop) const{

    std::string messaggio = "";

    //lancio errore
    if(start >= stop || start < orario){
        throw TimerError();
    }

    Elettrodomestico* device = checkDispositivo(nome);

    // prova ad eseguire un down cast a Manuale, se riesce vuol dire che posso impostare un timer, altrimenti manuale è nullptr
    Manuale* manualeDevice = dynamic_cast<Manuale*>(device);
    if (manualeDevice) {
        manualeDevice -> set(start, stop);
        messaggio += "Impostato un timer per il dispositivo '"+device->getNome()+"' dalle "+convertToOrarioStampa(start)+" alle "+convertToOrarioStampa(stop)+"\n";
    } else {
        throw AzioneDispositivoNonConcessa();
    }

    return messaggio;
}
