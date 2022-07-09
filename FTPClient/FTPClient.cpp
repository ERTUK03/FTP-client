#include "Client.h"

int main()
{
	boost::asio::io_context con;
	Client client("ftp.gnu.org", "21", con);
	client.write("USER anonymous");
	client.write("PASS anonymous");
	client.write("PASV");
	client.write("RETR README");
	con.run();
}
