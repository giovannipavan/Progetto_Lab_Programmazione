//Giorgio Fanin 2111552
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
// controlla se l'utente ha inserito un orario 
bool Casa::isOrario(std::string str) const{
    return (str.find(':') != std::string::npos); //ritorna true se trova :
}

// converti ora in stringa in minuti e se non si riesce a leggere lancia un' eccezzione
// [hh:mm] -> minuti da inizio giornata, quindi se e str è "14:30", la funzione restituirà 870 (14 * 60 + 30)
int Casa::convertToOrario(std::string str) const{

    //ore
    int i=0;
    std::string ora = "";
    while(str[i] != ':'){
        ora+=str[i];
        i++;
    }

    //minuti

    int k = i+1;
    std::string minuti = "";
    while(k < str.size()){
        minuti+=str[k];
        k++;
    }

    if(std::stoi(ora) < 0 || std::stoi(ora) >= 24 || std::stoi(minuti) < 0 || std::stoi(minuti) >= 60){
        throw OrarioNonRiconosciuto();
    }

    return (std::stoi(ora)*60)+(std::stoi(minuti));
}

// converte l'orario in formato stampabile stringa 
std::string Casa::convertToOrarioStampa(int minutes) const{
    
    bool checkParameter = false;
    //std::cout<< "controllo minuti "<<minutes;
    if(minutes == -1){
        minutes = getTime();
        checkParameter = true;
    }

    // calcola ore e minuti
    int hours = minutes / 60;
    int minuti = minutes % 60;
    
    std::string orario = "" ;
    // stringa in formato [hh:mm]
    orario += (hours < 10) ? "0" + std::to_string(hours) : std::to_string(hours);
    orario += ":";
    orario += (minuti < 10) ? "0" + std::to_string(minuti) : std::to_string(minuti);
    
    if(checkParameter){
        return "[" + orario + "] L’orario attuale è "+orario;
    }
    else{
        return "[" + orario + "]";
    }
    
}

// per " avanzare temporalmente "
std::string Casa::setOrario(int orarioFinale) {

    std::string messaggio = ""; 
    //se l' orario inserito è minore del precedente lacio errore
    if(getTime() > orarioFinale){
        throw OrarioNonConcesso();
    }

    // passo l'orario ad ogni dispositivo in modo da impostare lo start corretto 
    for (Elettrodomestico* device : listaElettrodomestici) {
        device -> riceviOrario(orario);
    }

    messaggio += printAzioni(getTime(),orarioFinale);
    this->orario = orarioFinale;
    messaggio += convertToOrarioStampa();

    return messaggio;

}

