#include "zone_message.hpp"
#include <stdio.h>
#include <string>
#include <list>
#include <iostream>
#include<vector>
#include <sstream>
#include <cstdlib>
#include<map>
#include <fstream>
#include <time.h>
#include <cstdio>
using namespace std;


//---------------------------------------------------------------------
//                         classe client
//---------------------------------------------------------------------

Client::Client (string s, double x)
{
    id_cl = s;
    solde = x;
    num_mes = 0;
    
}

void Client::print () const 
{
    cout << "id_cl : " << id_cl << endl;
    cout << "montant du compte : " << solde << endl;
}
//---------------------------------------------------------------------


//---------------------------------------------------------------------
//                         classe action
//---------------------------------------------------------------------


action::action()
{
    nom = "";
    cours_action = 0;
}


action::action(string s, int p)
{
    nom = s;
    cours_action = p;
    suivi_cours_action.resize(0);
    suivi_cours_action.push_back(p);

}

void action::print() const 
{
    cout << "nom,cours action = " << nom+','+ to_string(cours_action) << endl;
}

  



//---------------------------------------------------------------------
//                         classe Ordre
//---------------------------------------------------------------------

Ordre::Ordre(bool A,string s, int q, int p)
{
    achat = A;
    action  = s;
    quantite = q;
    prix = p;
}

string Ordre::encode() const
{
    string ordre;
    ordre = to_string(achat) + ordre + "," + action + "," + to_string(quantite) + "," + to_string(prix);
    return ordre;
    }

void Ordre::decode(const string &Ordre)
{
    vector<string> result;
    istringstream iss(Ordre);
    string str;
    while (getline(iss,str,','))
    {
        result.push_back(str);
    }
    vector<string>::iterator ite = result.begin();
    int j = stoi(*ite);
    achat = j;
    ite++;
    action = *ite;
    ite++;
    quantite = stoi(*ite);
    ite++;
    prix = stoi(*ite);
}

