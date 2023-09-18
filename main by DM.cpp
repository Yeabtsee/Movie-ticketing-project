#include <iostream>
#include <string>
#include <fstream>

using namespace std;

const int MAX_MOVIES = 10;
const int MAX_SEATS = 10;

// Structure to hold movie details
struct Movie {
    string title;
    string showtime;
    double price;
};

// Structure to hold seat status
struct SeatStatus {
    int movieIndex;
    int seatNumber;
    bool isReserved;
};

// Function to initialize movie data
void initializeMovies(Movie movies[], int numMovies) {
    // Add movie data as needed
    movies[0] = { "Barbie", "10:00 AM", 10.0 };
    movies[1] = { "Oppenheimer", "1:00 PM", 12.0 };
    movies[2] = { "Insidious: The Red Door", "4:00 PM", 15.0 };
}

// Function to display available movies and their details
void displayMovies(const Movie movies[], int numMovies) {
    cout << "Available Movies:" << endl;
    for (int i = 0; i < numMovies; i++) {
        cout << i + 1 << ". " << movies[i].title << " (" << movies[i].showtime << ")" << endl;
        cout << "   Price: $" << movies[i].price << endl;
    }
}

// Function to read seat reservation status from a file
void readSeatStatus(SeatStatus seatStatus[][MAX_SEATS], int numMovies) {
    ifstream inputFile("seat_status.txt");
    if (inputFile.is_open()) {
        for (int i = 0; i < numMovies; i++) {
            for (int j = 0; j < MAX_SEATS; j++) {
                inputFile >> seatStatus[i][j].isReserved;
            }
        }
        inputFile.close();
    }
}

// Function to write seat reservation status to a file
void writeSeatStatus(const SeatStatus seatStatus[][MAX_SEATS], int numMovies) {
    ofstream outputFile("seat_status.txt");
    if (outputFile.is_open()) {
        for (int i = 0; i < numMovies; i++) {
            for (int j = 0; j < MAX_SEATS; j++) {
                outputFile << seatStatus[i][j].isReserved << " ";
            }
            outputFile << endl;
        }
        outputFile.close();
    }
}

// Function to check seat availability for a particular movie and seat number
bool checkSeatAvailability(const SeatStatus seatStatus[][MAX_SEATS], int movieIndex, int seatNumber) {
    return !seatStatus[movieIndex][seatNumber].isReserved;
}

// Function to reserve seats for a particular movie and seat number
void reserveSeats(SeatStatus seatStatus[][MAX_SEATS], int movieIndex, int seatNumber, int numTickets) {
    for (int i = seatNumber; i < seatNumber + numTickets; i++) {
        seatStatus[movieIndex][i].isReserved = true;
    }
    cout << "Seats reserved for the movie!" << endl;
}

// Function to calculate total price based on the number of tickets
double calculateTotalPrice(double ticketPrice, int numTickets) {
    return ticketPrice * numTickets;
}

// Function to calculate total revenue(income) based on the number of tickets reserved for each movie
double calculateTotalRevenue(const SeatStatus seatStatus[][MAX_SEATS], const Movie movies[], int numMovies) {
    double totalRevenue = 0.0;

    for (int i = 0; i < numMovies; i++) {
        int numTicketsSold = 0;
        for (int j = 0; j < MAX_SEATS; j++) {
            if (seatStatus[i][j].isReserved) {
                numTicketsSold++;
            }
        }
        totalRevenue += numTicketsSold * movies[i].price;
    }

    return totalRevenue;
}

// Function to reset all seats to available (for managers only)
void resetAllSeats(SeatStatus seatStatus[][MAX_SEATS], int numMovies) {
    for (int i = 0; i < numMovies; i++) {
        for (int j = 0; j < MAX_SEATS; j++) {
            seatStatus[i][j].isReserved = false;
        }
    }
    cout << "Daily Revenue has been reset to $0 and all seats have been reset to available!" << endl;
}

// Function to authenticate the ticket manager
bool authenticateManager() {
    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

   
    return (username == "admin" && password == "password");
}

// Function to display total revenue for the day

void displayTotalRevenue(const SeatStatus seatStatus[][MAX_SEATS], const Movie movies[], int numMovies) {
    double totalRevenue = calculateTotalRevenue(seatStatus, movies, numMovies);
    cout << "Total Revenue of the day: $" << totalRevenue << endl;
 
    ofstream outputFile("total_revenue.txt", ios::ate | ios::app);
    if (outputFile.is_open()) {
        outputFile << totalRevenue <<endl;
        outputFile.close();
    }
}


