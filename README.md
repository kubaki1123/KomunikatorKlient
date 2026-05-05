"""# Asynchroniczny Komunikator — Klient (C++/Asio)

> Konsolowy klient TCP z asynchronicznym odbieraniem wiadomości i jednoczesnym wczytywaniem danych z klawiatury.

> 🔗 Powiązane repozytorium: [KomunikatorServer](https://github.com/kubaki1123/KomunikatorServer)

---

## Opis projektu

Klient komunikatora rozwiązuje klasyczny problem asynchronicznego I/O: jak jednocześnie **czekać na wiadomości z serwera** i **wczytywać dane z klawiatury**,
nie blokując żadnej z operacji. Rozwiązaniem jest uruchomienie logiki sieciowej Asio w dedykowanym wątku roboczym (`std::thread`), 
mpodczas gdy główny wątek obsługuje konsolę.

```
Wątek główny:    [wczytaj z konsoli] → [wyślij do serwera]
Wątek sieciowy:  [async_read] → [wyświetl odebraną wiadomość]
```

---

## Funkcje

- **Asynchroniczny odbiór** — wiadomości od innych użytkowników wyświetlają się natychmiast, nie przerywając wpisywania
- **Wysyłanie wiadomości prywatnych** — format `NICK:treść`
- **Broadcast** — format `ALL:treść`
- **Logowanie** — pierwsze wejście po połączeniu = podanie nicku
- **Graceful disconnect** — poprawne zamknięcie połączenia przy wyjściu (`Ctrl+C` lub pusty input)

---

## Wymagania

- Kompilator C++17 lub nowszy
- [Standalone Asio](https://think-async.com/Asio/) (header-only, bez Boost)
- CMake 3.14+
- Działający serwer: [KomunikatorServer](https://github.com/kubaki1123/KomunikatorServer)

---

## Kompilacja

```bash
git clone https://github.com/kubaki1123/KomunikatorKlient.git
cd KomunikatorKlient
git submodule init
mkdir build && cd build
cmake ..
cmake --build .
```

---

## Uruchomienie

Najpierw uruchom serwer ([KomunikatorServer](https://github.com/kubaki1123/KomunikatorServer)), następnie klienta:

```bash
./KomunikatorKlient
# Podaj adres serwera (domyślnie: 127.0.0.1:8080)
```


---

## Architektura

```
KomunikatorKlient/
├── main.cpp        # Punkt wejścia, połączenie TCP, uruchomienie wątku sieciowego
├── Client.h        # Deklaracja klasy Client
├── Client.cpp      # async_read_until (odbiór), async_write (wysyłanie), std::thread
└── CMakeLists.txt  # Konfiguracja budowania
```

### Kluczowy problem i rozwiązanie

Główne wyzwanie tego projektu to **jednoczesny odbiór z sieci i wczytywanie z konsoli**. Blokujące `std::cin` i asynchroniczne Asio nie mogą działać w tym samym wątku.

**Rozwiązanie — dwa wątki:**

```cpp
// Wątek sieciowy — obsługuje cały Asio
std::thread networkThread([&io_context]() {
    io_context.run();  // blokuje wątek, obsługuje async I/O
});

// Wątek główny — obsługuje konsolę
std::string line;
while (std::getline(std::cin, line)) {
    client.send(line);  // thread-safe zapis do kolejki wysyłania
}
```

---

## Użyte technologie

| Technologia | Zastosowanie |
|---|---|
| C++17/20 | Język implementacji |
| Asio (standalone) | TCP socket, `async_read_until`, `async_write` |
| `std::thread` | Izolacja logiki sieciowej od konsoli |
| `std::deque` | Kolejka wiadomości do wysłania (thread-safe zapis) |
| CMake | System budowania |

---
