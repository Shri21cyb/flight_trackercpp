#  Flight Booking System (C++)

This project is a **console-based flight booking system** implemented in C++ using object-oriented programming. It allows users to add flights, book tickets for passengers, list current bookings, and export booking data to a CSV file for persistence.

## Features

- Add new flights with a unique flight number and seat capacity
- Book flights for passengers with automatic seat availability check
- List all existing bookings
- Export booking data to a CSV file (`bookings.csv`)
- Automatically reload bookings from the CSV file (if present) on startup

## Classes

### `Passenger`
Represents a passenger with:
- `name`
- `id`

Includes methods for:
- Converting to/from CSV strings

### `Flight`
Represents a flight with:
- `flightNumber`
- `seats` (total capacity)
- `booked` (seats already booked)

Includes:
- Seat availability checks
- Booking a seat
- Conversion to CSV format

### `Booking`
Handles:
- Booking logic
- Flight and passenger storage
- CSV file export and import
- In-memory management using `vector`

##  How It Works

When the program starts, it:
- Loads previous bookings (if `bookings.csv` exists)
- Adds some sample flights
- Prompts the user with a menu to:
  1. Add a new flight
  2. Book a flight for a passenger
  3. Display all bookings
  4. Export to CSV
  5. Exit

##  Sample CSV Output Format

```csv
PassengerName,PassengerID,FlightNumber
sam	23325	444
mary	4242	333
milly	3252	333
![image](https://github.com/user-attachments/assets/66ec718a-e20b-4178-98be-a704b73400c5)

