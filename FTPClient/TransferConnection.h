#pragma once

#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <vector>

class TransferConnection
{
private:
	boost::asio::ip::tcp::socket sock;
	boost::system::error_code error;
    std::vector<char> buffer;

	void read();

public:
	TransferConnection(std::string address, int port, boost::asio::io_context& con);
};

