#pragma once

// Frontend
#include "frontend/mainframe.h" // MainFrame class

// STD
#include <string>

class DataStore {
public:
    explicit DataStore(const std::string& dbPath);
    ~DataStore();

    
    // Exporting
    void ExportTableToJSON(const std::string& tableName, const std::string& outputFilename);
    void ExportTableToCSV(const std::string& tableName, const std::string& outputFilename);
    void ExportTOQRCode(const std::string& content, const std::string& outputFilename);

    // Importing
    void ImportTableFromCSV(const std::string& tableName, const std::string& inputFilename);
private:
    void Connect(); // Connect to the SQL database
    void Disconnect(); // Disconnect from the SQL database

    bool TableExists(const std::string& tableName); // check if an SQL table exists

    sqlite3* m_db; // SQL database
    const std::string m_dbPath; // Path to the .db file. Set when DataBase is constructed
    bool m_connected; // If the database is connected
};
