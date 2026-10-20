#include <bits/stdc++.h>
#include <iomanip> 
#include <ctime> // For generating real entry timestamps
using namespace std;

// --- COLOR LIBRARIES (ANSI Escape Codes) ---
const string RESET  = "\033[0m";
const string RED    = "\033[31m";
const string GREEN  = "\033[32m";
const string YELLOW = "\033[33m";
const string CYAN   = "\033[36m";
const string BOLD   = "\033[1m";

// 1. DATA STRUCTURES
enum VehicleCategory { EMERGENCY = 1, VIP = 2, REGULAR = 3 }; 

struct Vehicle {
    string id;
    string type; // "Car" or "Bike" only
    VehicleCategory category;
    time_t entryTime; // Used internally so the queue knows who arrived first
};

// 2. PRIORITY QUEUE LOGIC
struct WaitlistCompare {
    bool operator()(Vehicle a, Vehicle b) {
        if (a.category != b.category) {
            if (a.category > b.category) return true;  
            else return false; 
        }
        if (a.entryTime > b.entryTime) return true;  
        else return false; 
    }
};

class DashboardManager {
private:
    int bikesMax = 300;
    int bikesOccupied = 0; 
    
    int campusCarsMax = 7;
    vector<string> carSlots;
    
    priority_queue<Vehicle, vector<Vehicle>, WaitlistCompare> waitlist;

    // Helper: Draws a colorful progress bar for Bikes
    void drawProgressBar(string label, int occupied, int maxCap) {
        cout << BOLD << CYAN << left << setw(8) << label << RESET << ": [";
        int filledBars = (occupied * 20) / maxCap; 
        for(int i = 0; i < 20; i++) {
            if(i < filledBars) cout << YELLOW << "#" << RESET;
            else cout << "-";
        }
        cout << "] " << occupied << "/" << maxCap << " Occupied\n";
    }

    // Helper: Formats individual car slots with colors
    void drawCarSlot(string slotData) {
        if (slotData == "FREE") {
            cout << "[" << GREEN << "   FREE   " << RESET << "] ";
        } else {
            cout << "[" << RED << left << setw(10) << slotData << RESET << "] ";
        }
    }

public:
    // Constructor initializes empty parking lot
    DashboardManager() {
        carSlots.assign(campusCarsMax, "FREE");
    }

    // INTEGRATION HOOKS
   
    
    void updateCarSlots(vector<string> liveCars) {
        carSlots = liveCars;
    }

    void updateBikes(int currentBikes) {
        bikesOccupied = currentBikes;
    }

    void updateWaitlist(priority_queue<Vehicle, vector<Vehicle>, WaitlistCompare> liveQueue) {
        waitlist = liveQueue;
    }

  

    // UI DASHBOARD
    void renderLiveDashboard() {
        cout << "\033[2J\033[H"; // Clear screen
        
        cout << CYAN << "+=================================================+\n";
        cout << "|        " << BOLD << "PICT CAMPUS PARKING DASHBOARD" << RESET << CYAN << "            |\n";
        cout << "+=================================================+\n\n" << RESET;
        
        cout << BOLD << "[ CAMPUS CARS (VIP/Staff/Emergency) ]\n" << RESET;
        cout << "---------------------------------------------------\n";
        
        for (int i = 0; i < carSlots.size(); i++) {
            drawCarSlot(carSlots[i]);
            if (i == 3) cout << "\n"; 
        }
        cout << "\n\n";

        cout << BOLD << "[ CAMPUS BIKES (Macro View) ]\n" << RESET;
        cout << "---------------------------------------------------\n";
        drawProgressBar("Bikes", bikesOccupied, bikesMax);
        cout << "\n";
        
        cout << BOLD << "[ LIVE WAITLIST (Top 3) ]\n" << RESET;
        cout << "---------------------------------------------------\n";
        cout << BOLD << left << setw(5)  << "No." 
             << left << setw(15) << "Vehicle ID" 
             << left << setw(15) << "Category" << RESET << "\n";
        cout << "---------------------------------------------------\n";

        priority_queue<Vehicle, vector<Vehicle>, WaitlistCompare> tempQ = waitlist;
        int count = 1;
        
        if(tempQ.empty()) {
            cout << GREEN << "Queue is currently empty.\n" << RESET;
        }
        
        while (!tempQ.empty() && count <= 3) {
            Vehicle v = tempQ.top();
            tempQ.pop();
            
            string catString = "";
            if (v.category == 1) catString = "EMERGENCY";
            else if (v.category == 2) catString = "VIP";
            else if (v.category == 3) catString = "REGULAR";

            cout << left << setw(5)  << count 
                 << left << setw(15) << v.id 
                 << left << setw(15) << catString << "\n";
            count++;
        }
        cout << CYAN << "+=================================================+\n" << RESET;
    }

