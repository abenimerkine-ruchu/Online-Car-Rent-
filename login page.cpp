#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// --- Global Constants for Formatting ---
const int LINE_WIDTH = 70; // Total width for centered effect
const string CARS_FILE = "cars.txt";
const string USERS_FILE = "users.txt";

// --- Function Prototypes ---
void centerPrint(const string& text);
void registerUser();
bool loginUser();
void viewAvailableCars();
void rentCar(const string& username);
void carRentalMenu(const string& username);
void displayMainMenu();

// --- Formatting Helper Function ---
// Simple function to pad text with spaces to simulate centering
void centerPrint(const string& text) {
    int padding = (LINE_WIDTH - text.length()) / 2;
    // Ensure padding is non-negative
    if (padding < 0) padding = 0; 
    cout << string(padding, ' ') << text << endl;
}

// --- 1. User Registration Function ---
void registerUser() {
    string username, password;
    centerPrint("--- New User Registration ---");
    centerPrint("Enter a new username:");
    cout << string(LINE_WIDTH/2, ' ') << "> ";
    cin >> username;
    centerPrint("Enter a new password:");
    cout << string(LINE_WIDTH/2, ' ') << "> ";
    cin >> password;

    ofstream outFile(USERS_FILE, ios::app); 
    
    if (outFile.is_open()) {
        outFile << username << " " << password << "\n";
        outFile.close();
        centerPrint("? Registration successful! Please log in now.");
    } else {
        centerPrint("? Error: Could not open the user database file.");
    }
}

// --- 2. User Login Function ---
bool loginUser() {
    string username, password;
    string fileUsername, filePassword;
    bool loginSuccess = false;

    centerPrint("--- User Login ---");
    centerPrint("Enter username:");
    cout << string(LINE_WIDTH/2, ' ') << "> ";
    cin >> username;
    centerPrint("Enter password:");
    cout << string(LINE_WIDTH/2, ' ') << "> ";
    cin >> password;

    ifstream inFile(USERS_FILE);

    if (inFile.is_open()) {
        while (inFile >> fileUsername >> filePassword) {
            if (fileUsername == username && filePassword == password) {
                centerPrint("? Welcome back, " + username + "! Login successful.");
                carRentalMenu(username);
                loginSuccess = true;
                break;
            }
        }
        inFile.close();

        if (!loginSuccess) {
            centerPrint("? Login failed. Invalid username or password.");
        }
    } else {
        centerPrint("?? No user accounts found. Please register first.");
    }
    
    return loginSuccess;
}

// --- 3. Car Listing Function (Ethiopia-Focused) ---
void viewAvailableCars() {
    string carID, makeModel, dailyRate;
    
    centerPrint("--- ?? Available Rental Cars in Ethiopia ---");
    centerPrint("--------------------------------------------------");
    
    // Check if the car list file exists and populate if it doesn't
    ifstream carCheck(CARS_FILE);
    if (!carCheck.good()) {
        ofstream carInit(CARS_FILE);
        carInit << "C001 Toyota_Yaris 500.00" << endl;
        carInit << "C002 Suzuki_Desire 650.00" << endl;
        carInit << "C003 Nissan_Tiida 750.00" << endl;
        carInit << "C004 Land_Cruiser_VX 2500.00" << endl;
        carInit.close();
        centerPrint("Note: Default car list created in " + CARS_FILE);
    }
    carCheck.close();
    
    // Read and display the list
    ifstream carFile(CARS_FILE);
    if (carFile.is_open()) {
        centerPrint("ID | Model/Make            | Daily Rate (ETB)");
        centerPrint("--------------------------------------------------");
        while (carFile >> carID >> makeModel >> dailyRate) {
            // Replace underscores with spaces for clean display
            size_t pos = makeModel.find('_');
            while (pos != string::npos) {
                makeModel.replace(pos, 1, " ");
                pos = makeModel.find('_', pos + 1);
            }
            
            // Basic manual padding for alignment
            string makeModelPadded = makeModel;
            if (makeModelPadded.length() < 20) {
                makeModelPadded += string(20 - makeModelPadded.length(), ' ');
            }

            string line = carID + " | " + makeModelPadded + " | " + dailyRate;
            centerPrint(line);
        }
        carFile.close();
    } else {
        centerPrint("? Error: Car list file not found.");
    }
    centerPrint("--------------------------------------------------");
}

