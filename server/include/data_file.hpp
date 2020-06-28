#ifndef DATA_FILE_HPP
#define DATA_FILE_HPP

#include <string>
#include <fstream>
#include <unordered_map>
#include <shared_mutex>
#include <json.hpp>

#include "response.hpp"

class DataFile {
private:
    std::string path;
    std::fstream dataFile;
    std::shared_mutex mutex;
    nlohmann::json data;
    const int kIndent = 4;

    int getFileLength();
    void createFileIfNotExists();
    void openInOut();
    void openTrunc();
    void close();

public:
    DataFile();
    DataFile(DataFile& instance) = delete;

    void operator=(const DataFile& instance) = delete;

    ~DataFile();

    HTTP::Response addRecord(const std::string& key, const std::string& value);
    HTTP::Response setRecord(const std::string& key, const std::string& value);
    HTTP::Response deleteRecord(const std::string& key);
    HTTP::Response getRecord(const std::string& key);
};

#endif //DATA_FILE_HPP
