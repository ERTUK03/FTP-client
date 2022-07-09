#include "TransferConnection.h"

void TransferConnection::read()
{
	sock.async_read_some(boost::asio::buffer(buffer), [&](std::error_code error, size_t bytes_transferred)
		{
			if (!error)
			{
				for (int i = 0; i < bytes_transferred; i++) std::cout << buffer[i];
				read();
			}
		}
	);
}

TransferConnection::TransferConnection(std::string address, int port, boost::asio::io_context& con) : sock{ con }, buffer(1024)
{
	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::make_address(address), port);
	sock.connect(endpoint);

	if (!sock.is_open())
	{
		std::cout << "Couldn't establish a transfer connection";
		return;
	}
	read();
}