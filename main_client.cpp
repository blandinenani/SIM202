#include <iostream>
#include <string>
#include <fstream>
#include "zone_message.hpp"
#include "zone_message.cpp"
#include<cstdlib>
#include<map>
#include <iterator>
#include <unistd.h>
#include <chrono>

using namespace std;









int main(int argc, char *argv[])
{


	map <string, action> liste_actions;

	action action_1 ("Apple",340);
	action action_2 ("Samsung",138);
	action action_3 ("Tesla",400);
	action action_4 ("Microsoft",600);
	action action_5 ("Facebook",540);
	action action_6 ("OpenAI",110);
	action action_7 ("Boeing",206);


	liste_actions.insert(pair<string, action>(action_1.nom,action_1));
	liste_actions.insert(pair<string, action>(action_2.nom,action_2));
	liste_actions.insert(pair<string, action>(action_3.nom,action_3));
	liste_actions.insert(pair<string, action>(action_4.nom,action_4));
	liste_actions.insert(pair<string, action>(action_5.nom,action_5));
	liste_actions.insert(pair<string, action>(action_6.nom,action_6));
	liste_actions.insert(pair<string, action>(action_7.nom,action_7));
	


	map<string,Client> liste_clients;
	Client client_1 ("AYC", 99999999);
	Client client_2 ("EMDC",900105488);
	liste_clients.insert(pair<string,Client>(client_1.id_cl,client_1));
	liste_clients.insert(pair<string,Client>(client_2.id_cl,client_2));


	map<string,multimap<int,Ordre> > map_vente;
	map<string,multimap<int,Ordre, greater<int> > > map_achat;





	chrono::steady_clock::time_point tic= std::chrono::steady_clock::now();
	chrono::steady_clock::time_point tac= std::chrono::steady_clock::now();
	int Temps = std::chrono::duration_cast<std::chrono::milliseconds>(tac - tic).count();
	cout << "Choississez si vous voulez le mode manuel ou automatique : manuel/automatique" << endl;
	string mode;
	cin >> mode;

	if (mode == "automatique"){
		while(Temps <660)
			{
		
				tac= std::chrono::steady_clock::now();
				Temps = std::chrono::duration_cast<std::chrono::seconds>(tac - tic).count();
				cout << Temps << endl;
				Message_client message_aleatoire;
				Ordre ordre_1 (0,"",0,0);
				ordre_1 = ordre_aleatoire(map_achat,liste_actions);
				ordre_1.print();
				message_aleatoire.client = &client_1;
				message_aleatoire.num_message = client_1.num_mes;
				client_1.num_mes += 1;
				message_aleatoire.ordres.push_back(ordre_1);
				message_aleatoire.envoie_mes_client();
				sleep(1);
			}
	}
	else {
		while(true)
			{

			cout << "Mode mannuel, donnez :  votre id client; numero du message ; Ordre = achat,action,quantité,prix" << endl;
			string s;
			cin >> s;
			Message_client message;
			cout << "Avant" << endl;
			print_map(liste_clients);
			message.decode_mes_client(s, liste_clients);
			cout << "Apres" << endl;
			print_map(liste_clients);
			message.envoie_mes_client();

			}
	}
}
