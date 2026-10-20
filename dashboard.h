
#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <bits/stdc++.h>
#include <iomanip>
#include <ctime>
using namespace std;

// COLOR CODES
const string RESET  = "\033[0m";
const string RED    = "\033[31m";
const string GREEN  = "\033[32m";
const string YELLOW = "\033[33m";
const string CYAN   = "\033[36m";
const string BOLD   = "\033[1m";

// Dashboard-specific vehicle structure
enum VehicleCategory {
    EMERGENCY = 1,
    VIP = 2,
    REGULAR = 3
};

struct DashboardVehicle {
    string id;
    string type;
    VehicleCategory category;
    time_t entryTime;
};

// Priority queue comparator
struct WaitlistCompare {
    bool operator()(const DashboardVehicle& a,
                    const DashboardVehicle& b) const {
        if (a.category != b.category)
            return a.category > b.category;

        return a.entryTime > b.entryTime;
    }
};

using DashboardQueue =
    priority_queue<DashboardVehicle,
                   vector<DashboardVehicle>,
                   WaitlistCompare>;

class DashboardManager {
private:
    int bikesMax = 300;
    int bikesOccupied = 0;

    int campusCarsMax = 7;
    vector<string> carSlots;

    DashboardQueue waitlist;

    void drawProgressBar(string label, int occupied, int maxCap) {
        cout << BOLD << CYAN << left << setw(8)
             << label << RESET << ": [";

        int filledBars = (occupied * 20) / maxCap;

        for (int i = 0; i < 20; i++) {
            if (i < filledBars)
                cout << YELLOW << "#" << RESET;
            else
                cout << "-";
        }

        cout << "] " << occupied << "/" << maxCap
             << " Occupied\n";
    }

    void drawCarSlot(string slotData) {
        if (slotData == "FREE") {
            cout << "[" << GREEN << "   FREE   " << RESET << "] ";
        } else {
            cout << "[" << RED << left << setw(10)
                 << slotData << RESET << "] ";
        }
    }

public:
    DashboardManager() {
        carSlots.assign(campusCarsMax, "FREE");
    }

    void updateCarSlots(const vector<string>& liveCars) {
        carSlots = liveCars;
    }

    void updateBikes(int currentBikes) {
        bikesOccupied = currentBikes;
    }

    void updateWaitlist(const DashboardQueue& liveQueue) {
        waitlist = liveQueue;
    }

    void renderLiveDashboard() {
        cout << "\033[2J\033[H";

        cout << CYAN
             << "+=================================================+\n"
             << "|        " << BOLD
             << "PICT CAMPUS PARKING DASHBOARD"
             << RESET << CYAN << "            |\n"
             << "+=================================================+\n\n"
             << RESET;

        cout << BOLD
             << "[ CAMPUS CARS (VIP/Staff/Emergency) ]\n"
             << RESET;
        cout << "---------------------------------------------------\n";

        for (int i = 0; i < static_cast<int>(carSlots.size()); i++) {
            drawCarSlot(carSlots[i]);
            if (i == 3)
                cout << "\n";
        }

        cout << "\n\n";
        cout << BOLD << "[ CAMPUS BIKES (Macro View) ]\n" << RESET;
        cout << "---------------------------------------------------\n";

        drawProgressBar("Bikes", bikesOccupied, bikesMax);

        cout << "\n";
        cout << BOLD << "[ LIVE WAITLIST (Top 3) ]\n" << RESET;
        cout << "---------------------------------------------------\n";

        cout << BOLD << left << setw(5) << "No."
             << left << setw(15) << "Vehicle ID"
             << left << setw(15) << "Category"
             << RESET << "\n";

        cout << "---------------------------------------------------\n";

        DashboardQueue tempQ = waitlist;
        int count = 1;

        if (tempQ.empty())
            cout << GREEN << "Queue is currently empty.\n" << RESET;

        while (!tempQ.empty() && count <= 3) {
            DashboardVehicle v = tempQ.top();
            tempQ.pop();

            string catString;

            if (v.category == EMERGENCY)
                catString = "EMERGENCY";
            else if (v.category == VIP)
                catString = "VIP";
            else
                catString = "REGULAR";

            cout << left << setw(5) << count
                 << left << setw(15) << v.id
                 << left << setw(15) << catString << "\n";

            count++;
        }

        cout << CYAN
             << "+=================================================+\n"
             << RESET;
    }

    void logEntryToCSV(const DashboardVehicle& v) {
        ofstream file("parking_history.csv", ios::app);

        if (file.is_open()) {
            file << v.id << "," << v.type << ","
                 << static_cast<int>(v.category) << "\n";

            cout << GREEN << "\n[SUCCESS] Saved entry for "
                 << v.id << " to database.\n" << RESET;
        } else {
            cout << RED << "\n[ERROR] Could not open file!\n" << RESET;
        }
    }

    void generateAnalyticsReport() {
        cout << "\033[2J\033[H";

        int totalCars = 0, totalBikes = 0;
        int countEmergency = 0, countVIP = 0, countRegular = 0;
        int totalEntries = 0;

        ifstream file("parking_history.csv");
        string line;

        while (getline(file, line)) {
            stringstream ss(line);
            string id, type, catStr;

            getline(ss, id, ',');
            getline(ss, type, ',');
            getline(ss, catStr, ',');

            if (id.empty() || catStr.empty())
                continue;

            totalEntries++;

            if (type == "Car" || type == "car")
                totalCars++;
            else if (type == "Bike" || type == "bike")
                totalBikes++;

            try {
                int category = stoi(catStr);

                if (category == EMERGENCY)
                    countEmergency++;
                else if (category == VIP)
                    countVIP++;
                else if (category == REGULAR)
                    countRegular++;
            } catch (const invalid_argument&) {
                // Ignore malformed CSV rows.
            } catch (const out_of_range&) {
                // Ignore malformed CSV rows.
            }
        }

        cout << CYAN
             << "+=================================================+\n"
             << "|             PARKING ENTRY ANALYTICS             |\n"
             << "+=================================================+\n\n"
             << RESET;

        cout << BOLD << "[ TOTAL CAMPUS TRAFFIC ]\n" << RESET;
        cout << "Total Vehicles Entered : " << totalEntries << "\n\n";

        cout << BOLD << "[ BREAKDOWN BY TYPE ]\n" << RESET;
        cout << "Total Cars  : " << totalCars << "\n";
        cout << "Total Bikes : " << totalBikes << "\n\n";

        cout << BOLD << "[ BREAKDOWN BY PRIORITY ]\n" << RESET;
        cout << "Emergency (Category 1) : " << countEmergency << "\n";
        cout << "VIP/Staff (Category 2) : " << countVIP << "\n";
        cout << "Regular   (Category 3) : " << countRegular << "\n";

        cout << CYAN
             << "\n+=================================================+\n"
             << RESET;
    }
};

#endif
