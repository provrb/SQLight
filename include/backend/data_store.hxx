#pragma once

// SQLite
#include "../ext/sqlite3.h"

// STD
#include <string>

class DataStore {
public:
    /*
        Automatically attempt to connect to a database iwth
        dbPath upon construction of the class.
    */
    explicit DataStore(const std::string& dbPath);
    
    /*
        Does not automatically connect when constructed. 
        Connect(const std::string&) should be called to connect to a data base.
    */
    DataStore() : m_db(nullptr), m_connected(false)
    {
    }
    
    /*
        Close data base connection, if any,
        and frees the m_db pointer, if not nullptr.
    */
    ~DataStore();

    bool Connect(const std::string& dbPath); // Connect to SQLite data base with path 'dbPath'
    const bool IsConnected() const { return m_connected; }
    bool Disconnect(); // Disconnect from the currently connected data base
    
    // Exporting
    void ExportTableToJSON(const std::string& tableName, const std::string& outputFilename);
    void ExportTableToCSV(const std::string& tableName, const std::string& outputFilename);
private:
    bool TableExists(const std::string& tableName); // check if an SQL table exists

    sqlite3* m_db; // SQL database
    const std::string m_dbPath; // Path to the .db file. Set when DataBase is constructed
    bool m_connected; // If the database is connected
};
