#include "../include/sql.hpp"
#include "../include/Table.hpp"
#include "../include/Database.hpp"

bool SQL::check_current_db()
{
    return Database::find_db(current_db) != nullptr;
}

string SQL::get_current()
{
    return (this->current_db);
}

string SQL::clean_sql(string str)
{
    string to_clean;
    string to_small_one;
    string to_small_two;
    int pos_start = str.find('(');
    int pos_end = str.find(')');

    if (pos_end < pos_start)
        return ("");

    if (pos_start == pos_end)
    {
        for (long unsigned int i = 0; i < str.length(); i += 1)
            str[i] = tolower(str[i]);
        return (str);
    }

    to_small_one = str.substr(0, pos_start);
    str.erase(0, pos_start);
    to_clean = str.substr(0, str.find(')'));
    str.erase(0, str.find(')'));
    to_small_two = str;

    for (long unsigned int i = 0; i < to_small_one.length(); i += 1)
        to_small_one[i] = tolower(to_small_one[i]);
    for (long unsigned int i = 0; i < to_small_two.length(); i += 1)
        to_small_two[i] = tolower(to_small_two[i]);
    to_clean.erase(remove(to_clean.begin(), to_clean.end(), ' '), to_clean.end());

    str = to_small_one + to_clean + to_small_two;
    return (str);
}

void SQL::parse_sql(string str)
{
    this->current_args.clear();
    str = clean_sql(str);

    for (; str.find(' ') != string::npos;)
    {
        this->current_args.push_back(str.substr(0, str.find(' ')));
        str.erase(0, str.find(' ') + 1);
    }
    this->current_args.push_back(str);
}

bool SQL::is_good_values(string str)
{
    const size_t seek_open = str.find('(');
    const size_t seek_close = str.find(')');

    if (seek_open == string::npos || seek_close == string::npos)
        return (false);
    return (true);
}

vector<string> SQL::parse_parentheses(const string &str)
{
    string columns_raw = str;
    vector<string> columns;

    columns_raw = columns_raw.substr(1, columns_raw.size() - 2); // pop first and last parentheses

    for (; columns_raw.find(',') != string::npos;)
    {
        columns.push_back(columns_raw.substr(0, columns_raw.find(',')));
        columns_raw.erase(0, columns_raw.find(',') + 1);
    }
    columns.push_back(columns_raw); // add text between last comma and endof string

    // for (string c : columns) cout << c;
    return columns;
}

void SQL::case_create()
{
    if (this->current_args.size() < 3 || this->current_args.size() > 4)
    {
        cout << "Erreur:        La commande donnée n'est pas valide" << endl;
        return;
    }
    if (this->current_args[1] != "database" && this->current_args[1] != "table")
    {
        cout << "Erreur:        La commande donnée n'est pas valide" << endl;
        return;
    }

    if (this->current_args[1] == "database")
    {
        if (this->current_args.size() != 3)
        {
            cout << "Erreur:        La commande donnée n'est pas valide" << endl;
            return;
        }
        else
        {

            Database *d = new Database(current_args[2]);
            cout << "create db" << endl;
        }
    }
    else
    {
        if (this->current_args.size() != 4)
        {
            cout << "Erreur:        La commande donnée n'est pas valide" << endl;
            return;
        }
        else if (!is_good_values(this->current_args[3]))
        {
            cout << "Erreur:        La commande donnée n'est pas valide" << endl;
            return;
        }
        else
        {
            Database *db = Database::find_db(current_db);
            if (db)
            {
                /*
                JE CREER UNE TABLE
                nom de table = current_args[2]
                values = current_args[3] (à pars)
                */

                cout << "CREATING: " << current_args[3];

                Database *db = Database::find_db(current_db);

                Table *tb = new Table(current_args[2], *db, parse_parentheses(current_args[3]));

                return;
            }
            cout << "Erreur, pas de base de données sélectionnée.";
        }
    }
}

