//Giorgio Fanin 2111552
#include <iostream>
#include "../include/CP.h"
#include <string>
#include "Eccezioni.h"
#include <algorithm>



//imposto il construttore manuale e metto il spento manualmente false
CP::CP(int id,std::string nome,double consumoProduzione,int priority, int minutaggio): Elettrodomestico::Elettrodomestico(id,nome,consumoProduzione,priority){
    this->minutaggio = minutaggio;
    start = -1;
    stop = -1;
}


// Implementazione del metodo set()
void CP::set(int start, int stop) {
    // se come secondo parametro ho un numero diverso da -1, non posso eseguire il comando per definizione 
    // dato che i dispositivi hanno un ciclo già prefissato 
    if(stop != -1){
        throw AzioneDispositivoNonConcessa();
    }
    // inserisco la prossima accensione del dispositivo, controllando che non ci siano sovrapposizioni
    if (!accensioni.empty()) {
        for (int accensione : accensioni) {
            if (start < accensione + minutaggio && start >= accensione) {
                throw OrarioInizioNonValido();
            }
        }
    }
    accensioni.push_back(start);
    // ordino il vettore delle accensioni
    std::sort(accensioni.begin(), accensioni.end());
}

// funzione per impostare la prossima accensione del dispositivo
// oer farlo prendo l'orario attuale e controllo se c'è una accensione successiva nel vettore delle accensioni
void CP::prossimaAccensione(int orario) {
    if (getStato()) {
        return;
    }

    // Se l'orario non è specificato, imposta la prossima accensione in base a "start"
    if (orario == -1) {
        orario = start;
    }

    if (!accensioni.empty()) {
        // Cerca la prossima accensione nel vettore delle accensioni
        for (int accensione : accensioni) {
            if (accensione > orario) {
                start = accensione;
                stop = start + minutaggio;
                return;
            }
        }
    }

    // Se non trova accensioni future, imposta start e stop a -1
    start = -1;
    stop = -1;
}

// imposta lo stato del dipositivo 
void CP::setStatoDispositivo(std::string stato, int orario){
    
    if ((stato == "on" && getStato()) || (stato == "off" && !getStato())) {
        return;  // se un dispositivo è già acceso non imposto l'orario di accensione dato che lo è già non fare nulla
    }
    
    // se non ho un orario specificato, imposto la prossima accensione e lo spengo
    if(orario == -1){
        if(stato == "off"){
            this->stato = false;
            setConsumoDispositivo(getStop());
            prossimaAccensione(orario);
        }
        else{
            throw AzioneDispositivoNonConcessa();
        }
    }
    else{
        if(stato == "on"){
            this->stato = true;
            this->stop = orario+minutaggio;
            
        }
        else if(stato == "off"){
            this->stato = false;
            setConsumoDispositivo(orario);
        }
        else{
            throw AzioneDispositivoNonConcessa();
        }
    }
}

// funzione per ricevere l'orario
void CP::riceviOrario(int orario){
    prossimaAccensione(orario);
}