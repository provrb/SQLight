// Backend
#include "backend/data_store.hxx"

// STD
#include <filesystem>

DataStore::DataStore(const std::string& dbPath) {
    this->Connect(dbPath);
}

DataStore::~DataStore() {
    this->Disconnect();
}

bool DataStore::Connect(const std::string& dbPath) {
    // Ensure we are not already connected and that
    // the path to the .db file actually exists
    if ( this->m_connected || !std::filesystem::exists(dbPath) )
        return false;

    // sqlite3_open returns SQLITE_OK on success
    return sqlite3_open(dbPath.c_str(), &this->m_db) == SQLITE_OK;
}

bool DataStore::Disconnect() {
    // Ensure we are connected and m_db is not null
    if ( !this->m_connected || !this->m_db )
        return false;

    return sqlite3_close(this->m_db) == SQLITE_OK;
}

void DataStore::ExportTableToJSON(
    const std::string& tableName, 
    const std::string& outputFilename
)
{

}

void DataStore::ExportTableToCSV(
    const std::string& tableName, 
    const std::string& outputFilename
) 
{

}

bool DataStore::TableExists(const std::string& tableName) {
    std::string query = "SELECT name FROM sqlite_master WHERE type='table' AND name='" + tableName + "';";
    sqlite3_stmt* stmt;

    int res = sqlite3_prepare_v2(m_db, query.c_str(), -1, &stmt, NULL);
    if ( res != SQLITE_OK )
        return false;

    // No rows with name 'tableName', means 
    // table doesn't exist.
    if ( sqlite3_step(stmt) != SQLITE_ROW ) {
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}
