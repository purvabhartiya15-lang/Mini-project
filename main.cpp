#include <iostream>
using namespace std;
#include "parkhandling.h"
#include "parkingslot.h"
#include "dashboard.h"

int main() {
    Parking p;
    DashboardManager dashboard;
    park slotManager;

    int choice;

    do {
        cout << "\n===== CAMPUS PARKING MANAGEMENT SYSTEM =====\n";
        cout << "1. Vehicle Entry\n";
        cout << "2. Vehicle Exit\n";
        cout << "3. Display Parked Vehicles\n";
        cout << "4. Display Waiting Queues\n";
        cout << "5. Search Vehicle\n";
        cout << "6. Display Available Slots\n";
        cout << "7. Display Occupancy Report\n";
        cout << "8. View Live Dashboard\n";
        cout << "9. Generate Analytics Report\n";
        cout << "10. Exit\n";

        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                p.entry();
                break;

            case 2:
                p.exitVehicle();
                break;

            case 3:
                p.display();
                break;

            case 4:
                p.displayQueues();
                break;

            case 5:
                slotManager.searchVehicle();
                break;

            case 6:
                cout << "Available bike slots: "
                     << p.available(p.bikes, BIKE) << "\n";
                cout << "Available car slots: "
                     << p.available(p.cars, CAR) << "\n";
                slotManager.displayAvailableSlots();
                break;

            case 7:
                p.display();
                cout << "\nOccupied slots in slot manager: "
                     << slotManager.getOccupiedCount() << "\n";
                break;

            case 8:
                dashboard.renderLiveDashboard();
                break;

            case 9:
                dashboard.generateAnalyticsReport();
                break;

            case 10:
                cout << "Exiting Campus Parking System.\n";
                break;

            default:
                cout << "Invalid choice. Please try again.\n";
        }

    } while (choice != 10);

    return 0;
}