void SQL::case_show()
{
    if (this->current_args.size() != 2)
    {
        cout << "Erreur:        La commande donnée n'est pas valide" << endl;
        return;
    }
    if (this->current_args[1] != "databases" && this->current_args[1] != "tables")
    {
        cout << "Erreur:        La commande donnée n'est pas valide" << endl;
        return;
    }

    if (this->current_args[1] == "databases")
    {

        Database::show_databases();
    }
    else
    {
        if (Database::find_db(current_db) != nullptr)
        {
            Database::find_db(current_db)->show_tables();
            return;
        }
        cout << "Aucune base de données valide sélectionnée";
    }
}

void SQL::case_use()
{
    if (this->current_args.size() != 2)
    {
        cout << "Erreur:        La commande donnée n'est pas valide" << endl;
        return;
    }

    /*
    JE PASSE SUR LA DATABASE CHOISIE
    nom de la database = current_args[1]
    */
    if (Database::find_db(current_args[1]) != nullptr)
    {
        cout << "USING " << current_args[1] << endl;
        current_db = current_args[1];
        return;
    }
        cout << "Erreur : Base de donnée non trouvée" << endl;

}

void SQL::case_describe()
{
    if (this->current_args.size() != 3)
    {
        cout << "Erreur:        La commande donnée n'est pas valide" << endl;
        return;
    }
    if (this->current_args[1] != "table")
    {
        cout << "Erreur:        La commande donnée n'est pas valide" << endl;
        return;
    }

    /*
    JE DECRIS LA TABLE CHOISIE
    nom de la table = current_args[2]
    */
    if (Database::find_db(current_db) == nullptr)
    {
        cout << "Erreur, pas de base de données sélectionnée." << endl;
        return;
    }
    Table *temp = Database::find_db(current_db)->find_table(current_args[2]);
    if (temp)
    {
        temp->describe();
        return;
    } // check table found
    cout << "Erreur, table non trouvée" << endl;
}

void SQL::case_insert()
{
    if (this->current_args.size() != 5)
    {
        cout << "Erreur:        La commande donnée n'est pas valide" << endl;
        return;
    }
    if (this->current_args[1] != "into" || this->current_args[3] != "values")
    {
        cout << "Erreur:        La commande donnée n'est pas valide" << endl;
        return;
    }
    if (!is_good_values(this->current_args[4]))
    {
        cout << "Erreur:        La commande donnée n'est pas valide" << endl;
        return;
    }

    if (Database::find_db(current_db) != nullptr){
        
        Table *t = Database::find_db(current_db)->find_table(current_args[2]);
        if (t) { // check its not nullptr (found)
            t->insert(parse_parentheses(current_args[4]));
            return;
        }
        cout << "La table "<< current_args[2] << " n'a pas été trouvée" << endl ;
        return;

    }
    
    cout << "Erreur, pas de base de données sélectionnée." << endl;

    /*
    J'INSERT DES DONNEES DANS LA TABLE
    nom de la table = current_args[2]
    argument à insert = current_args[4] (à pars)
    */
}

void SQL::case_select()
{
    if (this->current_args.size() != 4)
    {
        cout << "Erreur:        La commande donnée n'est pas valide" << endl;
        return;
    }
    if (this->current_args[2] != "from")
    {
        cout << "Erreur:        La commande donnée n'est pas valide" << endl;
        return;
    }

    /*
    JE SELECT DES DONNEES DANS LA TABLE
    value que l'on select = current_args[1]
    table que l'on fouille = current_args[3]
    */    
    if (Database::find_db(current_db) != nullptr){
        
        Table *t = Database::find_db(current_db)->find_table(current_args[3]);
        if (t) { // check its not nullptr (found)
            t->select(parse_parentheses(current_args[1]));
            return;
        }
        cout << "La table "<< current_args[2] << "n'a pas été trouvée" << endl ;

    }
}

void SQL::choose_action(string str)
{
    parse_sql(str);

    if (this->current_args.size() == 0)
    {
        cout << "Erreur:        La commande donnée n'est pas valide" << endl;
    }
    for (int i = 0; i < 6; i += 1)
    {
        if (this->current_args[0] == keywords[i])
        {
            (this->*(keywords_function[i]))();
            return;
        }
    }
    cout << "Erreur:        La commande donnée n'est pas valide" << endl;
}