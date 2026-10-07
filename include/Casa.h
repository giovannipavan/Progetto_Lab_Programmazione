//Giovanni Pavan 2101742
#ifndef CASA_H
#define CASA_H

#include <vector>
#include "Elettrodomestico.h"
#include "Eccezioni.h"
#include <iostream>
#include <memory>
#include <fstream>
#include <sstream>
#include <exception>


class Casa{
private:

    int orario;                                                                     // orario corrente di gestione IN MINUTI  
    const double MAX_ASSORBIMENTO ;                                              // ultimo dispositivo inserito 
    std::vector<Elettrodomestico *> listaElettrodomestici;                          // lista degli elettrodomestici
    std::vector<Elettrodomestico *> deviceAccesi;
    void insertElettrodomestici( const std::string& filePath );
    Elettrodomestico* checkDispositivo(std::string nome ) const ;

public:

    Casa(double maxConsumo = 3.5);                                                                     // costruttore 
    const double& getMaxAssorbimento() const  {return MAX_ASSORBIMENTO;};       // valore del massimo assorbimento raggiungibile
    std::string print() const;                                                         // funzione per stampare tutti i membri contenuti nella lista Elettrodomestici
    const int& getTime() const {return orario; };                               // ritorna l'orario corrente della gestione del orario
    double energiaProdotta() const;                                             // ritorna la somma delle energie Prodotte da inizio giornata 
    double energiaProdottaInstantanea() const;
    double energiaConsumata() const;                                            // ritrona la somma delle energie Consumate da inizio giornata 
    std::string show(std::string nome) const;                                        // mostra il nome e il consumo di un dispositivo da inizio giornata 
    std::string show() const;                                                   // stamapa in output i vari consumi dei vai vari apparati 

    std::string set(std::string nome, std::string stato);                              // set ${DEVICENAME} on/off       
    std::string set(std::string nome, int start) const;                                // set ${DEVICENAME} ${START}
    std::string set(std::string nome, int start, int stop) const;                      // set ${DEVICENAME} ${START} ${STOP}
    
    void resetTime();
    std::string resetTimers();           
    std::string rm(std::string nome);                                           // rm ${DEVICENAME}
    std::string setOrario(int orarioFinale);                                    // set time ${TIME} 
    std::string spegniCasa();                                                   // spegne tutti i dispositivi
    std::string printAzioni(int orarioPartenza,int orarioFinale);               // stampa la lista delle azioni successe dalla precedente invocazione di set 
    std::string convertToOrarioStampa(int minutes = -1) const;                  // converte da minuti a [hh:mm] per la stampa
    bool isOrario(std::string str) const;                                       // guarda se si può converire la stringa ore e minuti in minuti
    int convertToOrario(std::string str) const;                                 // converte una string di formato [hh:mm] in minuti
    ~Casa(); 

};


#endif


