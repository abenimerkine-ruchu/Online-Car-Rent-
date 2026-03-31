#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

//--------------------CLEAR SCREEN---------------------------
void ClearScreen() {
#ifdef _WIN32
    system("CLS");
#else
    system("clear");
#endif
}


//----------------------------------CAR CATALOG-----------------------
class Car {
public:
    int Id;
    string Brand;
    string Model;
    double PricePerDay;
    bool IsAvailable;

    Car() : Id(0), PricePerDay(0.0), IsAvailable(true) {}

    static void InitializeCars(Car list[], int& count, const int max) {
        string brands[5] = {"Toyota", "Hyundai", "Ford", "Nissan", "Kia"};
        string models[3] = {"Sedan", "Double Pickup", "Pickup"};
        double prices[5] = {2500, 3000, 3500, 4000, 4500};
        int id = 1001;
        for (int i = 0; i < max; i++) {
            list[i].Id = id++;
            list[i].Brand = brands[i % 5];
            list[i].Model = models[i % 3];
            list[i].PricePerDay = prices[i % 5];
            list[i].IsAvailable = true;
        }
        count = max;
    }
};



//--------------------------CUSTOMER DATA RECORDER--------------------
class Customer {
public:
    string Username;
    string Email;
    string LicenseNumber;
    string Password;

    static void LoadCustomers(Customer list[], int& count) {
        ifstream file("customers.txt");
        if (!file) return;
        count = 0;
        while (file >> list[count].Username
                    >> list[count].Email
                    >> list[count].LicenseNumber
                    >> list[count].Password) {
            count++;
        }
        file.close();
    }

    static void SaveCustomers(Customer list[], int count) {
        ofstream file("customers.txt");
        for (int i = 0; i < count; i++) {
            file << list[i].Username << " "
                 << list[i].Email << " "
                 << list[i].LicenseNumber << " "
                 << list[i].Password << endl;
        }
        file.close();
    }
};


//----------------RESERVATION------------------
struct Reservation {
    int Id;
    string CustomerName;
    int CarId;
    int RentalDays;
    double TotalCost;
    bool PaymentCompleted;
    string Status; 

    static void LoadReservations(Reservation list[], int& count) {
        ifstream file("reservations.txt");
        if (!file) return;
        count = 0;
        while (file >> list[count].Id
                    >> list[count].CustomerName
                    >> list[count].CarId
                    >> list[count].RentalDays
                    >> list[count].TotalCost
                    >> list[count].PaymentCompleted
                    >> list[count].Status) {
            count++;
        }
        file.close();
    }

    static void SaveReservations(Reservation list[], int count) {
        ofstream file("reservations.txt");
        for (int i = 0; i < count; i++) {
            file << list[i].Id << " "
                 << list[i].CustomerName << " "
                 << list[i].CarId << " "
                 << list[i].RentalDays << " "
                 << list[i].TotalCost << " "
                 << list[i].PaymentCompleted << " "
                 << list[i].Status << endl;
        }
        file.close();
    }
};



//-----------------GLOBAL DATA DECLARATION-------------------------
const int MAX_CARS = 30;
const int MAX_CUSTOMERS = 50;
const int MAX_RESERVATIONS = 100;

Car CarList[MAX_CARS];                
Customer CustomerList[MAX_CUSTOMERS]; 
Reservation ReservationList[MAX_RESERVATIONS]; 

int CarCount = 0;
int CustomerCount = 0;
int ReservationCount = 0;

string AdminUsername = "admin1";
string AdminPassword = "adm123";



//----------------------DATE CALCULATION------------------------
int CalculateDays(int y1, int m1, int d1, int y2, int m2, int d2) {
    struct tm start = {0}, end = {0};
    start.tm_year = y1 - 1900; start.tm_mon = m1 - 1; start.tm_mday = d1;
    end.tm_year = y2 - 1900; end.tm_mon = m2 - 1; end.tm_mday = d2;
    time_t startTime = mktime(&start);
    time_t endTime = mktime(&end);
    if (startTime == -1 || endTime == -1) return -1;
    return (int)(difftime(endTime, startTime) / (60 * 60 * 24));
}



//------------CAR CATALOG DURING BROWSE------------------------------
void DisplayCarCatalog() {
    ClearScreen();
    cout << "\n--- CAR CATALOG ---\n";
    for (int i = 0; i < CarCount; i++) {
        cout << "\t\t\t===================================" << endl;
        cout << "\t\t\tCar ID: " << CarList[i].Id << endl;
        cout << "\t\t\tCar Brand | " << CarList[i].Brand << endl;
        cout << "\t\t\tCar Model | " << CarList[i].Model << endl;
        cout << "\t\t\tRate      | Birr " << CarList[i].PricePerDay << "/Day" << endl;
        cout << "\t\t\tStatus    | " << (CarList[i].IsAvailable ? "Available" : "Booked") << endl;
    }
}



