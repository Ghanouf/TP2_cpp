
// j'ai fait 2 projet celui la contient l'outil de cryptage et le 2 eme non!
//merci!

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

static const string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ""abcdefghijklmnopqrstuvwxyz""0123456789+/";

// le Debut de notre programme
//Debut de notre fonction
void clearScreen();
vector<string> splitS (const string &s, const string &delimiter);
inline bool fileExists (const string &name);
void readString (string &input);
string makeNewData (int &choice);
string xorCipher(const bool &decrypt, string str, const string &password);  //pour le dechiffrement
string hashString (const string& str);  // the process of transforming key
static inline bool is_base64(unsigned char c);
vector<unsigned char> stringToUnsignedChar(const string& str);
string unsignedCharVectorToString(const vector<unsigned char>& vec);  //vecteur d'infos
string base64Encode(unsigned char const* buf, unsigned int bufLen);
vector<unsigned char> base64Decode(string const& encoded_string);
string stringToBase64(const string &in);
string base64ToString(const string &in);
// fin de notre fonction 



// nos classe

class Personne {       // creation de la classe mére personne
    public:
        int id;
        string nom;
        int zoneGeo;         // il existe un constructeur par défaut
        string formatForFile() {return to_string(id) + " " + nom + " " + to_string(zoneGeo);}
        string formatForUser(const string &temp) {return temp + "\nid:" + to_string(id) + " name:" + nom + " zone:" + to_string(zoneGeo);}
};

class Recommande {      //creation de la classe recommande
    public:
        int id;
        string type;
        int idFacteur;
        int idDestinataire;
        Recommande(int x, string s, int f, int d) {id = x; type = s; idFacteur = f; idDestinataire = d;} // notre constructeur
        string formatForFile() {return to_string(id) + " " + type + " " + to_string(idFacteur) + " " + to_string(idDestinataire);}
        string formatForUser() {return "RECOMMANDE\nid:" + to_string(id) + " type:" + type + " idFacteur:" + to_string(idFacteur) + " idDestinataire:" + to_string(idDestinataire);}
};

class Facteur : public Personne {
    public:
        Facteur(int x, string n, int z) {id = x; nom = n; zoneGeo = z;}   // constr!!!
        
};

class Habitant : public Personne {
    public:
        Habitant(int x, string n, int z) {id = x; nom = n; zoneGeo = z;}    //contr!!!
};

// fin de la creation des classes