// --- 4. Essential Additional Function: Rent a Car ---
void rentCar(const string& username) {
    string carID;
    int rentalDays;
    
    centerPrint("--- ?? Rent a Car Booking ---");
    centerPrint("You must view the cars first (Option 1) to get the ID.");
    centerPrint("Enter the Car ID to rent (e.g., C002):");
    cout << string(LINE_WIDTH/2, ' ') << "> ";
    cin >> carID;
    centerPrint("Enter number of rental days:");
    cout << string(LINE_WIDTH/2, ' ') << "> ";
    
    // Input validation loop
    while (!(cin >> rentalDays) || rentalDays <= 0) {
        centerPrint("? Invalid input. Enter a positive number of days:");
        cin.clear();
        string dummy;
        cin >> dummy;
        cout << string(LINE_WIDTH/2, ' ') << "> ";
    }
    
    // Simulation of Cost Calculation
    double rate = 0.0;
    // Simple look-up simulation based on ID
    if (carID == "C001") rate = 500.0;
    else if (carID == "C002") rate = 650.0;
    else if (carID == "C003") rate = 750.0;
    else if (carID == "C004") rate = 2500.0;
    else rate = 700.0; // Default rate for unknown ID

    double totalCost = rentalDays * rate;
    
    centerPrint("--- Booking Summary ---");
    centerPrint("Car ID: " + carID);
    centerPrint("Rental Days: " + to_string(rentalDays));
    centerPrint("Daily Rate: " + to_string(rate) + " ETB");
    centerPrint("TOTAL COST: " + to_string(totalCost) + " ETB");
    centerPrint("? Booking Confirmed for " + username + "!");
    centerPrint("--------------------------------------------------");
}

// --- 5. Main Application Menu (After Successful Login) ---
void carRentalMenu(const string& username) {
    int choice;
    do {
        centerPrint("\n--- ?? Car Rental System Menu ---");
        centerPrint("Logged in as: " + username);
        centerPrint("1. View Available Cars");
        centerPrint("2. Rent a Car");
        centerPrint("3. View My Bookings (Placeholder)");
        centerPrint("4. Logout");
        centerPrint("Enter your choice (1-4):");
        cout << string(LINE_WIDTH/2, ' ') << "> ";
        
        if (!(cin >> choice)) {
            centerPrint("?? Invalid input. Please enter a number.");
            cin.clear();
            string dummy;
            cin >> dummy; 
            continue;
        }

        switch (choice) {
            case 1: 
                viewAvailableCars();
                break;
            case 2: 
                rentCar(username); 
                break;
            case 3: 
                centerPrint("[FUNCTIONALITY PLACEHOLDER] Loading booking history...");
                break;
            case 4: 
                centerPrint("You have been successfully logged out.");
                return; 
            default: 
                centerPrint("?? Invalid choice. Please try again (1-4).");
        }
    } while (choice != 4);
}

// --- 6. Initial Main Menu ---
void displayMainMenu() {
    int choice;
    
    do {
        centerPrint("\n=======================================================");
        centerPrint("???? Ethiopian Car Rental System");
        centerPrint("=======================================================");
        centerPrint("1. Register");
        centerPrint("2. Login");
        centerPrint("3. Exit System");
        centerPrint("Enter your choice (1-3):");
        cout << string(LINE_WIDTH/2, ' ') << "> ";

        if (!(cin >> choice)) {
            centerPrint("?? Invalid input. Please enter a number.");
            cin.clear();
            string dummy;
            cin >> dummy; 
            continue;
        }

        switch (choice) {
            case 1:
                registerUser();
                break;
            case 2:
                loginUser();
                break;
            case 3:
                centerPrint("?? Thank you for using the Car Rental System. Goodbye!");
                break;
            default:
                centerPrint("?? Invalid choice. Please select 1, 2, or 3.");
                break;
        }

    } while (choice != 3); 
}

// --- Main Program Entry Point ---
int main() {
    // Initialize the console output for better readability
    cout << "\n\n";
    displayMainMenu();
    return 0;
}