//----------------CUSTOMER REGISTERATION----------------------
void RegisterCustomer() {
    ClearScreen();
    if (CustomerCount >= MAX_CUSTOMERS) {
        cout << "\t\t\tDatabase full!\n";
        return;
    }
    Customer c;
    cout << "\t\t\t--- REGISTRATION ---\n";
    
    
    bool exists;
    do {
        exists = false;
        cout << "\t\t\tUsername: "; cin >> c.Username;
        for (int i = 0; i < CustomerCount; i++) {
            if (CustomerList[i].Username == c.Username) {
                cout << "\t\t\tError: Username already exists! Try again.\n";
                exists = true;
                break;
            }
        }
    } while (exists);

    cout << "\t\t\tEmail: "; cin >> c.Email;
    cout << "\t\t\tLicense: "; cin >> c.LicenseNumber;
    cout << "\t\t\tPassword: "; cin >> c.Password;

    CustomerList[CustomerCount++] = c;
    Customer::SaveCustomers(CustomerList, CustomerCount);
    cout << "\t\t\tRegistered successfully! Press Enter to return to menu...";
    cin.ignore(); cin.get();
}



//------------LOGIN DASHBOARD------------------
int LoginCustomer() {
    ClearScreen();
    string u, p;
    cout << "\t\t\t--- LOGIN ---\n";
    cout << "\t\t\tUsername: "; cin >> u;
    cout << "\t\t\tPassword: "; cin >> p;
    for (int i = 0; i < CustomerCount; i++) {
        if (CustomerList[i].Username == u && CustomerList[i].Password == p) return i;
    }
    return -1;
}



//--------------BILL GENERATIG----------------------
bool DisplayBill(int cIdx, int carIdx, int days, double total) {
    ClearScreen();
    cout << "\t\t\t===============================\n";
    cout << "\t\t\t        BILLING SUMMARY\n";
    cout << "\t\t\t===============================\n";
    cout << "\t\t\tCustomer: " << CustomerList[cIdx].Username << endl;
    cout << "\t\t\tCar     : " << CarList[carIdx].Brand << " " << CarList[carIdx].Model << endl;
    cout << "\t\t\tDuration: " << days << " days\n";
    cout << "\t\t\tTotal   : Birr " << total << endl;
    cout << "\t\t\t===============================\n";
    char confirm;
    cout << "\t\t\tConfirm Payment? (y/n): "; cin >> confirm;
    return (confirm == 'y' || confirm == 'Y');
}



//----------------REQUIRED INPUTS FROM CUSTOMER DURING RESERVATION------------------------------
void ReserveCar(int customerIndex) {
    DisplayCarCatalog();
    int carId, py, pm, pd, ry, rm, rd;
    cout << "\n\t\t\tEnter Car ID: "; cin >> carId;

    for (int i = 0; i < CarCount; i++) {
        if (CarList[i].Id == carId && CarList[i].IsAvailable) {
            
            
            cout << "\t\t\tPickup Date (Year): "; cin >> py ;
            do {
                cout << "\t\t\t(Month 1-12): "; cin >> pm;
            } while (pm < 1 || pm > 12);
            do {
                cout << "\t\t\t(Date 1-30): "; cin >> pd;
            } while (pd < 1 || pd > 30);

            cout << "\t\t\tReturn Date (Year): "; cin >> ry; 
            do {
                cout << "\t\t\t(Month 1-12): "; cin >> rm;
            } while (rm < 1 || rm > 12);
            do {
                cout << "\t\t\t(Date 1-30): "; cin >> rd;
            } while (rd < 1 || rd > 30);

            int days = CalculateDays(py, pm, pd, ry, rm, rd);
            if (days <= 0) {
                cout << "\t\t\tError: Check Your Inserted Date. Press Enter...";
                cin.ignore(); cin.get(); return;
            }

            double total = days * CarList[i].PricePerDay;
            if (DisplayBill(customerIndex, i, days, total)) {
                Reservation r;
                r.Id = ReservationCount + 1;
                r.CustomerName = CustomerList[customerIndex].Username;
                r.CarId = carId;
                r.RentalDays = days;
                r.TotalCost = total;
                r.PaymentCompleted = true;
                r.Status = "Pending";

                ReservationList[ReservationCount++] = r;
                CarList[i].IsAvailable = false;
                Reservation::SaveReservations(ReservationList, ReservationCount); 
                cout << "\t\t\tReserved! A waiting Admin Approval. Press Enter...";
            } else {
                cout << "\t\t\tCancelled. Press Enter...";
            }
            cin.ignore(); cin.get(); return;
        }
    }
}



