#include "Client.h"

std::string Client::parseLine()
{
    std::istream is(&buffer);
    std::string line;
    std::getline(is, line);
    line.pop_back();
    line.push_back('\n');
    return line;
}

void Client::extractFromLine(std::vector<int>& destVector, std::string line)
{
    for (int i = 3; i < line.size(); i++)
    {
        int num = 0;
        for (; isdigit(line[i]); i++)
        {
            num *= 10;
            num += line[i] - '0';
        }
        if (num) destVector.push_back(num);
    }
}

void Client::read()
{
    boost::asio::async_read_until(sock, buffer, "\r\n", [&](std::error_code error, size_t bytes_transferred)
        {
            if (!error)
            {
                std::string line = parseLine();
                std::cout.write(line.c_str(), line.size());
                if (std::regex_search(line, std::regex("^227")))
                {
                    std::vector<int> data;
                    extractFromLine(data, line);
                    Data transferData(data);
                    boost::asio::io_context newCon;
                    TransferConnection newConnection(transferData.address, transferData.port, newCon);
                    newCon.run();
                }
            }
            read();
        }
    );
}

Client::Client(std::string address, std::string port, boost::asio::io_context& con) : sock{ con }
{
    boost::asio::ip::tcp::resolver::query qry(address, port);
    boost::asio::ip::tcp::resolver res(con);
    boost::asio::ip::tcp::resolver::iterator iter = res.resolve(qry, error);

    if (error)
    {
        std::cout << "There was an error with resolver";
        return;
    }

    boost::asio::ip::tcp::endpoint endpoint = iter->endpoint();
    sock.connect(endpoint);

    if (!sock.is_open())
    {
        std::cout << "Couldn't connect";
        return;
    }
    read();
}

void Client::write(std::string request)
{
    request += "\r\n";
    sock.write_some(boost::asio::buffer(request, request.size()));
}