int main() {
    Movie movies[MAX_MOVIES];
    int numMovies = 3; // Number of available movies
    string reserveChoice;
    SeatStatus seatStatus[MAX_MOVIES][MAX_SEATS];

    initializeMovies(movies, numMovies);

    // Load seat reservation status from a file
    readSeatStatus(seatStatus, numMovies);

    
    cout<< "\t\t\tWELCOME TO CINEBOOK" <<endl;
    int userChoice;
    cout << "What would you like to do?" << endl;
    cout << "1. Check available movies" << endl;
    cout << "2. Reserve seats" << endl;
    cout << "3. Total Revenue of the day (For Ticket Managers Only)" << endl;
    cout << "4. Reset All Seats and Daily Revenue (For Ticket Managers Only)" << endl;
    cout << "Enter the number of your choice: ";
    cin >> userChoice;

    switch (userChoice) {
        case 1:
            displayMovies(movies, numMovies);
            cout << "Do you want to reserve a seat? (y/n): ";
            cin >> reserveChoice;
            if (reserveChoice == "y"||reserveChoice == "Y") {
                // Reservation process
                int movieIndex, seatNumber, numTickets;
                cout << "Enter the movie number: ";
                cin >> movieIndex;
                movieIndex--; // Adjusting to 0-based index

                if (movieIndex >= 0 && movieIndex < numMovies) {
                    cout << "Enter the first seat number (1-" << MAX_SEATS << "): ";
                    cin >> seatNumber;
                    seatNumber--; // Adjusting to 0-based index

                    if (seatNumber >= 0 && seatNumber < MAX_SEATS) {
                        cout << "Enter the number of tickets to reserve: ";
                        cin >> numTickets;

                        if (seatNumber + numTickets <= MAX_SEATS &&
                            checkSeatAvailability(seatStatus, movieIndex, seatNumber)) {
                            double totalPrice = calculateTotalPrice(movies[movieIndex].price, numTickets);
                            cout << "Total price: $" << totalPrice << endl;

                            reserveSeats(seatStatus, movieIndex, seatNumber, numTickets);

                            // Save seat reservation status to a file
                            writeSeatStatus(seatStatus, numMovies);
                        } else {
                            cout << "Invalid seat number or the seats are already reserved." << endl;
                        }
                    } else {
                        cout << "Invalid seat number!" << endl;
                    }
                } else {
                    cout << "Invalid movie number!" << endl;
                }
            }else{
                cout<<"Thanks for using CINEBOOK!!";
            }
            break;
        case 2:
            displayMovies(movies, numMovies);
            int movieIndex, seatNumber, numTickets;
            cout << "Enter the movie number: ";
            cin >> movieIndex;
            movieIndex--; // Adjusting to 0-based index

            if (movieIndex >= 0 && movieIndex < numMovies) {
                cout << "Enter the first seat number (1-" << MAX_SEATS << "): ";
                cin >> seatNumber;
                seatNumber--; // Adjusting to 0-based index

                if (seatNumber >= 0 && seatNumber < MAX_SEATS) {
                    cout << "Enter the number of tickets to reserve: ";
                    cin >> numTickets;

                    if (seatNumber + numTickets <= MAX_SEATS &&
                        checkSeatAvailability(seatStatus, movieIndex, seatNumber)) {
                        double totalPrice = calculateTotalPrice(movies[movieIndex].price, numTickets);
                        cout << "Total price: $" << totalPrice << endl;

                        reserveSeats(seatStatus, movieIndex, seatNumber, numTickets);

                        // Save seat reservation status to a file
                        writeSeatStatus(seatStatus, numMovies);
                    } else {
                        cout << "Invalid seat number or the seats are already reserved." << endl;
                    }
                } else {
                    cout << "Invalid seat number!" << endl;
                }
            } else {
                cout << "Invalid movie number!" << endl;
            }
            break;
        case 3:
            // Ticket manager authentication
            if (authenticateManager()) {
                displayTotalRevenue(seatStatus, movies, numMovies);
            } else {
                cout << "Authentication failed. Only ticket managers can access this option." << endl;
            }
            break;
        case 4:
            // Ticket manager authentication
            if (authenticateManager()) {
                resetAllSeats(seatStatus, numMovies);
                // Save seat reservation status to a file after resetting
                writeSeatStatus(seatStatus, numMovies);
            } else {
                cout << "Authentication failed. Only ticket managers can access this option." << endl;
            }
            break;
        default:
            cout << "Invalid choice!" << endl;
    }

    return 0;
}