void Ordre::print ()
{
    cout << (*this).encode()<<endl;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void print_actions(map<string,action>& actions)
{
    
    for(map<string,action>::iterator it=actions.begin();it!=actions.end();++it)
    {
        cout << it->first << endl;
    }
}

void print_cours_actions(map<string,action>& actions, const string nom)
{
   
    map<string,action >::iterator it = actions.find(nom);
    cout << (*it).second.suivi_cours_action.size() << endl;
    cout << (*it).second.cours_action << endl;
    for (vector<int>::iterator itv = (*it).second.suivi_cours_action.begin(); itv!=(*it).second.suivi_cours_action.end();++itv)
    {  
        cout << (*itv) << ",";
    }
}

void historique_action(string nom, map<string, action> actions)
{
    map<string,action >::iterator ite = actions.find(nom);
    ofstream action("C:/Users/AHMED-YASSINE/Desktop/COURS ENSTA/2A/SIM202/cours_action/cours_" + (*ite).second.nom + ".txt");
    if (action)
    {
        for( vector<int>::iterator it= (*ite).second.suivi_cours_action.begin(); it != (*ite).second.suivi_cours_action.end(); ++it)
        {
            action << *it << ";";
        }
    }  
}

void print_map(map<string,Client> clients)
{
	for(map<string,Client >::iterator it=clients.begin(); it!=clients.end(); ++it)
	{
		cout << it->first << endl;
		
	}

}



void print_action_av(multimap<int,Ordre> map_vente, multimap<int,Ordre, greater<int> > map_achat )
{
    cout << "               Offre de vente" << "                "<< "Offre d'achat" << endl;
	cout << "               PRIX | QUANTITE"<< "                     PRIX | QUANTITE"<< endl ;

    multimap<int,Ordre>::iterator itv=map_vente.begin();
    multimap<int,Ordre>::iterator ita=map_achat.begin();
    while( itv != map_vente.end() && ita != map_achat.end())
	{
		cout <<"                "<< (*itv).first << " | " << (*itv).second.quantite << "                "<< (*ita).first << " | " << (*ita).second.quantite << endl;
        ++ita;
        ++itv;

	}
}



void print_maps_av(map<string,multimap<int,Ordre> > map_vente , map<string,multimap<int,Ordre, greater<int> > > map_achat)
{
    map<string,multimap<int,Ordre > >::iterator itv=map_vente.begin();
    map<string,multimap<int,Ordre, greater<int> > >::iterator ita=map_achat.begin();
    while( ita!=map_achat.end() && itv!=map_vente.end())
	{
		cout <<"                                     "<< ita->first << endl;
		print_action_av(itv->second, ita->second);
        cout << "                                       " << endl;
        ++ita;
        ++itv;
	}
    cout << "                                       " << endl;
    cout << "                                       " << endl;
}






void Fixing(multimap<int,Ordre, greater<int> >& liste_achat, multimap<int,Ordre>& liste_vente, map<string,Client >& clients, map <string, action>& actions)
{
    
	multimap<int,Ordre>::iterator itv;
    multimap<int,Ordre>::iterator ita;
	multimap<int,Ordre>::iterator it;
	int test;
    bool fixing = true;

    while (fixing)
    {
        if(liste_vente.empty())
        {
            return;
        }
        if(liste_achat.empty())
        {
            return;
        }
        ita=liste_achat.begin();
        itv=liste_vente.begin();
        Message_serveur transaction_achat;
        Message_serveur transaction_vente;
		if((*ita).first>=(*itv).first)
        {
                cout << "Transaction : " << (*ita).second.action << endl;
                test=(*ita).second.quantite-(*itv).second.quantite;
                if(test>0)
				{ 

                    //      Le fixing       // :
                    map<string,action >::iterator itb = actions.find((*ita).second.action);
                    (*itb).second.print();
                    cout << (*itb).second.suivi_cours_action.size();
                    (*itb).second.suivi_cours_action.push_back((*itv).second.prix); // rajoute le cours de l'action à son historique des cours
                    (*itb).second.cours_action = (*itv).second.prix; //on mets à jours de prix de l'action
            
                    (*ita).second.quantite-=(*itv).second.quantite; // on mets à jour la quantité
                    liste_vente.erase(itv); //on supprime l'offre de vente puique le client a tout acheté





                    map<string,Client >::iterator itm = clients.find((*itv).second.id_cl);
                    if(itm==clients.end())
                    {
                        break;
                    }
                    transaction_vente.client = &itm->second;
                    (*itm).second.num_mes++;
                    transaction_vente.num_message = (*itm).second.num_mes;
                    Ordre ordre_vente (0,(*itv).second.action,(*itv).second.quantite,(*itv).second.prix);
                    transaction_vente.ordres.push_back(ordre_vente);
                    transaction_vente.envoie_mes_serveur();
        

                    map<string,Client >::iterator itm_2 = clients.find((*ita).second.id_cl);
                    if(itm_2==clients.end())
                    {
                        break;
                    }
                    transaction_achat.client = &itm_2->second;
                    (*itm_2).second.num_mes ++;
                    transaction_achat.num_message = (*itm_2).second.num_mes;
                    Ordre ordre_achat (1,(*ita).second.action,(*itv).second.quantite,(*itv).second.prix);
                    transaction_achat.ordres.push_back(ordre_achat);
                    transaction_achat.envoie_mes_serveur();
                    


                }

                if(test<0)
				{

                    //          Fixing          : // 
                    map<string,action >::iterator itb = actions.find((*ita).second.action);
                    (*itb).second.suivi_cours_action.push_back((*itv).second.prix);
                    (*itb).second.cours_action = (*itv).second.prix;

                    (*itv).second.quantite-=(*ita).second.quantite;
                    liste_achat.erase(ita);


                    // enregistrer la transaction d'achat et envoyer un message au client
                    map<string,Client >::iterator itm = clients.find((*ita).second.id_cl);
                    if(itm==clients.end())
                    {
                        exit(-1);
                    }
                    transaction_achat.client = &itm->second;
                    (*itm).second.num_mes ++;
                    transaction_achat.num_message = (*itm).second.num_mes;
                    Ordre ordre_achat (1,(*ita).second.action,(*ita).second.quantite,(*itv).second.prix);
                    transaction_achat.ordres.push_back(ordre_achat);
                    transaction_achat.envoie_mes_serveur();


                    // enregistrer la transaction de vente et envoyer un message au client
                    map<string,Client >::iterator itm_2 = clients.find((*itv).second.id_cl);
                    if(itm_2==clients.end())
                    {
                        exit(-1);
                    }
                    
                    transaction_vente.client = &itm_2->second;
                    (*itm_2).second.num_mes ++;
                    transaction_vente.num_message = (*itm_2).second.num_mes;
                    Ordre ordre_vente (0,(*itv).second.action,(*ita).second.quantite,(*itv).second.prix);
                    transaction_vente.ordres.push_back(ordre_vente);
                    transaction_vente.envoie_mes_serveur();

                    

                }
                if(test==0)
				{
                    //      Fixing      : //
                    map<string,action >::iterator itb = actions.find((*ita).second.action);                    
                    (*itb).second.suivi_cours_action.push_back((*itv).second.prix);
                    (*itb).second.cours_action = (*itv).second.prix;
                    liste_vente.erase(itv);
                    liste_achat.erase(ita);


                    map<string,Client >::iterator itm = clients.find((*itv).second.id_cl);
                    if(itm==clients.end())
                    {
                        exit(-1);
                    }
                    transaction_vente.client = &itm->second;
                    (*itm).second.num_mes ++;
                    transaction_vente.num_message = (*itm).second.num_mes;
                    transaction_vente.ordres.push_back((*itv).second);
                    transaction_vente.envoie_mes_serveur();


                    map<string,Client >::iterator itm_2 = clients.find((*ita).second.id_cl);
                    if(itm_2==clients.end())
                    {
                        exit(-1);
                    }
                    transaction_achat.client = &itm_2->second;
                    (*itm_2).second.num_mes ++;
                    transaction_achat.num_message = (*itm_2).second.num_mes;
                    Ordre o (1,(*ita).second.action,(*ita).second.quantite,(*itv).second.prix);
                    transaction_achat.ordres.push_back(o);
                    transaction_achat.envoie_mes_serveur();




                }
        }
        else {fixing = false;}

    }
}






void update_tableaux(list<Ordre> ordres, map<string,multimap<int,Ordre, greater<int>>>& map_achat, map<string,multimap<int,Ordre> >& map_vente, map<string,Client >& clients, map <string, action>& actions, bool ouverture)
{
	list<Ordre>::iterator ite;
	map<string,multimap<int,Ordre> >::iterator it;
    map<string,multimap<int,Ordre, greater<int> > >::iterator it_2;
    if (ouverture==true)
    {
        for (ite = ordres.begin();ite!=ordres.end();ite++)
    	{
		    bool t = 0;
		    if((*ite).achat==1)
		    {
			        for(it_2 = map_achat.begin();it_2!=map_achat.end();++it_2)
			        {
				            if((*it_2).first==(*ite).action)
				            {
					            (*it_2).second.insert(pair<int,Ordre>((*ite).prix,*ite));
					            t = 1;
                                Fixing(map_achat[(*ite).action],map_vente[(*ite).action],clients,actions);
				            }
			        }
			        if(t==0)
			        {
				        map_achat.insert(pair<string,multimap<int,Ordre, greater<int> > >((*ite).action,multimap<int,Ordre, greater<int> >()));
				        map_achat[(*ite).action].insert(pair<int,Ordre>((*ite).prix,*ite));	
                        map_vente.insert(pair<string,multimap<int,Ordre> >((*ite).action,multimap<int,Ordre>()));
			        }
		    }
		    else
		    {
			        for(it = map_vente.begin();it!=map_vente.end();++it)
			        {
				        if((*it).first==(*ite).action)
		                {
					        (*it).second.insert(pair<int,Ordre>((*ite).prix,*ite));
					        t=1;
                            Fixing(map_achat[(*ite).action],map_vente[(*ite).action],clients,actions);
				        }
			        }
			        if(t==0)
			        {
                        map_achat.insert(pair<string,multimap<int,Ordre, greater<int> > >((*ite).action,multimap<int,Ordre, greater<int> >()));
				        map_vente.insert(pair<string,multimap<int,Ordre> >((*ite).action,multimap<int,Ordre>()));
				        map_vente[(*ite).action].insert(pair<int,Ordre>((*ite).prix,*ite));
                
			        }
		    }

        print_maps_av(map_vente, map_achat);
	 }
    }
    else
    {
        for (ite= ordres.begin();ite!=ordres.end();ite++)
    	{
		    bool t = 0;
		    if((*ite).achat==1)
		    {
			        for(it_2 = map_achat.begin();it_2!=map_achat.end();++it_2)
			        {
				            if((*it_2).first==(*ite).action)
				            {
					            (*it_2).second.insert(pair<int,Ordre>((*ite).prix,*ite));
					            t = 1;
				            }
			        }
			        if(t==0)
			        {
				        map_achat.insert(pair<string,multimap<int,Ordre, greater<int> > >((*ite).action,multimap<int,Ordre, greater<int> >()));
				        map_achat[(*ite).action].insert(pair<int,Ordre>((*ite).prix,*ite));	
                        map_vente.insert(pair<string,multimap<int,Ordre> >((*ite).action,multimap<int,Ordre>()));
			        }
		    }
		    else
		    {
			        for(it = map_vente.begin();it!=map_vente.end();++it)
			        {
				        if((*it).first==(*ite).action)
		                {
					        (*it).second.insert(pair<int,Ordre>((*ite).prix,*ite));
					        t=1;
				        }
			        }
			        if(t==0)
			        {
                        map_achat.insert(pair<string,multimap<int,Ordre, greater<int> > >((*ite).action,multimap<int,Ordre, greater<int> >()));
				        map_vente.insert(pair<string,multimap<int,Ordre> >((*ite).action,multimap<int,Ordre>()));
				        map_vente[(*ite).action].insert(pair<int,Ordre>((*ite).prix,*ite));
                
			        }
		    }
        print_maps_av(map_vente, map_achat);
	 }
    }
    
     
}





//---------------------------------------------------------------------
//                         classe message
//---------------------------------------------------------------------
// classe mère pour les messages


Message::Message()
{
    client = NULL;
    num_message =0;

}

Message::Message(Client* c, int num, list <Ordre> o)
{
    client = c;
    num_message = num;
    ordres = o;
}




//---------------------------------------------------------------------
// classe des messages envoyés par le client
//---------------------------------------------------------------------


Message_client::Message_client()
{
    client = NULL;
    num_message =0;
}

Message_client::Message_client(Client* c, int num, list <Ordre> o)
{
    client = c;
    num_message = num;
    ordres = o;
}

// fonction pour encodé les messages envoyés par le client
string Message_client::encode_mes_client() 
{
    string message_client;
    message_client = client->id_cl+";";
    message_client += to_string(num_message)+";";
    for (list<Ordre>::iterator ite=ordres.begin();ite!=ordres.end();++ite)
    {
        message_client+= (*ite).encode();
        message_client += ";";
    }
    return message_client;
}

void Message_client::decode_mes_client(const string &message,  map<string,Client >& clients)
{
    //cout << "entrée !!!" << endl << flush;
    vector<string> result;
    istringstream iss(message);
    string str;
    while(getline(iss,str,';'))
    {
        result.push_back(str);
    }
    vector<string>::iterator ite = result.begin();
    map<string,Client >::iterator itm = clients.find(*ite);
    if(itm==clients.end())
    {
        cout << "Premiere connexion, entrez votre montant : " << endl;
        int montant;
        cin >> montant;
        Client nv_client(*ite,montant);
        clients.insert(pair<string,Client>(nv_client.id_cl,nv_client));

        map<string,Client >::iterator it = clients.find(*ite);
        client = &it->second;

        ite++;
        num_message = stoi(*ite);
        ite++;
        ordres.clear();
        for(;ite!=result.end();++ite)
        {
            Ordre o(1,"rien",0,0);
            o.decode(*ite);
            o.id_cl = client->id_cl;
            ordres.push_back(o);
        }

        return;
    }
    client = &itm->second;
    ite++;
    num_message = stoi(*ite);
    ite++;
    ordres.clear();
    for(;ite!=result.end();++ite)
    {
        Ordre o(1,"rien",0,0);
        o.decode(*ite);
        o.id_cl = client->id_cl;
        ordres.push_back(o);
    }
    return;
}

void Message_client::print() 
{
    cout << (*this).encode_mes_client()<<endl;

}

void Message_client::envoie_mes_client()
{
    string nom_fichier;
    nom_fichier = "C:/Users/AHMED-YASSINE/Desktop/COURS ENSTA/2A/SIM202/messages_serveur/CLI_" + client->id_cl + "_" + to_string(num_message) + ".mes.lock.txt";
    ofstream message_verouiller(nom_fichier.c_str());
	ofstream message_pret("C:/Users/AHMED-YASSINE/Desktop/COURS ENSTA/2A/SIM202/messages_serveur/CLI_" + client->id_cl + "_" + to_string(num_message) + ".mes.txt");
    if(message_pret)
    {
        string contenu_du_mes = (*this).encode_mes_client();
        message_pret << contenu_du_mes;
    }
    message_verouiller.close();
    remove(nom_fichier.c_str());
}
//---------------------------------------------------------------------


//---------------------------------------------------------------------
// classe des messages envoyés par le serveur
//---------------------------------------------------------------------


Message_serveur::Message_serveur()
{
    client = NULL;
    num_message =0;
}

Message_serveur::Message_serveur(Client* c, int num, list <Ordre> o)
{
    client = c;
    num_message = num;
    ordres = o;
}

string Message_serveur::encode_mes_serveur(){
    string message_client;
    message_client = client->id_cl+";";
    message_client += to_string(num_message)+";";
    for (list<Ordre>::iterator ite=ordres.begin();ite!=ordres.end();++ite)
    {
        message_client+= (*ite).encode();
        message_client += ";";
    }
    return message_client;
}

string Message_serveur::encode_acr_serveur(){
    string message_serveur;
    message_serveur = client->id_cl+";";
    message_serveur += to_string(num_message)+";";
    
    double prix_total = 0;
    for (list<Ordre>::iterator ite=ordres.begin();ite!=ordres.end();++ite){
        prix_total += (*ite).prix*(*ite).quantite;
    }

    if (prix_total <= client->solde){
        message_serveur += "accepté";
    } else {
        message_serveur += "refusé";
    }
return message_serveur;
}

void Message_serveur::envoie_mes_serveur()
{
    string nom_fichier;
    nom_fichier = "C:/Users/AHMED-YASSINE/Desktop/COURS ENSTA/2A/SIM202/messages_clients/SER_" + client->id_cl + "_" + to_string(num_message) + ".mes.lock.txt";
    ofstream message_verouiller(nom_fichier.c_str());
    ofstream message_pret("C:/Users/AHMED-YASSINE/Desktop/COURS ENSTA/2A/SIM202/messages_clients/SER_" + client->id_cl + "_" + to_string(num_message) + ".mes.txt");
    if(message_pret)
    {
        string contenu_du_mes = (*this).encode_mes_serveur();
        message_pret << contenu_du_mes;
    }
    message_verouiller.close(); 
    remove(nom_fichier.c_str());
}

void Message_serveur::envoie_acr_serveur()
{
    string nom_fichier_2;
    nom_fichier_2 = "C:/Users/AHMED-YASSINE/Desktop/COURS ENSTA/2A/SIM202/messages_clients/SER_" + client->id_cl + "_" + to_string(num_message) + ".acr.lock.txt";
    ofstream message_verouiller(nom_fichier_2.c_str());
    ofstream acr_pret("C:/Users/AHMED-YASSINE/Desktop/COURS ENSTA/2A/SIM202/messages_clients/SER_" + client->id_cl + "_" + to_string(num_message) + ".acr.txt");
    if(acr_pret)
    {
        string contenu_du_mes = (*this).encode_acr_serveur();
        acr_pret << contenu_du_mes;
    }
    message_verouiller.close(); 
    remove(nom_fichier_2.c_str());
}


void Message_serveur::decode_mes_client(const string &message,  map<string,Client >& clients)
{
    vector<string> result;
    istringstream iss(message);
    string str;
    while(getline(iss,str,';'))
    {
        result.push_back(str);
    }

    vector<string>::iterator ite=result.begin();
    map<string,Client >::iterator itm = clients.find(*ite);
    if(itm==clients.end())
    {
        cout << "Premire connexion, entrer votre montant : " << endl;
        int montant;
        cin >> montant;
        Client nv_client(*ite,montant);
        clients.insert(pair<string,Client>(nv_client.id_cl,nv_client));
    

        map<string,Client >::iterator it = clients.find(*ite);
        client = &it->second;

        ite++;
        num_message = stoi(*ite);
        ite++;
        ordres.clear();
        for(;ite!=result.end();++ite)
        {
            Ordre o(1,"rien",0,0);
            o.decode(*ite);
            o.id_cl = client->id_cl;
            ordres.push_back(o);
        }

        return;
    }
    client = &itm->second;

    ite++;
    num_message = stoi(*ite);
    ite++;
    ordres.clear();
    for(;ite!=result.end();++ite)
    {
        Ordre o(1,"rien",0,0);
        o.decode(*ite);
        o.id_cl = client->id_cl;
        ordres.push_back(o);
    }
    return;
}


void Message_serveur::print() 
{
    cout << (*this).encode_mes_serveur()<<endl;

}


////////////////////////////////////////////////////////////////////////////////////
/////// Client automatique 


Ordre ordre_aleatoire (map<string,multimap<int,Ordre, greater<int> > >& map_achat, map<string,action>& actions)
{
    srand(time(NULL));
    Ordre ordre(0,"",0,0);
    int A_ou_V=rand()%2;
    ordre.achat=A_ou_V;

    int W_action=rand()%(actions.size());
    int j =0;
    map<string,action >::iterator it=actions.begin();
    for (it=actions.begin(); it!=actions.end(); ++it)
    {
        if(j==W_action)
        {
            ordre.action = (*it).first;
        }
        j++;
    }
    map<string,action >::iterator itb = actions.find(ordre.action);
    srand(time(0));
    int ecart = rand()%50+1;
    int prix = (*itb).second.cours_action-25 +ecart;
    ordre.prix = prix;
    int quantity = 100*ecart;
    ordre.quantite = quantity;
    return(ordre);



}




