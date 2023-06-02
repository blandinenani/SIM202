

#ifndef zone_message_hpp
#define zone_message_hpp

#include <stdio.h>
#include <string>
#include <list>
#include <iostream>
#include<vector>
#include <sstream>
#include <cstdlib>
#include<map>
#include <fstream>
#include <stdlib.h>     
#include <time.h>       
using namespace std;

//---------------------------------------------------------------------
//                         classe client
//---------------------------------------------------------------------
class Client{
public:
    string id_cl;
    double solde;
    int num_mes;
    Client (string, double);
    void print () const;

};
//---------------------------------------------------------------------


//---------------------------------------------------------------------
//                         classe action
//---------------------------------------------------------------------
class action{
public:
    string nom;
    int cours_action;
    vector<int> suivi_cours_action;
    action();
    action (string, int); 
    void print () const;
};

//---------------------------------------------------------------------
//                         classe ordre
//---------------------------------------------------------------------

class Ordre{
public:
    bool achat; //True si l'ordre est un achat, False si vente
    string action; // nom de l'action
    int quantite;
    int prix;
    string id_cl; // id du client concerné

    Ordre(bool,string,int,int);
    string encode() const; //fonction qui encode l'ordre sous la forme achat,action,quantité,prix
    void decode(const string&); //fonction qui decode un ordre 
    void print(); //affichage de l'ordre
};

void insere(Ordre);

//---------------------------------------------------------------------

//---------------------------------------------------------------------
//                         classe message
//---------------------------------------------------------------------
// classe mère pour les messages
class Message{
public:
    // identifiant client
    Client* client;
    // numéro de message unique du client
    int num_message;
    // liste des ordres codés
    list <Ordre> ordres;
    Message (Client*, int, list<Ordre>);
    Message();
};

//---------------------------------------------------------------------
// classe des messages envoyés par le client
class Message_client: public Message {
public:
    Message_client();
    Message_client(Client*, int, list<Ordre>);
    // fonction pour encodé les messages envoyés par le client
    string encode_mes_client();
    // fonction pour décodé les messages reçus par le client
    void  decode_mes_client(const string&, map<string,Client>&);
    // fonction création fichier message
    // fonction lire fichier message
    void print ();
    void envoie_mes_client();
};

class Message_serveur: public Message {
public:
    Message_serveur();
    Message_serveur(Client*, int, list<Ordre>);
    

    string encode_mes_serveur();
    void envoie_mes_serveur();

    string encode_acr_serveur();
    void envoie_acr_serveur();

    void  decode_mes_client(const string&, map<string,Client>&);

    void print();


};

//---------------------------------------------------------------------

#endif