    // 4. FILE HANDLING (Now logs at ENTRY)
    void logEntryToCSV(Vehicle v) {
        ofstream file;
        file.open("parking_history.csv", ios::app); 
        
        if (file.is_open()) {
            // Saving only ID, Type, and Category
            file << v.id << "," << v.type << "," << v.category << "\n";
            cout << GREEN << "\n[SUCCESS] Saved entry for " << v.id << " to database.\n" << RESET;
            file.close();
        } else {
            cout << RED << "\n[ERROR] Could not open file!\n" << RESET;
        }
    }

    // 5. ANALYTICS (Counts total traffic instead of wait times)
    void generateAnalyticsReport() {
        cout << "\033[2J\033[H"; // Clear screen
        
        int totalCars = 0, totalBikes = 0;
        int countEmergency = 0, countVIP = 0, countRegular = 0;
        int totalEntries = 0;
        
        ifstream file("parking_history.csv");
        string line;
        
        // Read file line by line
        while (getline(file, line)) {
            stringstream ss(line);
            string id, type, catStr;
            
            getline(ss, id, ',');
            getline(ss, type, ',');
            getline(ss, catStr, ',');
            
            if (id.empty()) continue; // Skip empty lines
            
            totalEntries++;
            if (type == "Car" || type == "car") totalCars++;
            else if (type == "Bike" || type == "bike") totalBikes++;
            
            int category = stoi(catStr);
            if (category == 1) countEmergency++;
            else if (category == 2) countVIP++;
            else countRegular++;
        }
        file.close();

        cout << CYAN << "+=================================================+\n";
        cout << "|             PARKING ENTRY ANALYTICS             |\n";
        cout << "+=================================================+\n\n" << RESET;
        
        cout << BOLD << "[ TOTAL CAMPUS TRAFFIC ]\n" << RESET;
        cout << "Total Vehicles Entered : " << totalEntries << "\n\n";
        
        cout << BOLD << "[ BREAKDOWN BY TYPE ]\n" << RESET;
        cout << "Total Cars  : " << totalCars << "\n";
        cout << "Total Bikes : " << totalBikes << "\n\n";
        
        cout << BOLD << "[ BREAKDOWN BY PRIORITY ]\n" << RESET;
        cout << "Emergency (Category 1) : " << countEmergency << "\n";
        cout << "VIP/Staff (Category 2) : " << countVIP << "\n";
        cout << "Regular   (Category 3) : " << countRegular << "\n";
        
        cout << CYAN << "\n+=================================================+\n" << RESET;
    }
};

int main() {
    DashboardManager dashboard;
    
    // --- DUMMY DATA FOR TESTING THE UI ---
    vector<string> testIn = {"MH12EM0001", "MH12VIP002", "FREE", "FREE", "FREE", "FREE", "FREE"};
    dashboard.updateCarSlots(testIn);
    dashboard.updateBikes(145);
    
    priority_queue<Vehicle, vector<Vehicle>, WaitlistCompare> testQ;
    // We use time(0) to simulate real-world arrival times for the queue
    testQ.push({"MH12AA0001", "Car", REGULAR, time(0) + 1});
    testQ.push({"MH12EM9999", "Car", EMERGENCY, time(0)});
    dashboard.updateWaitlist(testQ);
    // -------------------------------------

    int choice;
    do {
        cout << CYAN << "\n[ MAIN MENU ]\n" << RESET;
        cout << "1. View Live Dashboard\n";
        cout << "2. Simulate Vehicle Entry (Save to CSV)\n";
        cout << "3. View Entry Analytics\n";
        cout << "4. Exit System\n";
        cout << "Enter choice: ";
        cin >> choice;
        
        if (choice == 1) {
            dashboard.renderLiveDashboard();
        } 
        else if (choice == 2) {
            Vehicle v;
            int catInput;
            
            cout << "\n--- RECORD VEHICLE ENTRY ---\n";
            cout << "Enter Vehicle ID (e.g., MH12XY1234): ";
            cin >> v.id;
            cout << "Enter Type (Car/Bike): ";
            cin >> v.type;
            cout << "Enter Category (1=Emergency, 2=VIP, 3=Regular): ";
            cin >> catInput;
            
            if (catInput == 1) v.category = EMERGENCY;
            else if (catInput == 2) v.category = VIP;
            else v.category = REGULAR;
            
            v.entryTime = time(0); // Captures the exact current computer time
            
            dashboard.logEntryToCSV(v);
        }
        else if (choice == 3) {
            dashboard.generateAnalyticsReport();
        }
        
    } while (choice != 4);

    return 0;
}