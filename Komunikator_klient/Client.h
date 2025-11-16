#pragma once
#include<iostream>
#include<asio.hpp>
#include<memory>

class Client: public std::enable_shared_from_this<Client>{
private:
	
	asio::ip::tcp::socket socket_;
	asio::streambuf buffer_;
	void handle_read(const asio::error_code& error, std::size_t bytes_transformed);
	void handle_write(const asio::error_code& error, std::size_t bytes_transformed);
	void handle_connect(const asio::error_code& error);
public:
	Client(asio::io_context& io);
	void async_read();
	void async_write(const std::string& message);
	void connect(int port);
	void close();
};
