#include"Client.h"

Client::Client(asio::io_context& io)
	:socket_(io)
{
	
}

void Client::handle_connect(const asio::error_code& error)
{
	if (!error) {
		std::cout << "server polaczony" << std::endl;

		async_read();
	}
}


void Client::connect(int port) {
	auto self = shared_from_this();
	socket_.async_connect(asio::ip::tcp::endpoint(asio::ip::make_address("127.0.0.1"), port),
		[this, self](const asio::error_code& error) {
			handle_connect(error);
		}
		);

}

void Client::handle_read(const asio::error_code& error, std::size_t bytes_transformed) {
	if (!error) {
		std::istream stream(&buffer_);
		std::string wiadomosc;
		std::getline(stream, wiadomosc);

		std::cout << wiadomosc << std::endl;

		async_read();
	}
}


void Client::async_read() {
	auto self = shared_from_this();
	asio::async_read_until(socket_, buffer_, std::string("\n"),
		[this,self](const asio::error_code& error, std::size_t bytes_transformed) {
			handle_read(error,bytes_transformed);
		}
		);
}

void Client::handle_write(const asio::error_code& error, std::size_t bytes_transformed) {
	if (error) {
		std::cout << "blad" << error.message() << std::endl;
	}
}


void Client::async_write(const std::string& message) {
	auto shared_message = std::make_shared<std::string>(message + "\n");
	auto self = shared_from_this();
	asio::async_write(socket_, asio::buffer(*shared_message),
		[this, self, shared_message](const asio::error_code& error, std::size_t bytes_transformed) {
			handle_write(error, bytes_transformed);
		}
		);
}

void Client::close() {
	socket_.close();
}