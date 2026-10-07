//Giorgio Fanin 2111552
#ifndef CP_H
#define CP_H
#include "Elettrodomestico.h"
#include <string>
class CP : public Elettrodomestico
{
private:

    int minutaggio;
    int start;
    int stop;

public:

    class Invalid{};

    // costruttore 
    CP(int id,std::string nome,double consumoProduzione,int priority, int minutaggio);
    
    // getter per ottenere l'inizio e la fine del timer
    virtual int getStart() const override{return start;};
    virtual int getStop() const override {return stop;};

    virtual void set(int start, int stop = -1) override;                            // setter per impostare l'inizio e la fine del timer
    virtual void setStatoDispositivo(std::string nome, int orario = -1) override;   // setter per impostare lo stato del dispositivo
    virtual void riceviOrario(int orario) override;                                 // overide, comunicare l'orario al dispositivo   
    void prossimaAccensione(int orario);                                            // funzione per impostare la prossima accensione    
};


#endif