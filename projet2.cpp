#include <sys/stat.h>
#include <limits>
#include <unistd.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <unordered_map>
using namespace std;

const string TEMP_FILE = "temp";

// Debut de la fonction prototype
void clearScreen(); // declaration de la fonction de suppression 
vector<string> splitS (const string &s, const string &delimiter);
inline bool fileExists (const string &name);
void readString (string &input);
string makeNewData (int &choice);


// fin de la fonction prototype


// Creation des classes

class Personne {
    public:
        int id;
        string nom;
        int zoneGeo;
        string formatForFile() {return to_string(id) + " " + nom + " " + to_string(zoneGeo);}
        string formatForUser(const string &temp) {return temp + "\nid:" + to_string(id) + " name:" + nom + " zone:" + to_string(zoneGeo);}
};

class Recommande {
    public:
        int id;
        string type;
        int idFacteur;
        int idDestinataire;
        Recommande(int x, string s, int f, int d) {id = x; type = s; idFacteur = f; idDestinataire = d;}
        string formatForFile() {return to_string(id) + " " + type + " " + to_string(idFacteur) + " " + to_string(idDestinataire);}
        string formatForUser() {return "RECOMMANDE\nid:" + to_string(id) + " type:" + type + " idFacteur:" + to_string(idFacteur) + " idDestinataire:" + to_string(idDestinataire);}
};

class Facteur : public Personne {
    public:
        Facteur(int x, string n, int z) {id = x; nom = n; zoneGeo = z;}
        
};

class Habitant : public Personne {
    public:
        Habitant(int x, string n, int z) {id = x; nom = n; zoneGeo = z;}
};

// creation des classes terminer

int main() {
    string filename, line, password;
    int deleteLine, choice;


    cout << "donner un nom pour la base de donnee:" << endl;
    readString(filename);
    filename = filename + ".txt";

    while (true) { // While loop  pour que le programme ne sera pas fermer lorsque la tache est terminer
        cout << "fait ton propre choix (0:ajouter une ligne, 1:supprime une donnee, 2:afficher info):" << endl;
        cin >> choice;

        if (choice == 0) { // ajoue d'une nv ligne

            int classType;
            string output;

            cout << "ajouter? (0:Facteur, 1:Habitant, 2:Recommande):" << endl;
            cin >> choice;

            if (choice != 0 && choice != 1 && choice != 2) {

                cout << "vraiment???\n" << endl;
                return 1;

            }

            classType = choice;

            output = to_string(classType) + " " + makeNewData(choice);

            //ouverture du fichier 
            ofstream fileOut;
            fileOut.open(filename, ios_base::app);

            // mettre les donner dans le fichier 
            fileOut << output << endl;

            fileOut.close();

        } else if (choice == 1) { // supprimer la ligne 
            int i=0;

            cout << "donner le numero de la ligne pour la supprimer:" << endl;
            cin >> deleteLine;
            deleteLine -= 1;

            ifstream fileIn;
            fileIn.open(filename);
            ofstream temp;
            temp.open(TEMP_FILE);

            while (getline(fileIn, line)) {
                //ecrire toutes les lignes..
                if (i != deleteLine)
                    temp << line << endl;
                i++;
            }
            temp.close();
            fileIn.close();

            // les fichier originaux par les fichier temp
            const char * p = filename.c_str();
            remove(p);
            rename(TEMP_FILE.c_str(), p);

            cout << "suppression terminer: " + to_string(deleteLine + 1) + "\n" << endl;

        } else if (choice == 2) { // lire fichier
            int i=1;

            ifstream fileIn;
            fileIn.open(filename);

            // Loop ligne par ligne
            while (getline(fileIn, line)) {

                vector<string> vector_input;
                string toShow;
                int classType;
                vector_input = splitS(line, " ");
                classType = stoi(vector_input[0]);
                if (classType == 0) {

                    Facteur fac(stoi(vector_input[1]),vector_input[2],stoi(vector_input[3]));
                    toShow = fac.formatForUser("FACTEUR");

                } else if (classType == 1) {

                    Habitant hab(stoi(vector_input[1]),vector_input[2],stoi(vector_input[3]));
                    toShow = hab.formatForUser("HABITANT");

                } else if (classType == 2) {

                    Recommande reco(stoi(vector_input[1]),vector_input[2],stoi(vector_input[3]),stoi(vector_input[4]));
                    toShow = reco.formatForUser();
                }
                cout << to_string(i) + ":\n" + toShow + "\n" << endl;
                i++;
            }

            fileIn.close();

        } else {
		}
		           cout << "Deja afficher\n" << endl;
            return 1;

        }
        
        // voulez vous quitter?
        char tmp='0';
        cout << "Quitter? (Oui/Non)" << endl;
           while (tmp != 'n' && tmp != 'N' && tmp != 'y' && tmp != 'Y') {
            cin >> tmp;
        }
        if (tmp == 'y' || tmp == 'Y') 
	     	    
        clearScreen();

    }
    


vector<string> splitS(const string &s, const string &delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = s.find (delimiter, pos_start)) != string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}

// verifier si le fichier existe
inline bool fileExists (const string &name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}
void readString(string &input) {
    getline(cin, input);
    if (input == "") {
        readString(input);
    }
}
string makeNewData(int &choice) {

    string text_input;
    vector<string> vector_input;

    if (choice == 0 || choice == 1) {

        Personne personne;
        cout << "donner <id> <name> <zone geo>:" << endl;
        readString(text_input);
        vector_input = splitS(text_input, " ");

        if (choice == 0) {
            personne = Facteur(stoi(vector_input[0]),vector_input[1],stoi(vector_input[2]));
            cout << "ajoue valide:\n\n" + personne.formatForUser("FACTEUR") + "\n" << endl;
        }
        if (choice == 1) {
            personne = Habitant(stoi(vector_input[0]),vector_input[1],stoi(vector_input[2]));
            cout << "ajoue valide:\n\n" + personne.formatForUser("HABITANT") + "\n" << endl;
        }

        return personne.formatForFile();

    } else if (choice == 2) {

        string type[2] = {"lettre", "colis"};
        cout << "type? (0:lettre, 1:colis)" << endl;
        cin >> choice;
        cout << "donner <id> <idFacteur> <idDestinataire>" << endl;
        readString(text_input);
        vector_input = splitS(text_input, " ");

        Recommande reco(stoi(vector_input[0]),type[choice],stoi(vector_input[1]),stoi(vector_input[2]));

        cout << "ajoue valide:\n\n" + reco.formatForUser() + "\n" << endl;

        return reco.formatForFile();
    }

    return 0;
}