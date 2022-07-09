#pragma once

#include <cctype>
#include <regex>
#include "TransferConnection.h"

class Client
{
private:
    boost::asio::ip::tcp::socket sock;
	boost::system::error_code error;
    boost::asio::streambuf buffer;

    struct Data
    {
        std::string address;
        int port;
        Data(std::vector<int> vector)
        {
            address = std::to_string(vector[0]) + "." + std::to_string(vector[1]) + "." + std::to_string(vector[2]) + "." + std::to_string(vector[3]);
            port = (vector[4] * 256) + vector[5];
        }
    };

    std::string parseLine();
    void extractFromLine(std::vector<int>& destVector, std::string line);
    void read();

public:
    Client(std::string address, std::string port, boost::asio::io_context& con);
    void write(std::string request);
};

