//Giovanni Pavan 2101742
#ifndef MANUALE_H
#define MANUALE_H
#include "Elettrodomestico.h"
#include <utility>
#include <vector>

class Manuale : public Elettrodomestico{
private:
    int start;  // orario in minuti di inizio
    int stop;   // orario in minuti di fine 
    // vettore di pair<int, int>, ogni pair rappresenta un orario di start e stop di un timer 
    std::vector<std::pair<int, int>> timerDispositivo;

public:
    class Invalid{};
    // costruttore 
    Manuale(int id, std::string nome, double consumoProduzione, int priority);
    
    // getter per ottenere l'inizio e la fine del timer
    virtual int getStop() const override{return stop;};
    virtual int getStart() const override{return start;};

    virtual void set(int start, int stop = -1) override;                                // setter per impostare l'inizio e la fine del timer
    virtual void setStatoDispositivo(std::string nome, int orario = -1) override;       // setter per impostare lo stato del dispositivo
    void impostaOrarioInizio(int orario);                                               // setter per impostare l'orario di start del dispositivo             
    int numeroTimer() const {return timerDispositivo.size();};                          // getter per ottenere il numero di timer
    void eliminaTimer();                                                                // funzione per eliminare tutti i timer
    virtual void riceviOrario(int orario) override;                                     // overide, comunicare l'orario al dispositivo
};

#endif