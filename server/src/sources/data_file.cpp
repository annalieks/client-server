#include <boost/filesystem.hpp>
#include <iostream>

#include "data_file.hpp"

namespace {

constexpr std::string_view alreadyExistMsg =
    "Pair with such key already exists";
constexpr std::string_view cannotOpenTheFileMsg =
    "Could not add the record. Data file cannot be opened";
constexpr std::string_view addSuccessMsg = "Pair was successfully added";
constexpr std::string_view doesNotExistMsg =
    "Pair with such key does not exist";
constexpr std::string_view setSuccessMsg = "Value was successfully set";
constexpr std::string_view deleteSuccessMsg = "Value was successfully deleted";

}  // namespace

DataFile::DataFile() : path("../data/data.json") {
  createFileIfNotExists();
  try {
    openInOut();
  } catch (std::fstream::failure &e) {
    std::cerr << e.what();
  }
  if (getFileLength() > 0) {
    dataFile >> data;
  }
}

DataFile::~DataFile() { close(); }

HTTP::Response DataFile::addRecord(const std::string &key,
                                   const std::string &value) {
  std::unique_lock<std::shared_mutex> m(mutex);
  if (data.contains(key)) {
    return HTTP::Response("409", alreadyExistMsg);
  }
  if (!dataFile.is_open()) {
    return HTTP::Response("500", cannotOpenTheFileMsg);
  }
  try {
    openTrunc();
  } catch (std::fstream::failure &e) {
    return HTTP::Response("500", e.what());
  }
  data[key] = value;
  dataFile << data.dump(kIndent) << std::flush;
  return HTTP::Response("200", addSuccessMsg);
}

HTTP::Response DataFile::setRecord(const std::string &key,
                                   const std::string &value) {
  std::unique_lock<std::shared_mutex> m(mutex);
  if (!data.contains(key)) {
    return HTTP::Response("409", doesNotExistMsg);
  }
  close();
  try {
    openTrunc();
  } catch (std::fstream::failure &e) {
    return HTTP::Response("500", e.what());
  }
  data[key] = value;
  dataFile << data.dump(kIndent) << std::flush;
  return HTTP::Response("200", setSuccessMsg);
}

HTTP::Response DataFile::getRecord(const std::string &key) {
  std::shared_lock<std::shared_mutex> m(mutex);
  if (!data.contains(key)) {
    return HTTP::Response("409", doesNotExistMsg);
  }
  if (!dataFile.is_open()) {
    return HTTP::Response("500", cannotOpenTheFileMsg);
  }
  nlohmann::json answer;
  answer[key] = data[key];
  auto response = HTTP::Response("200", answer.dump(kIndent));
  response.setHeader("Content-Type", "application/json");
  return response;
}

HTTP::Response DataFile::deleteRecord(const std::string &key) {
  std::unique_lock<std::shared_mutex> m(mutex);
  if (!data.contains(key)) {
    return HTTP::Response("409", doesNotExistMsg);
  }
  close();
  try {
    openTrunc();
  } catch (std::fstream::failure &e) {
    return HTTP::Response("500", e.what());
  }
  data.erase(key);
  dataFile << data.dump(kIndent) << std::flush;
  return HTTP::Response("200", deleteSuccessMsg);
}

int DataFile::getFileLength() {
  if (!dataFile.is_open()) {
    return 0;
  }
  dataFile.seekg(0, std::fstream::end);
  int length = dataFile.tellg();
  dataFile.seekg(0, std::fstream::beg);
  return length;
}

void DataFile::createFileIfNotExists() {
  try {
    if (!boost::filesystem::exists(path)) {
      std::shared_lock<std::shared_mutex> m(mutex);
      dataFile.open(path, std::fstream::app);
      dataFile.close();
    }
  } catch (const std::fstream::failure &e) {
    std::cerr << e.what();
  }
}

void DataFile::openInOut() {
  if (dataFile.is_open()) {
    dataFile.close();
  }
  try {
    dataFile.open(path, std::fstream::in | std::fstream::out);
  } catch (std::fstream::failure &e) {
    throw e;
  }
}

void DataFile::openTrunc() {
  if (dataFile.is_open()) {
    dataFile.close();
  }
  try {
    dataFile.open(path,
                  std::fstream::trunc | std::fstream::in | std::fstream::out);
  } catch (std::fstream::failure &e) {
    throw e;
  }
}

void DataFile::close() {
  if (dataFile.is_open()) {
    dataFile.close();
  }
}
