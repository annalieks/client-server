#ifndef REQUEST_MANAGER_HPP
#define REQUEST_MANAGER_HPP

#include <utility>

#include "data_file.hpp"
#include "request.hpp"
#include "response.hpp"

class RequestManager {
private:
    std::shared_ptr<DataFile> file;

public:
    explicit RequestManager(std::shared_ptr<DataFile>  file)
    : file(std::move(file)) { }

    HTTP::Request request;

    HTTP::Response executeRequest();
    HTTP::Response handlePostRequest();
    HTTP::Response handlePutRequest();
    HTTP::Response handleGetRequest();
    HTTP::Response handleDeleteRequest();
    static HTTP::Response handleUnimplemented();
};


#endif //REQUEST_MANAGER_HPP
