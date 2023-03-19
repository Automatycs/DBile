#include "../include/Table.hpp"
#include "../include/Utils.hpp"
#include <sstream>

Table::Table(string nm, Database &dbx, vector<string> columns) : name(nm), db(dbx), cols(columns)
{

    dbx.tables.push_back(this);
    string folders = "db/" + dbx.name + "/";
    file_addr = string(folders + nm + ".csv");
    // create if they don't exist
    system(string("mkdir -p " + folders).c_str());

    ofstream file;
    file.open(file_addr, ios::out);
    // TODO: Insert username to check privileges.
    file << Utils::csvify(cols) << endl;
    file.close();
}

void Table::describe() const
{
    cout << "DESCRIBING " << name << endl;
    for (string col : cols)
    {
        cout << col << endl;
    }
}

void Table::insert(const vector<string> &values) const
{
    ofstream file;
    file.open(file_addr, ios::app);
    file << Utils::csvify(values) << endl;
    file.close();
}

void Table::select(vector<string> query_cols) const
{
    
    cout << "SELECT * FROM " << name << endl;
    vector<vector<string>> data = read_all();
    vector<vector<string>> new_data;

    for (string query_col : query_cols)
    {
        if (query_col == "*")
        {
            // Cancel all, we want everything
            data = read_all();
            break;
        }

        for (int col_id = 0; col_id < cols.size(); col_id++)
        {
            if (cols[col_id] == query_col)
            {   
            
                // for each line of the data, add the value at right column.
                for (int line_id = 0; line_id < data.size(); line_id++)
                {
                    cout << "value" << data[line_id][col_id]<< endl;
                    new_data.push_back(vector<string>()); // newline
                    new_data[line_id].push_back(data[line_id][col_id]);
                }
            }
        }

    }

    display_select(new_data);
    cout << "--- END SELECT" << endl;
}

void Table::display_select(vector<vector<string>> data) const
{

    for (int i = 0; i < data.size(); i++)
    {
        for (int j = 0; j < data[i].size(); j++)
        {
            cout << data[i][j] << " ";
        }
        cout << "\n";
    }
}

vector<vector<string>> Table::read_all() const
{

    ifstream file;
    file.open(file_addr);

    vector<vector<string>> content;
    vector<string> row;
    string line, word;

    if (file.is_open())
    {
        while (getline(file, line))
        {
            row.clear();

            stringstream str = stringstream(line);

            while (getline(str, word, ','))
                row.push_back(word);
            content.push_back(row);
        }
    }
    return content;
}