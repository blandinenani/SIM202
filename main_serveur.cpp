#include <iostream>
#include <string>
#include <fstream>
#include "zone_message.hpp"
#include "zone_message.cpp"
#include<cstdlib>
#include<map>
#include <iterator>
#include <dirent.h>
#include <stdlib.h>   
#include <time.h>       
#include <unistd.h>
#include <cstdio>
#include<chrono>
using namespace std;





int main(int argc, char *argv[])
{

		
	map<string,multimap<int,Ordre> > map_vente;

	map<string,multimap<int,Ordre, greater<int> > > map_achat;

	map <string, action> liste_actions;

	cout << "Creation des liste_actions" << endl;

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
	Client client_3 ("BLCHA",65149850);
	liste_clients.insert(pair<string,Client>(client_1.id_cl,client_1));
	liste_clients.insert(pair<string,Client>(client_2.id_cl,client_2));
	liste_clients.insert(pair<string,Client>(client_3.id_cl,client_3));

	chrono::steady_clock::time_point tic=std::chrono::steady_clock::now();
	chrono::steady_clock::time_point tac=std::chrono::steady_clock::now();
	int Temps = std::chrono::duration_cast<std::chrono::milliseconds>(tac - tic).count();
	
	cout << "On commence la journee" << endl;
	while(Temps <660)
	{
			bool ouverture = false;
    		tac= std::chrono::steady_clock::now();
			Temps = std::chrono::duration_cast<std::chrono::seconds>(tac - tic).count();
			cout << "Temps : " << endl;
			cout << Temps << endl;

			if (Temps>90 && Temps <420)
			{
				ouverture = true;
			}

				struct dirent *dir;
				DIR *doss = opendir("C:/Users/AHMED-YASSINE/Desktop/COURS ENSTA/2A/SIM202/messages_serveur/"); 
				string locked_msg;
				string nom_fichier;
				bool ouvrir=true;
				if (doss)
				{			
					while ((dir = readdir(doss)) != NULL)
					{
						locked_msg = dir->d_name;
						cout << locked_msg << endl;
						if (locked_msg.size()>10)
						{
							locked_msg = dir->d_name;
							nom_fichier = dir->d_name;
							locked_msg.erase(locked_msg.size()-3,locked_msg.size()-1);
							locked_msg += "lock.txt";

							DIR *doss2 = opendir("C:/Users/AHMED-YASSINE/Desktop/COURS ENSTA/2A/SIM202/messages_serveur/"); 
							if (doss2)
							{
								while ((dir = readdir(doss2)) != NULL)
								{
									if (dir->d_name==locked_msg)
									{
										ouvrir = false;
									}
								}
								closedir(doss2);

							}
							if (ouvrir)
							{
								ifstream fichier("C:/Users/AHMED-YASSINE/Desktop/COURS ENSTA/2A/SIM202/messages_serveur/"+nom_fichier);
								string message;
								getline(fichier,message);

								Message_serveur acr;
								acr.decode_mes_client(message,liste_clients);
								acr.envoie_acr_serveur();

								Message_client message_cli_decode;
								message_cli_decode.decode_mes_client(message,liste_clients);

								update_tableaux(message_cli_decode.ordres,map_achat,map_vente,liste_clients,liste_actions,ouverture);
								historique_action("Apple", liste_actions);
								historique_action("Samsung", liste_actions);
								historique_action("Tesla", liste_actions);
								historique_action("Microsoft", liste_actions);
								historique_action("Facebook", liste_actions);
								historique_action("OpenAI", liste_actions);
								historique_action("Boeing", liste_actions);

								fichier.close();
								remove(("C:/Users/AHMED-YASSINE/Desktop/COURS ENSTA/2A/SIM202/messages_serveur/"+nom_fichier).c_str());
							}
						}
					}
			closedir(doss);
				}
			
	sleep(1);
	}
}




