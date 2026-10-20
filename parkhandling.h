#ifndef PARKHANDLING_H
#define PARKHANDLING_H
#include <iostream>
#include <string>
#include <ctime>
using namespace std;

const int BIKE = 300;
const int CAR = 15;

int getTime();
void showTime(int t);

struct Vehicle {
    string number;
    int entry;

    Vehicle() {
        number = "";
        entry = -1;
    }
};

struct Node {
    string number;
    int wait;
    Node* next;

    Node(string n, int w) {
        number = n;
        wait = w;
        next = nullptr;
    }
};

class Queue {
public:
    Node* front;
    Node* rear;

    Queue() {
        front = nullptr;
        rear = nullptr;
    }

    bool empty() {
        return front == nullptr;
    }

    bool contains(string n) {
        Node* temp = front;

        while (temp != nullptr) {
            if (temp->number == n)
                return true;

            temp = temp->next;
        }

        return false;
    }

    void push(string n, int t) {
        Node* temp = new Node(n, t);

        if (front == nullptr) {
            front = rear = temp;
        } else {
            rear->next = temp;
            rear = temp;
        }
    }

    Node* pop() {
        if (front == nullptr)
            return nullptr;

        Node* temp = front;
        front = front->next;

        if (front == nullptr)
            rear = nullptr;

        temp->next = nullptr;
        return temp;
    }

    void display() {
        Node* temp = front;

        if (temp == nullptr) {
            cout << "No vehicles waiting\n";
            return;
        }

        while (temp != nullptr) {
            cout << temp->number << " ";
            temp = temp->next;
        }

        cout << "\n";
    }

    ~Queue() {
        while (front != nullptr) {
            Node* temp = front;
            front = front->next;
            delete temp;
        }
    }
};

class Parking {
public:
    Vehicle bikes[BIKE];
    Vehicle cars[CAR];

    Queue bikeQueue;
    Queue carQueue;

    int findSlot(Vehicle a[], int size) {
        for (int i = 0; i < size; i++) {
            if (a[i].number == "")
                return i;
        }

        return -1;
    }

    int findVehicle(Vehicle a[], int size, string n) {
        for (int i = 0; i < size; i++) {
            if (a[i].number == n)
                return i;
        }

        return -1;
    }

    bool exists(string n) {
        if (findVehicle(bikes, BIKE, n) != -1)
            return true;

        if (findVehicle(cars, CAR, n) != -1)
            return true;

        if (bikeQueue.contains(n))
            return true;

        if (carQueue.contains(n))
            return true;

        return false;
    }

    void entry() {
        int type;
        string n;

        cout << "1. Student two-wheeler\n";
        cout << "2. Professor four-wheeler\n";
        cout << "Enter type: ";
        cin >> type;

        if (type != 1 && type != 2) {
            cout << "Invalid type\n";
            return;
        }

        cout << "Enter vehicle number: ";
        cin >> n;

        if (exists(n)) {
            cout << "Vehicle already exists\n";
            return;
        }

        int t = getTime();

        if (type == 1) {
            int s = findSlot(bikes, BIKE);

            if (s != -1) {
                bikes[s].number = n;
                bikes[s].entry = t;
                cout << "Bike parked in slot " << s + 1 << "\n";
            } else {
                bikeQueue.push(n, t);
                cout << "Bike parking full. Added to queue\n";
            }
        } else {
            int s = findSlot(cars, CAR);

            if (s != -1) {
                cars[s].number = n;
                cars[s].entry = t;
                cout << "Car parked in slot " << s + 1 << "\n";
            } else {
                carQueue.push(n, t);
                cout << "Car parking full. Added to queue\n";
            }
        }
    }

    void exitVehicle() {
        string n;

        cout << "Enter vehicle number: ";
        cin >> n;

        int s = findVehicle(bikes, BIKE, n);
        bool isBike = (s != -1);

        if (!isBike)
            s = findVehicle(cars, CAR, n);

        if (s == -1) {
            cout << "Vehicle not parked\n";
            return;
        }

        Vehicle* a;
        Queue* q;

        if (isBike) {
            a = bikes;
            q = &bikeQueue;
        } else {
            a = cars;
            q = &carQueue;
        }

        int t = getTime();
        int duration = t - a[s].entry;

        if (duration < 0)
            duration += 1440;

        cout << "Entry time: ";
        showTime(a[s].entry);
        cout << "\nExit time: ";
        showTime(t);
        cout << "\nParking duration: " << duration << " minutes\n";

        a[s].number = "";
        a[s].entry = -1;

        cout << "Vehicle exited. No fee charged\n";

        if (!q->empty()) {
            Node* temp = q->pop();

            int waitTime = t - temp->wait;
            if (waitTime < 0)
                waitTime += 1440;

            a[s].number = temp->number;
            a[s].entry = getTime();

            cout << temp->number << " got the empty slot\n";
            cout << "Waiting time: " << waitTime << " minutes\n";

            delete temp;
        }
    }

    void display() {
        cout << "\n--- BIKE PARKING ---\n";

        for (int i = 0; i < BIKE; i++) {
            if (bikes[i].number != "")
                cout << i + 1 << ". " << bikes[i].number << "\n";
        }

        cout << "Bike slots occupied: "
             << BIKE - available(bikes, BIKE) << "/" << BIKE << "\n";

        cout << "\n--- CAR PARKING ---\n";

        for (int i = 0; i < CAR; i++) {
            if (cars[i].number != "")
                cout << i + 1 << ". " << cars[i].number << "\n";
        }

        cout << "Car slots occupied: "
             << CAR - available(cars, CAR) << "/" << CAR << "\n";
    }

    int available(Vehicle a[], int size) {
        int count = 0;

        for (int i = 0; i < size; i++) {
            if (a[i].number == "")
                count++;
        }

        return count;
    }

    void displayQueues() {
        cout << "Student waiting queue: ";
        bikeQueue.display();

        cout << "Professor waiting queue: ";
        carQueue.display();
    }
};
#endif

