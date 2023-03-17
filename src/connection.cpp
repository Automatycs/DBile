#include "../include/connection.hpp"

void Connection::connect_user() {
    string tmp = "";

    if (this->user == "") {
        cout << "Warning:       Aucun utilisateur précisé, création impossible" << endl;
        return;
    }
    if (this->password == "") {
        cout << "Warning:       Aucun mot de passe précisé, création impossible" << endl;
        return;
    }
    
    this->i_flux.open(this->userfile);
    for (string flux_input; getline(this->i_flux, flux_input);) {
        tmp = flux_input.substr(0, flux_input.find(','));
        flux_input.erase(0, flux_input.find(',') + 1);
        if (tmp == this->user && flux_input == this->password) {
            cout << "Connexion réussie:     Bienvenue " << this->user << endl;
            return;
        }
    }
    this->i_flux.close();

    cout << "Erreur:        Aucun profil correspondant aux infomations données n'a été trouvé, veuillez réessayer" << endl;
}

void Connection::create_user() {
    string tmp = "";

    if (this->user == "") {
        cout << "Warning:       Aucun utilisateur précisé, création impossible" << endl;
        return;
    }
    if (this->password == "") {
        cout << "Warning:       Aucun mot de passe précisé, création impossible" << endl;
        return;
    }
    
    this->i_flux.open(this->userfile);
    for (string flux_input; getline(this->i_flux, flux_input);) {
        tmp = flux_input.substr(0, flux_input.find(','));
        if (tmp == this->user) {
            cout << "Warning:       L'utilisateur " << tmp << " existe déjà!" << endl;
            return;
        }
    }
    this->i_flux.close();

    this->o_flux.open(this->userfile, fstream::app);
    this->o_flux << this->user << ',' << this->password << endl;
    this->o_flux.close();
    return;
}

void Connection::show() {
    cout << "user = " << this->user << endl
         << "password = " << this->password << endl
         << "sql_args = " << this->sql_args << endl
         << "format = " << this->format << endl
         << "login state = " << this->login << endl
         << "new user state = " << this->new_user << endl
         << "help state = " << this->help << endl
         << "version state = " << this->version << endl;
}

void Connection::show_version() {
    cout << "DBile      Version 1.0.0       by      Hélie Lévy de Mareuil" << endl
         << "                                       Frédéric Gabriel" << endl;
}

void Connection::show_help() {
    cout << "DBile help:" << endl
         << "-n             --new                  Indique la création d'un nouvel utilisateur" << endl
         << "-h             --help                 Affiche l'aide" << endl
         << "-v             --version              Affiche la version de l'application"
         << "-l             --login                Permet de tester la connection" << endl
         << "-u ...         --user ...             Permet d'indiquer le nom d'utilisateur" << endl
         << "-p ...         --pasword ...          Permet d'indiquer le mot de passe de l'utilisateur" << endl
         << "-s ...         --sql ...              Permet de donner une ligne de commande SQL" << endl
         << "-f ...         --format ...           Permet d'indiquer le format de sortie. Options: [csv, json, visual_tab]" << endl;
}

bool Connection::is_valid(string str) {
    for (long unsigned int i = 0; i < str.length(); i += 1) {
        if ((str[i] < 'a' || str[i] > 'z') && (str[i] < 'A' || str[i] > 'Z') && (str[i] < '0' || str[i] > '9'))
            return (false);
    }
    return (true);
}

void Connection::case_new(string str) {
    this->new_user = true;
}

void Connection::case_help(string str) {
    this->help = true;
}

void Connection::case_version(string str) {
    this->version = true;
}

void Connection::case_login(string str) {
    this->login = true;
}

void Connection::case_user(string str) {
    if (is_valid(str)) {
        this->user = str;
    } else
        this->user = "";
}

void Connection::case_password(string str) {
    if (is_valid(str))
        this->password = str;
    else
        this->password = "";
}

void Connection::case_sql(string str) {
    this->sql_args = str;
}

void Connection::case_format(string str) {
    if (str == "csv")
        this->format = "csv";
    if (str == "json")
        this->format = "json";
    if (str == "visual_tab")
        this->format = "visual_tab";
}

int Connection::connect(int size, char **args) {

    for (int i = 1; i < size; i += 1) {
        for (int j = 0; j < 8; j += 1) {
            if (args[i] == flags[j] || args[i] == l_flags[j]) {
                if (i + 1 < size)
                    (this->*(flags_fonction[j]))(args[i + 1]);
                else
                    (this->*(flags_fonction[j]))("");
                break;
            }
        }
    }

    if (this->version) {
        show_version();
        return (0);
    }
    if (this->help) {
        show_help();
        return (0);
    }
    if (this->new_user) {
        create_user();
    }
    if (this->login) {
        connect_user();
    }
    return (0);
}

int main(int ac, char **av) {
    Connection pouet;

    pouet.connect(ac, av);
    return (0);
}
