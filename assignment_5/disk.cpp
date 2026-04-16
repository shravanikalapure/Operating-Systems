#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;


// FCFS
int FCFS(vector<int> req, int head) {
    int total = 0; // Total seek time

    cout << "\nHead Movement (FCFS): " << head;

    // Process requests in given order
    for (int i = 0; i < req.size(); i++) {
        cout << " -> " << req[i];  // Print movement
        total += abs(head - req[i]); // Calculate seek distance
        head = req[i]; // Move head
    }

    cout << "\nTotal Seek Time (FCFS): " << total << endl;
    return total;
}


// SSTF
int SSTF(vector<int> req, int head) {
    int total = 0;
    int n = req.size();

    vector<bool> visited(n, false); // Track serviced requests

    cout << "\nHead Movement (SSTF): " << head;

    // Select closest request each time
    for (int i = 0; i < n; i++) {
        int minDist = 1e9;
        int index = -1;

        // Find nearest unvisited request
        for (int j = 0; j < n; j++) {
            if (!visited[j]) {
                int dist = abs(head - req[j]);
                if (dist < minDist) {
                    minDist = dist;
                    index = j;
                }
            }
        }

        visited[index] = true;
        cout << " -> " << req[index];

        total += minDist;
        head = req[index];
    }

    cout << "\nTotal Seek Time (SSTF): " << total << endl;
    return total;
}


// SCAN
int SCAN(vector<int> req, int head, int disk_size, string direction) {
    int total = 0;

    vector<int> left, right;

    for (int r : req) {
        if (r < head) left.push_back(r);
        else right.push_back(r);
    }

    sort(left.begin(), left.end());
    sort(right.begin(), right.end());

    cout << "\nHead Movement (SCAN): " << head;

    if (direction == "right") {

        for (int r : right) {
            cout << " -> " << r;
            total += abs(head - r);
            head = r;
        }

        cout << " -> " << (disk_size - 1);
        total += abs(head - (disk_size - 1));
        head = disk_size - 1;

        for (int i = left.size() - 1; i >= 0; i--) {
            cout << " -> " << left[i];
            total += abs(head - left[i]);
            head = left[i];
        }
    }
    else {

        for (int i = left.size() - 1; i >= 0; i--) {
            cout << " -> " << left[i];
            total += abs(head - left[i]);
            head = left[i];
        }

        cout << " -> 0";
        total += abs(head - 0);
        head = 0;

        for (int r : right) {
            cout << " -> " << r;
            total += abs(head - r);
            head = r;
        }
    }

    cout << "\nTotal Seek Time (SCAN): " << total << endl;
    return total;
}


// C-SCAN (FIXED)
int CSCAN(vector<int> req, int head, int disk_size, string direction) {
    int total = 0;

    vector<int> left, right;

    for (int r : req) {
        if (r < head) left.push_back(r);
        else right.push_back(r);
    }

    sort(left.begin(), left.end());
    sort(right.begin(), right.end());

    cout << "\nHead Movement (C-SCAN): " << head;

    if (direction == "right") {

        // Move right
        for (int r : right) {
            cout << " -> " << r;
            total += abs(head - r);
            head = r;
        }

        // Go to end
        cout << " -> " << (disk_size - 1);
        total += abs(head - (disk_size - 1));
        head = disk_size - 1;

        // Jump to start (NO seek counted)
        cout << " -> 0";
        head = 0;

        // Continue
        for (int r : left) {
            cout << " -> " << r;
            total += abs(head - r);
            head = r;
        }
    }
    else {

        // Move left
        for (int i = left.size() - 1; i >= 0; i--) {
            cout << " -> " << left[i];
            total += abs(head - left[i]);
            head = left[i];
        }

        // Go to start
        cout << " -> 0";
        total += abs(head - 0);
        head = 0;

        // Jump to end (NO seek counted)
        cout << " -> " << (disk_size - 1);
        head = disk_size - 1;

        // Continue
        for (int i = right.size() - 1; i >= 0; i--) {
            cout << " -> " << right[i];
            total += abs(head - right[i]);
            head = right[i];
        }
    }

    cout << "\nTotal Seek Time (C-SCAN): " << total << endl;
    return total;
}


// ===================== MAIN =====================
int main() {

    int n, head, disk_size, choice;
    string direction;

    cout << "Enter disk size: ";
    cin >> disk_size;

    cout << "Enter number of requests: ";
    cin >> n;

    vector<int> req(n);

    // Request validation
    for (int i = 0; i < n; i++) {
        while (true) {
            cout << "Request " << i + 1 << ": ";
            cin >> req[i];

            if (req[i] >= 0 && req[i] < disk_size) break;
            else cout << "Invalid! Enter 0 to " << disk_size - 1 << endl;
        }
    }

    // Head validation
    while (true) {
        cout << "Enter initial head position: ";
        cin >> head;

        if (head >= 0 && head < disk_size) break;
        else cout << "Invalid! Enter 0 to " << disk_size - 1 << endl;
    }

    // Direction validation
    while (true) {
        cout << "Enter direction (left/right): ";
        cin >> direction;

        if (direction == "left" || direction == "right") break;
        else cout << "Invalid! Enter left or right\n";
    }

    // Menu
    do {
        cout << "\n\n===== MENU =====\n";
        cout << "1. FCFS\n2. SSTF\n3. SCAN\n4. C-SCAN\n5. Compare All\n6. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

            case 1: FCFS(req, head); break;
            case 2: SSTF(req, head); break;
            case 3: SCAN(req, head, disk_size, direction); break;
            case 4: CSCAN(req, head, disk_size, direction); break;

            case 5: {
                int fcfs = FCFS(req, head);
                int sstf = SSTF(req, head);
                int scan = SCAN(req, head, disk_size, direction);
                int cscan = CSCAN(req, head, disk_size, direction);

                cout << "\n===== COMPARISON =====\n";
                cout << "FCFS   : " << fcfs << endl;
                cout << "SSTF   : " << sstf << endl;
                cout << "SCAN   : " << scan << endl;
                cout << "C-SCAN : " << cscan << endl;
                break;
            }

            case 6: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice!\n";
        }

    } while (choice != 6);

    return 0;
}