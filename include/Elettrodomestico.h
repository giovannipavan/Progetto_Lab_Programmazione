//Alessandro Michelazzo 2111551
#ifndef ELETTRODOMESTICO_H
#define ELETTRODOMESTICO_H
#include <string>
#include <vector>


class Elettrodomestico
{
private:

    int id;                         // ID dispositivo
    std::string nome;               // Nome del dispositivo
    double consumoProduzione;       // consumo del dispositico istanteo
    int priority;                   // priorità 

protected:
    
    double consumoTotale;           // consumo del dispositivo da inizio giornata 
    bool stato;                     // stato: acceso o spento
    std::vector<int> accensioni;    // tiene conto delle future accensioni di ogni dispositivo

public:

    class Invalid{};
    // costruttore 
    Elettrodomestico(int id, std::string nome, double consumoProduzione, int priority);

    //getter
    int getId() const {return id;};                             // ottieni l'id del dispositivo
    std::string getNome() const { return nome; };               // ottieni il nome del dispositivo
    double getConsumoProduzione() const;                        // ottieni il consumo istantaneo del dispositivo
    double getConsumoTotale()const { return consumoTotale; };   // ottieni il consumo totale del dispositivo
    int getPriority() const {return priority;}                  // ottieni la priorità del dispositivo
    bool getStato()const {return stato;};                       // ottieni lo stato del dispositivo

    //setter
    void setConsumoDispositivo(int orario = -1);                                // calcola il consumo del dispositivo
    // obligatorio per la classe derivata da specificare il comportamento
    virtual void setStatoDispositivo(std::string stato,int orario = -1) = 0;    // imposta lo stato del dispositivo
    virtual void set(int start, int stop = -1) = 0;                             // imposta l'orario di accensione e/o spegnimento del dispositivo
    virtual int getStart() const = 0;                                           // ottieni l'orario di accensione del dispositivo
    virtual int getStop() const = 0;                                            // ottieni l'orario di spegnimento del dispositivo

    // funzione per passare l'orario al dispiositivo così da impostare un orario sdi accensione successivo
    virtual void riceviOrario(int orario) = 0;

    // distruttore virtuale
    virtual ~Elettrodomestico() {}
};


#endif
