//Alessandro Michelazzo 2111551
#include <iostream>
#include "../include/Elettrodomestico.h"
#include <string>
#include <algorithm>


// costruttore 
Elettrodomestico::Elettrodomestico(int id, std::string nome, double consumoProduzione, int priority){
    this->id = id;
    this->nome = nome;
    this->consumoProduzione = consumoProduzione;
    this->priority = priority;
    this->stato = false;
    this->consumoTotale = 0;
}

void Elettrodomestico::setConsumoDispositivo(int orario){
    // calcolo il consumo al minuto 
    double consumoAlminuto = getConsumoProduzione() / 60;

    if(orario == -1){
        this->consumoTotale += consumoAlminuto;
    }
    else{
        int ora = orario % 60;
        int minuti = orario - (ora * 60);
        this->consumoTotale += ora* getConsumoProduzione() + minuti * consumoAlminuto;
    }
    
}

// ritorna il consumo solo se il dispositivo è acceso 
double Elettrodomestico::getConsumoProduzione() const{
    if(this->stato){
        return this->consumoProduzione;
    }
    else{
        return 0;
    }
}
