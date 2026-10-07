//Giovanni Pavan 2101742
#include <iostream>
#include "../include/Manuale.h"
#include <string>
#include <algorithm>
#include "Eccezioni.h"

//imposto il construttore manuale e metto il spento manualmente false
Manuale::Manuale(int id,std::string nome,double consumoProduzione,int priority): Elettrodomestico::Elettrodomestico(id,nome,consumoProduzione,priority){
    start = -1;
    stop = -1;
}


// Implementazione del metodo set()
void Manuale::set(int start, int stop) {
    
    // se il campo stop è diverso da -1 vuol dire che è stato inserito un timer 
    // controllo se riesco ad inserirlo nel vettore dei timer
    if(stop != -1){
        // Se il vettore è vuoto, aggiungi direttamente il nuovo timer
        if (timerDispositivo.empty()) {
            timerDispositivo.push_back(std::make_pair(start, stop));
            return;
        }

        // devo controllare che non ci sia una sovrapposizione di timer
        for (const std::pair<int, int>& timer : timerDispositivo) {
            if ((start >= timer.first && start < timer.second) || 
                (stop > timer.first && stop <= timer.second) || 
                (start <= timer.first && stop >= timer.second)) {
                // Se gli orari si intrecciano, lancia un'eccezione
                throw TimerSovrappostoException();
            }
        }

        // aggiungo il nuovo timer
        timerDispositivo.push_back(std::make_pair(start,stop));

        // uso della Lambda function per ordinare il vettore in base all'orario di inizio
        std::sort(timerDispositivo.begin(), timerDispositivo.end(), [](const std::pair<int, int>& primo, const std::pair<int, int>& secondo) {
            return primo.first < secondo.first;
        });

    }

    // se il campo stop è uguale a -1 vuol dire che è stato inserito solo un orario di start
    // quindi provo ad aggiungere l'orario di start al vettore delle accensioni
    else{
        // controlla che start non sia interno a un timer esistente
        for (const std::pair<int, int>& timer : timerDispositivo) {
            if (start >= timer.first && start < timer.second) {
                throw OrarioInizioNonValido();
            }
        }
        accensioni.push_back(start);
        // ordino il vettore delle accensioni
        std::sort(accensioni.begin(), accensioni.end());
    }
}

// imposta lo stato del dispositivo
void Manuale::setStatoDispositivo(std::string stato,int orario){
    // se deve essere acceso allora start diventa orario mentre se off faccio il consumo con orario e elettrodomestico.start
    // se non ho un orario 
    if(orario == -1){
        if(stato == "on"){
            this->stato = true;
        }
        else if(stato == "off"){
            this->stato = false;
            setConsumoDispositivo(getStop());
            impostaOrarioInizio(stop);
        }
        else{
            throw Elettrodomestico::Invalid{};
        }
    }
    // entro se ho un orario 
    else{
        if(stato == "on"){
            this->stato = true;
        }
        else if(stato == "off"){
            this->stato = false;
            setConsumoDispositivo(orario);
        }
        else{
            throw Elettrodomestico::Invalid{};
        }
    }
}

// imposta l'orario di inizio del dispositivo dato un orario, 
// va a interrogare sia il vettore delle accensioni che il vettore dei timer
void Manuale::impostaOrarioInizio(int orarioCorrente) {
    // controlla se il vettore dei timer è vuoto
    if (timerDispositivo.empty()) {
        // se non ci sono accensioni future, imposta start e stop a -1
        if (accensioni.empty()) {
            start = -1;
            stop = -1;
            return;
        }
        // altrimenti, imposta start al prossimo orario di accensione corretto
        for (int accensione : accensioni) {
            if (accensione > orarioCorrente) {
                start = accensione;
            }
        }
    }

    // se ho dei timer devo impostare correttamente il prossimo orario di accensione 
    // controllando anche le accensioni future

    // imposto a -1 per vedere se trovo un orario di accensione successivo
    int accensioneSeguente = -1;
    // trovo la ipotetica prossima accensione
    if (!accensioni.empty()){
        for (int accensione : accensioni) {
            if (accensione > orarioCorrente) {
                accensioneSeguente = accensione;
            }
        }
    } 
    
    // trova la coppia di orari successiva all'orario corrente
    for (const std::pair<int, int>& timer : timerDispositivo) {
        // controllo che l'orario del timer sia maggiore dell'orario corrente
        if (timer.first > orarioCorrente) {
            // se non ci sono accensioni future e il primo timer disponibile è minore del orario delle accensioni
            if(timer.first < accensioneSeguente || accensioneSeguente == -1){
                start = timer.first;
                stop = timer.second;
                return;
            }
            start = accensioneSeguente;
            stop = timer.second;
            return;
        }
    }

    // altrimenti, imposta start al prossimo orario di accensione e rimarrà acceso
    for (int accensione : accensioni) {
        if (accensione > orarioCorrente) {
            start = accensione;
            return;
        }
        else{
            start = -1;
            stop = -1;
        }
    }
}

// elimina tutti i timer
void Manuale::eliminaTimer() {
    timerDispositivo.clear();
    start = -1;
    stop = -1;
}

// gli passo l'orario di inizio, in modo che possa impostare start e stop più prossimi a quel orario 
void Manuale::riceviOrario(int orario){
    impostaOrarioInizio(orario);
}