// lista di azioni successe dal ultimo orario chiamato 
std::string Casa::printAzioni(int orarioPartenza,int orarioFinale){

    std::string messaggio = ""; 
    double energiaConsumataInstantanea = 0;
    orarioPartenza++;
    
    while (orarioPartenza <= orarioFinale)
    {   
        energiaConsumataInstantanea = 0;

        //printa tutte le azioni per ogni dispositivo
        for(Elettrodomestico* device : listaElettrodomestici){

            // CALCOLO CONSUMO ISTANTANEO dei vari dispositivi
            if(device->getStato()){
                device->setConsumoDispositivo();
                energiaConsumataInstantanea += device->getConsumoProduzione();
            }

            // se l'orario di start è uguale all'orario di partenza allora il dispositivo si accende
            if(device->getStart() == orarioPartenza){

                // controllo se il dispositivo è spento 
                if (!device->getStato()) {
                    messaggio += convertToOrarioStampa(orarioPartenza) + " il dispositivo '" +device->getNome()+ "' si è acceso\n";
                    device->setStatoDispositivo("on", orarioPartenza);
                }
            }

            // se l'orario di stop è uguale all'orario di partenza allora il dispositivo si spegne
            else if(device->getStop() == orarioPartenza){

                // controllo se il dispositivo è acceso
                if (device->getStato()) {
                    messaggio += convertToOrarioStampa(orarioPartenza)+" il dispositivo '" +device->getNome()+ "' si è spento\n";
                device->setStatoDispositivo("off");
                }
            }

            // AGGIUNTA DISPOSITIVO ALLA LISTA DEI DISPOSITIVI ACCESI
            if ( device->getStato()) {
                if (deviceAccesi.empty()) {
                    deviceAccesi.push_back(device);
                }   
                for(int i = 0; i < deviceAccesi.size(); i++){
                    if(deviceAccesi[i] == device){
                        break;
                    }
                    if( i == deviceAccesi.size() - 1){ 
                        deviceAccesi.push_back(device);
                    }
                }
            }
            else {
                for(int i = 0; i < deviceAccesi.size(); i++){
                    if(deviceAccesi[i] == device){
                        deviceAccesi.erase(deviceAccesi.begin() + i);
                    }
                }
            }
 
        }

        while(std::abs(energiaConsumataInstantanea) > (getMaxAssorbimento() + energiaProdottaInstantanea())){//da finire
            
            
            if (!deviceAccesi.empty()) {
                
                //-------- da commentare se si vuole la politica: si spegne ultimo dispositivo acceso se il consumo supera il limite massimo
                std::sort(deviceAccesi.begin(), deviceAccesi.end(), [](Elettrodomestico* a, Elettrodomestico* b) {
                        return a->getPriority() < b->getPriority();
                });
                
                //--------

                //trova l' ultimo dispositivo acceso e lo elimina

                Elettrodomestico* dispositivo = deviceAccesi[deviceAccesi.size()-1];
                deviceAccesi.erase(deviceAccesi.begin() + (deviceAccesi.size()-1)); 
                set(dispositivo->getNome(), "off");
                messaggio += convertToOrarioStampa(orarioPartenza) + " Il sistema sta consumando: " +
                std::to_string(energiaConsumataInstantanea) + " kW, rispetto ai  " +
                std::to_string(getMaxAssorbimento() + energiaProdottaInstantanea()) + " kW disponibili, procedo a  rimuovo " 
                + dispositivo->getNome() + "\n";
            }
            // ricalcolo il consumo istantaneo
            energiaConsumataInstantanea = 0;
            for (Elettrodomestico* device : listaElettrodomestici) {
                if (device->getStato()) {
                    energiaConsumataInstantanea += device->getConsumoProduzione();
                }
            }
        }

        orarioPartenza++;
    }
    return messaggio;
}

// rm ${DEVICENAME}
std::string Casa::rm(std::string nome){
    Elettrodomestico* device = checkDispositivo(nome);
    // prova ad eseguire un down cast a Manuale, se non si riesce sarà nullptr, quindi è un dispositivo di tipo CP
    Manuale* manualeDevice = dynamic_cast<Manuale*>(device);
    if (manualeDevice) {
        if (manualeDevice->numeroTimer() > 0) {
            manualeDevice->eliminaTimer();
            return "Rimosso il timer dal dispositivo '"+device->getNome()+"' \n";
        }
        else {
            return "Il dispositivo '"+device->getNome()+"' non ha timer impostati \n";
        }
    }
    else{
        throw AzioneDispositivoNonConcessa();
    }
}

// resetta l'orario
void Casa::resetTime(){
    this->orario = 0;
    // spengo ogni dispositivo
    spegniCasa();
}

// resetta tutti i timer
std::string Casa::resetTimers(){
    std::string messaggio = "";
    for(Elettrodomestico* device : listaElettrodomestici){
        // prova ad eseguire un down cast a Manuale, se riesce esegue il comando resetta il timer, altrimenti manuale è nullptr
        Manuale* manualeDevice = dynamic_cast<Manuale*>(device);
        if (manualeDevice) {
            if (manualeDevice->numeroTimer() > 0) {
                manualeDevice->eliminaTimer();
                messaggio += "Timer eliminati per il dispositivo '"+device->getNome()+"' \n";
            }
        }
    }
    return messaggio;
}

// spegne tutti i dispositivi accesi 
std::string Casa::spegniCasa(){
    std::string messaggio = "";
    for(Elettrodomestico* device : listaElettrodomestici){
        if(device->getStato()){
            device->setStatoDispositivo("off");
        }
    }
    messaggio += "Tutti i dispositivi sono stati spenti";
    return messaggio;
}

Casa::~Casa() {
    listaElettrodomestici.clear();
}

