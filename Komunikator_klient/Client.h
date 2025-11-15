#pragma once
#include<iostream>
#include<asio.hpp>


class Client {
private:
	int port_;
	void handle_connect();
	void handle_read();
	void handle_write();
public:
	Client(int port);
	void connect(int port);
	void async_read();
	void async_write(std::string message);
};
