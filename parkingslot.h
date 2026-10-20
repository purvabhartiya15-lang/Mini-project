#ifndef PARKINGSLOT_H
#define PARKINGSLOT_H

#include<iostream>
using namespace std;

class park{
    public:
    // int n;
    // cout<<"Enter number of slots";
    // cin>>n;
    park* arr[50];
    int reg;

    void parking(){
     cout<<"Enter registration number:";
     cin>>reg;
     for(int i=0;i<50;i++){
        if(arr[i] != NULL && arr[i]->reg==reg){
            cout<<"Duplicate number";
            return;
        }
     }
     for (int i = 0; i < 50; i++) {
        if (arr[i] == NULL) {
            arr[i] = new park;
            arr[i]->reg = reg;

            cout << "Vehicle parked successfully!\n";
            cout << "Parking slot number: " << i + 1 << endl;
            return;
        }
    }
     cout << "No duplicate found. Vehicle can be parked.";
    }

    void remove(){
        cout<<"Enter registration number:";
        cin>>reg;
        for(int i=0;i<50;i++){
            if(arr[i]!=NULL && arr[i]->reg==reg){
                cout<<"Found and removed successfully";
                arr[i]=NULL;
                break;
            }
        }
        cout<<"Appropriate number please enter valid one!";
    }

    void displayParkingSlots(){
        cout<<"===== PARKING STATUS =====";
        for(int i=0;i<50;i++){
            if(arr[i]!=NULL){
                cout<<arr[i]->reg<<"(Occupied)";
                // break;
            } else {
            cout << "Slot " << i + 1 << ": Empty\n";
            }
        }
        
    }

    void displayAvailableSlots(){
        int count=0;
        cout << "\n===== AVAILABLE PARKING SLOTS =====\n";
        for(int i=0;i<50;i++){
            if(arr[i]==NULL){
                cout << "Slot " << i + 1 << " is available." << endl;
                count++;
            }
        }
        if (count == 0) {
           cout << "No parking slots available!" << endl;
        }

        cout << "Total available slots: " << count << endl;
    }

    void searchVehicle(){
        cout<<"Enter registration number:";
        cin>>reg;
        for(int i=0;i<50;i++){
            if(arr[i]!=NULL && arr[i]->reg==reg){
                cout<<"Vehicle Found";
                cout<<"Registration number:"<<arr[i]->reg;
                cout<<"Parking Slot number:"<<i+1;
                return;
            }
        }
        cout << "Vehicle not found in parking!" << endl;
    }

    int getOccupiedCount() {
        int count = 0;

        for (int i = 0; i < 50; i++) {
            if (arr[i] != NULL) {
                count++;
            }
        }

    return count;
   }

   void duplicatedetection(){
    for (int i = 0; i < 50; i++) {
        if (arr[i] != NULL && arr[i]->reg == reg) {
        cout << "Duplicate number";
        return;
        }
    }
   }
};
#endif
// int main() {
//     park p;
//     int choice;

//     do {
//         cout << "\n===== PARKING MANAGEMENT SYSTEM =====\n";
//         cout << "1. Park Vehicle\n";
//         cout << "2. Remove Vehicle\n";
//         cout << "3. Display Parking Slots\n";
//         cout << "4. Display Available Slots\n";
//         cout << "5. Search Vehicle\n";
//         cout << "6. Display Occupancy Report\n";
//         cout << "7. Display Waiting Queue\n";
//         cout << "8. Exit\n";

//         cout << "Enter choice: ";
//         cin >> choice;

//         switch (choice) {
//             case 1:
//                 p.parking();
//                 break;
//             case 2:
//                 p.remove();
//                 break;
//             case 3:
//                 p.displayParkingSlots();
//                 break;
//             case 4:
//                 p.displayAvailableSlots();
//                 break;
//             case 5:
//                 p.searchVehicle();
//                 break;
//             // case 6:
//             //     p.displayOccupancyReport();
//             //     break;
//             // case 7:
//             //     p.displayWaitingQueue();
//             //     break;
//             case 6:
//                 cout << "Exiting...\n";
//                 break;
//             default:
//                 cout << "Invalid choice!\n";
//         }
//     } while (choice != 8);

//     return 0;
// }