// ici on vas programmer un systeme qui va lire chaque ligne et chaque donnée de cette ligne 
int main() {
    string filename, line, password;
    int deleteLine, choice;   // choix de l'utilisateur ( le premier choix est de supprimer une ligne )!!

    clearScreen(); //fonction pour passer a une autre action ( vider l'ecran)

    while (true) { // Loop pour attendre le mot de pass correct en fonction du base de données

        cout << "donner un nom pour la base de donnee:" << endl;
        readString(filename);
        clearScreen();
        filename = filename + ".txt";

        if (fileExists(filename)) { // Ficher existe donc pose la qst du mot de pass

            cout << "base de donnee existe deja, donne le mot de pass:" << endl;
            readString(password);
            clearScreen();

            // avoir la premiere ligne (encrypted hash of the password)
            ifstream fileIn;
            fileIn.open(filename);
            getline(fileIn, line);
            line = xorCipher(true, line,password);

            // verifier si le mot de passe est correct
            if (hashString(password) == line) {
                cout << "mot de pass correct" << endl;
                break;
            } else {
                clearScreen();
                cout << "mot de pass incorrect...\n" << endl;
            }

        } else {

            cout << "Cree une nv base de donnee...\nmot de pass demander:" << endl;
            readString(password);

            // Cree une nv base de donnée fichier
            // ajouter encrypted hash of the password dans la premiere ligne du fichier 
            ofstream fileOut;
            fileOut.open(filename, ios_base::app);
            fileOut << xorCipher(false, hashString(password),password) << endl;

            fileOut.close();

            break;
        }

    }

    while (true) { // loop pour ne pas fermer le programme lorsque c terminé

        clearScreen();
        cout << "Fait ton choix (0:AJOUTER LIGNE, 1:SUPPRESSION DONNEE, 2:AFFICHER):" << endl;
        cin >> choice;
        clearScreen();

        if (choice == 0) { // creation du nv ligne 

            int classType;
            string output;

            cout << "qlq chose pour l'ajouter ? (0:Facteur, 1:Habitant, 2:Recommande):" << endl;
            cin >> choice;
            if (choice != 0 && choice != 1 && choice != 2) {

                clearScreen();
                cout << "comment?\n" << endl;
                return 1;

            }

            classType = choice;

            // en vas encrpter 
            clearScreen();
            output = xorCipher(false, to_string(classType) + " " + makeNewData(choice),password);

            //ouvrir fichier
            ofstream fileOut;
            fileOut.open(filename, ios_base::app);

            // mettre les données dans le fichier
            fileOut << output << endl;

            fileOut.close();

        } else if (choice == 1) {
            int i=0;

            cout << "donner le numero de la ligne pour la supprimer:" << endl;
            cin >> deleteLine;

            ifstream fileIn;
            fileIn.open(filename);
            ofstream temp;
            temp.open(".tmp.txt");

            while (getline(fileIn, line)) {
 
                if (i != deleteLine || i == 0)
                    temp << line << std::endl;
                i++;
            }
            temp.close();
            fileIn.close();

            // remplacer les fichier originaux par les fichier temporelles
            const char * p = filename.c_str();
            remove(p);
            std::rename(".tmp.txt", p);

            clearScreen();
            cout << "suppression valider: " + to_string(deleteLine) + "\n" << endl;

        } else if (choice == 2) { // lire fichier
            int i=0;

            ifstream fileIn;
            fileIn.open(filename);

            // Loop lire ligne par ligne
            while (getline(fileIn, line)) {
                if (i==0) {i++; continue;}

                // Decrypttion de la ligne
                line = xorCipher(true, line,password);

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

            clearScreen();
            cout << "Deja afficher\n" << endl;
            return 1;

        }
        
        // poser la qst a l'utilisateur esque il veut quitter le programme 
        char tmp='0';
        cout << "Quitter? (OUI/NON)" << endl;
        while (tmp != 'n' && tmp != 'N' && tmp != 'y' && tmp != 'Y') {
            cin >> tmp;
        }
        if (tmp == 'y' || tmp == 'Y') break;
        clearScreen();

    }

    clearScreen();
    return 0;
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

// verification si un fichier existe 
inline bool fileExists (const string &name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}

// vider le terminal
void clearScreen() {
    #if defined _WIN32
        system("cls");
    #elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
        system("clear"); 
    #elif defined (__APPLE__)
        system("clear");
    #endif
}
void readString(string &input) {
    getline(cin, input);
    if (input == "") {
        readString(input);
    }
}


// Fonction pour ajoue de nv ligne 
string makeNewData(int &choice) {
    string text_input;
    vector<string> vector_input;

    if (choice == 0 || choice == 1) {

        Personne personne;
        cout << "Donner <id> <name> <zone geo>:" << endl;
        readString(text_input);
        clearScreen();
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
        clearScreen();
        cout << "donner <id> <idFacteur> <idDestinataire>" << endl;
        readString(text_input);
        clearScreen();
        vector_input = splitS(text_input, " ");

        Recommande reco(stoi(vector_input[0]),type[choice],stoi(vector_input[1]),stoi(vector_input[2]));

        cout <<"ajoue valide:\n" + reco.formatForUser() + "\n" << endl;

        return reco.formatForFile();
    }

    return 0;
}
string xorCipher(const bool &decrypt, string str, const string &password) {

    if (decrypt) str = base64ToString(str);

    string encryptedString;
    for (int i = 0; i < str.length(); i++)
    {
        encryptedString += (char)(str[i] ^ password[i % password.length()]);
    }
    if (!decrypt) encryptedString = stringToBase64(encryptedString);
    
    return encryptedString;
}

// Homemade function to hash a string
string hashString(const string& str)
{
    size_t hash = 0;
    for (char c : str)
    {
        hash = hash * 31 + c;
    }
    return to_string(hash);
}

// verifier si le caractere est de base 64
static inline bool is_base64(unsigned char c) {
  return (isalnum(c) || (c == '+') || (c == '/'));
}
vector<unsigned char> stringToUnsignedChar(const string& str)
{
    std::vector<unsigned char> result;
    for (char c : str)
    {
        result.push_back(static_cast<unsigned char>(c));
    }
    return result;
}

string unsignedCharVectorToString(const vector<unsigned char>& vec)
{
    std::string str;
    for (auto c : vec)
    {
        str += c;
    }
    return str;
}
string base64Encode(unsigned char const* buf, unsigned int bufLen) {
  string ret;
  int i = 0;
  int j = 0;
  unsigned char char_array_3[3];
  unsigned char char_array_4[4];

  while (bufLen--) {
    char_array_3[i++] = *(buf++);
    if (i == 3) {
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;

      for(i = 0; (i <4) ; i++)
        ret += base64_chars[char_array_4[i]];
      i = 0;
    }
  }

  if (i)
  {
    for(j = i; j < 3; j++)
      char_array_3[j] = '\0';

    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
    char_array_4[3] = char_array_3[2] & 0x3f;

    for (j = 0; (j < i + 1); j++)
      ret += base64_chars[char_array_4[j]];

    while((i++ < 3))
      ret += '=';
  }

  return ret;
}
vector<unsigned char> base64Decode(string const& encoded_string) {
  int in_len = encoded_string.size();
  int i = 0;
  int j = 0;
  int in_ = 0;
  unsigned char char_array_4[4], char_array_3[3];
  std::vector<unsigned char> ret;

  while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
    char_array_4[i++] = encoded_string[in_]; in_++;
    if (i ==4) {
      for (i = 0; i <4; i++)
        char_array_4[i] = base64_chars.find(char_array_4[i]);

      char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
      char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

      for (i = 0; (i < 3); i++)
          ret.push_back(char_array_3[i]);
      i = 0;
    }
  }

  if (i) {
    for (j = i; j <4; j++)
      char_array_4[j] = 0;

    for (j = 0; j <4; j++)
      char_array_4[j] = base64_chars.find(char_array_4[j]);

    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
    char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
    char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

    for (j = 0; (j < i - 1); j++) ret.push_back(char_array_3[j]);
  }

  return ret;
}

// fonction simplifier pour coder une string a la base 64
string stringToBase64(const string &in) {
    vector<unsigned char> temp = stringToUnsignedChar(in);
    return base64Encode(&temp[0], temp.size());
}

// fonction simplifier aussi pour decoderune string au base 64
string base64ToString(const string &in) {
    return unsignedCharVectorToString(base64Decode(in));
}
