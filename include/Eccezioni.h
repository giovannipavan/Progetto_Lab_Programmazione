//Giorgio Fanin 2111552
#ifndef ECCEZIONI_H
#define ECCEZIONI_H

#include <vector>
#include <iostream>
#include <memory>
#include <fstream>
#include <sstream>
#include <exception>

// Classi lanciate in caso di vari errori nel programma 

// Classe lanciata se manca il dispositivo
class DispositivoAssente : public std::exception {
private:
    std::string message;

public:

    // costruttore che mi andrà a creare il messaggio di errore
    explicit DispositivoAssente(const std::string& nome)
        : message("Dispositivo '" + nome + "' non trovato nella casa") {}

    // override del metodo what che mi so scrive il messaggio d'errore 
    // noexcept segnalo che la funzione che sto scrivendo NON produrrà possibili errori
    const char* what() const noexcept override {
        return message.c_str();
    }
};

// Se il comando scritto dal utente non esiste 
class ComandoNonRiconosciuto : public std::exception {
public:
    const char* what() const noexcept override {
        return "Comando Sconosciuto";
    }
};

// Se l'orario è inesistente 
class OrarioNonRiconosciuto : public std::exception {
public:
    const char* what() const noexcept override {
        return "Orario non riconosciuto";
    }
};

// se l'utente mette un orario già avvenuto 
class OrarioNonConcesso : public std::exception {
public:
    const char* what() const noexcept override {
        return "Orario non concesso";
    }
};

// Se si prova ad eseguire una opereazione su un dispositivo che non la supporta 
class AzioneDispositivoNonConcessa : public std::exception {
public:
    const char* what() const noexcept override {
        return "Azione del dispositivo non concessa";
    }
};

// se ci sono sovrapposizione di timer 
class TimerSovrappostoException : public std::exception {
public:
    virtual const char* what() const noexcept override {
        return "Timer sovrapposti";
    }
};

// se si prova a fare un downcast su un dispositivo che non è di tipo Manuale
class DowncastException : public std::exception {
public:
    virtual const char* what() const noexcept override {
        return "Errore nel downcast: il dispositivo non è di tipo Manuale.";
    }
};

// se si associa un timer sbagliato 
class TimerError : public std::exception {
public:
    virtual const char* what() const noexcept override {
        return "Errore nel timer"; 
    }
};

class OrarioInizioNonValido : public std::exception {
public:
    virtual const char* what() const noexcept override {
        return "Impossibile inserire l'orario di inizio dato che è già occupato";
    }
};
#endif