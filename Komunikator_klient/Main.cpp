#include<iostream>
#include<asio.hpp>
//#include <QApplication>
#include<thread>
#include<string>
#include"Client.h"

int main() {
	try {
		std::cout << "witaj w komunikatorze stronie dla klienta twoja pierwsza wyslana wiadomosc to bedzie twoj nick pod jakim inni moga wysylac do ciebie wiadomosci!" << std::endl;
		std::cout << "jezeli chcesz wyslac wiadomosc przykladowo do boba musisz napisac jego nick. Bob:[wiadomosc] a jezeli wyslac wiadomosc do wszystkich to All:[wiadomosc]" << std::endl;
		std::cout << "jezeli chcesz dostac informacje o po³¹czonych uzytkownikach napisz INFO" << std::endl;
		std::cout << "jezeli chcesz zakonczyc program wpisz exit" << std::endl;
		const int port = 8080;
		asio::io_context io;

		auto client = std::make_shared<Client>(io);
		client->connect(port);

		std::thread asio_thread([&io]() {
			try {
				io.run();
			}
			catch (std::exception& e) {
				std::cerr << "Blad watku Asio: " << e.what() << std::endl;
			}
			});

		std::string line;
		while (std::getline(std::cin, line)) {
			if (line == "exit") {
				client->close();
				io.stop();
				break;
			}
			client->async_write(line);
		}

		if (asio_thread.joinable()) {
			asio_thread.join();
		}
	}
	catch (std::exception& e) {
		std::cerr << "wystapil blad:" << e.what() << std::endl;
	}

	return 0;
}