//--------------------BOOKING TIME MENU------------------------
void CustomerMenu(int cIdx) {
    int choice;
    do {
        ClearScreen();
        cout << "\t\t\t--- USER MENU: " << CustomerList[cIdx].Username << " ---\n";
        cout << "\t\t\t1. Browse Cars\n\t\t\t2. Book Car\n\t\t\t3. Logout\n\t\t\tSelect: ";
        cin >> choice;
        if (choice == 1) { DisplayCarCatalog(); cout << "Press Enter..."; cin.ignore(); cin.get(); }
        else if (choice == 2) ReserveCar(cIdx);
    } while (choice != 3);
}



//------------ADMIN DASHBOARD--------------------
void AdminMenu() {
    int choice;
    do {
        ClearScreen();
        cout << "\t\t\t--- ADMIN DASHBOARD ---\n";
        if (ReservationCount == 0) cout << "\t\t\tNo bookings found.\n";
        else {
            for (int i = 0; i < ReservationCount; i++) {
                cout << "\t\t\tID: " << ReservationList[i].Id << " | User: " << ReservationList[i].CustomerName 
                     << " | Cost: " << ReservationList[i].TotalCost << " | Status: " << ReservationList[i].Status << endl;
            }
        }
        cout << "\n\t\t\t1. Process Booking\n\t\t\t2. Exit Admin Menu\n\t\t\tSelect: ";
        cin >> choice;

        if (choice == 1 && ReservationCount > 0) {
            int rId, action;
            cout << "\t\t\tEnter Reservation ID: "; cin >> rId;
            for (int i = 0; i < ReservationCount; i++) {
                if (ReservationList[i].Id == rId) {
                    cout << "\t\t\t1. Approve \n";
                    cout << "\t\t\t2. Reject"<< endl;
                    cout << "\t\t\tEnter Your Decision: ";
                    cin >> action;
                    ReservationList[i].Status = (action == 1) ? "Approved" : "Rejected";
                    if (action == 2) {
                        for(int j=0; j<CarCount; j++) if(CarList[j].Id == ReservationList[i].CarId) CarList[j].IsAvailable = true;
                    }
                    Reservation::SaveReservations(ReservationList, ReservationCount); 
                    cout << "\t\t\tAction complete! Press Enter..."; cin.ignore(); cin.get();
                }
            }
        }
    } while (choice != 2);
}



//----------MAIN FUNCTION------------------
int main() {
    #ifdef _WIN32
    system("color 02");
    #endif

    Car::InitializeCars(CarList, CarCount, MAX_CARS);
    Customer::LoadCustomers(CustomerList, CustomerCount);
    Reservation::LoadReservations(ReservationList, ReservationCount);

    int choice;
    do {
        ClearScreen();
        cout << "\n--- WELCOME TO KIDUS CAR RENTAL ---\n";
        cout << "1. Browse Car(Guest View)\n2. Register\n3. Customer Login\n4. Admin Login\n5. Exit\nSelect: ";
        cin >> choice;

        if (choice == 1) { DisplayCarCatalog(); cout << "Press Enter..."; cin.ignore(); cin.get(); }
        else if (choice == 2) RegisterCustomer();
        else if (choice == 3) {
            int idx = LoginCustomer();
            if (idx != -1) CustomerMenu(idx);
            else { cout << "\t\t\tLogin Failed! Press Enter..."; cin.ignore(); cin.get(); }
        }
        else if (choice == 4) {
            string u, p;
            cout << "\t\t\tAdmin User: "; cin >> u;
            cout << "\t\t\tAdmin Pass: "; cin >> p;
            if (u == AdminUsername && p == AdminPassword) AdminMenu();
            else { cout << "\t\t\tAccess Denied! Press Enter..."; cin.ignore(); cin.get(); }
        }
    } while (choice != 5);

    ClearScreen();
    cout << "\n\n\n\t\t\t=========================================\n";
    cout << "\t\t\tTHANK YOU FOR VISITING OUR WEBSITE!\n";
    cout << "\t\t\t        HAVE A WONDERFUL DAY!\n";
    cout << "\t\t\t=========================================\n\n\n";

    return 0;
}