#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>

using namespace std;

class Passenger {
public:
    string name;
    string id;

    Passenger(const string& n, const string& i) : name(n), id(i) {}

    string toString() const {
        return name + "," + id;
    }

    static Passenger fromString(const string& line) {
        size_t pos = line.find(',');
        if (pos == string::npos) throw runtime_error("Invalid passenger format in CSV");
        string n = line.substr(0, pos);
        string i = line.substr(pos + 1);
        return Passenger(n, i);
    }
};

class Flight {
public:
    string flightNumber;
    int seats;
    int booked;

    Flight(const string& num, int s) : flightNumber(num), seats(s), booked(0) {}

    bool isFull() const { return booked >= seats; }

    void bookSeat() {
        if (isFull()) throw runtime_error("Flight " + flightNumber + " is overbooked!");
        booked++;
    }

    string toString() const {
        return flightNumber + "," + to_string(seats) + "," + to_string(booked);
    }
};

class Booking {
private:
    vector<Flight> flights;
    vector<pair<Passenger, Flight*>> bookings;
    string csvFile = "bookings.csv";

    void loadFromCSV() {
        ifstream in(csvFile);
        if (!in) return; // No file yet, skip loading
        string line;
        getline(in, line); // Skip header
        while (getline(in, line)) {
            if (line.empty()) continue;
            size_t pos1 = line.find(',');
            size_t pos2 = line.find(',', pos1 + 1);
            if (pos1 == string::npos || pos2 == string::npos) continue; // Skip malformed lines
            string name = line.substr(0, pos1);
            string id = line.substr(pos1 + 1, pos2 - pos1 - 1);
            string flightNum = line.substr(pos2 + 1);
            Passenger passenger(name, id);
            bool booked = false;
            for (auto& flight : flights) {
                if (flight.flightNumber == flightNum) {
                    try {
                        flight.bookSeat();
                        bookings.emplace_back(passenger, &flight);
                        booked = true;
                        break;
                    } catch (const runtime_error&) {
                        cerr << "Warning: Could not reload booking for " << name << " on " << flightNum << endl;
                    }
                }
            }
            if (!booked) {
                cerr << "Warning: Flight " << flightNum << " not found for " << name << endl;
            }
        }
        in.close();
    }

    string escapeCSV(const string& field) const {
        string result = field;
        if (field.find(',') != string::npos) {
            result = "\"" + field + "\"";
        }
        return result;
    }

public:
    Booking() {
        loadFromCSV(); // Load existing bookings on startup
    }

    void addFlight(const Flight& flight) {
        flights.push_back(flight);
    }

    bool bookFlight(const Passenger& passenger, const string& flightNumber) {
        try {
            for (auto& flight : flights) {
                if (flight.flightNumber == flightNumber) {
                    flight.bookSeat();
                    bookings.emplace_back(passenger, &flight);
                    return true;
                }
            }
            throw runtime_error("Flight " + flightNumber + " not found!");
        } catch (const runtime_error& e) {
            cerr << "Error: " << e.what() << endl;
            return false;
        }
    }

    void listBookings() const {
        for (size_t i = 0; i < bookings.size(); ++i) {
            auto& [passenger, flight] = bookings[i];
            cout << i + 1 << ". Passenger: " << passenger.name
                 << ", ID: " << passenger.id
                 << ", Flight: " << flight->flightNumber << endl;
        }
    }

    bool exportToCSV() const {
        ofstream out(csvFile);
        if (!out) {
            cerr << "Error: Failed to open '" << csvFile << "' for writing. Check permissions, if file is in use, or directory path." << endl;
            return false;
        }
        out << "PassengerName,PassengerID,FlightNumber\n";
        for (const auto& [passenger, flight] : bookings) {
            out << escapeCSV(passenger.name) << "," << escapeCSV(passenger.id) << "," << escapeCSV(flight->flightNumber) << "\n";
        }
        out.close();
        if (out.fail()) {
            cerr << "Error: Failed to write to '" << csvFile << "'. Check disk space, permissions, or directory path." << endl;
            return false;
        }
        cout << "Bookings exported to " << csvFile << " successfully at " << __TIME__ << " on " << __DATE__ << "\n";
        return true;
    }
};

int main() {
    Booking system;
    try {
        // Sample flights
        system.addFlight(Flight("AA123", 2));
        system.addFlight(Flight("BB456", 3));

        while (true) {
            cout << "\n1. Add Flight\n2. Book Flight\n3. List Bookings\n4. Export to CSV\n5. Exit\nChoose: ";
            int choice;
            cin >> choice;
            cin.ignore();

            if (choice == 1) {
                string num;
                int seats;
                cout << "Flight Number: ";
                getline(cin, num);
                cout << "Seats: ";
                cin >> seats;
                cin.ignore();
                system.addFlight(Flight(num, seats));
                cout << "Flight added!\n";
            } else if (choice == 2) {
                string name, id, flightNum;
                cout << "Passenger Name: ";
                getline(cin, name);
                cout << "Passenger ID: ";
                getline(cin, id);
                cout << "Flight Number: ";
                getline(cin, flightNum);
                if (system.bookFlight(Passenger(name, id), flightNum)) {
                    cout << "Booking confirmed!\n";
                }
            } else if (choice == 3) {
                system.listBookings();
            } else if (choice == 4) {
                system.exportToCSV();
            } else if (choice == 5) {
                break;
            }
        }
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    return 0;
}