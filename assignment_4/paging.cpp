#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iomanip>
using namespace std;

/*
    Prints horizontal line for table
*/
void printLine(int cols) {
    for (int i = 0; i < cols; i++)
        cout << "--------";
    cout << "\n";
}

/*
    Prints table in box format
*/
void printTable(vector<vector<int>> &table, vector<int> &pages, vector<bool> &fault) {
    int frames = table.size();
    int n = pages.size();

    printLine(n + 1);

    cout << "|Ref  |";
    for (int i = 0; i < n; i++)
        cout << setw(5) << pages[i] << " |";
    cout << "\n";

    printLine(n + 1);

    for (int i = 0; i < frames; i++) {
        cout << "|F" << i+1 << "   |";
        for (int j = 0; j < n; j++) {
            if (table[i][j] == -1)
                cout << setw(5) << "-" << " |";
            else
                cout << setw(5) << table[i][j] << " |";
        }
        cout << "\n";
    }

    printLine(n + 1);

    cout << "|Flt  |";
    for (int i = 0; i < n; i++) {
        if (fault[i]) cout << setw(5) << "F" << " |";
        else cout << setw(5) << " " << " |";
    }
    cout << "\n";

    printLine(n + 1);
}

// ---------------- FIFO ----------------
int FIFO(vector<int> pages, int frame_count) {

    vector<int> frames(frame_count, -1);   // fixed positions
    vector<vector<int>> table(frame_count, vector<int>(pages.size(), -1));
    vector<bool> fault(pages.size(), false);

    int faults = 0;
    int index = 0; // circular replacement

    for (int i = 0; i < pages.size(); i++) {
        int page = pages[i];
        bool found = false;

        // check hit
        for (int f : frames) {
            if (f == page) {
                found = true;
                break;
            }
        }

        if (!found) {
            frames[index] = page;               // replace FIXED position
            index = (index + 1) % frame_count; // circular move
            faults++;
            fault[i] = true;
        }

        // store snapshot (IMPORTANT)
        for (int j = 0; j < frame_count; j++) {
            table[j][i] = frames[j];
        }
    }

    cout << "\n=== FIFO ===\n";
    printTable(table, pages, fault);

    return faults;
}

// ---------------- LRU ----------------
int LRU(vector<int> pages, int frame_count) {

    vector<int> frames(frame_count, -1);
    vector<vector<int>> table(frame_count, vector<int>(pages.size(), -1));
    vector<bool> fault(pages.size(), false);

    unordered_map<int, int> recent;
    int faults = 0;

    for (int i = 0; i < pages.size(); i++) {
        int page = pages[i];
        bool found = false;

        for (int f : frames) {
            if (f == page) {
                found = true;
                break;
            }
        }

        if (!found) {
            faults++;
            fault[i] = true;

            int emptyIndex = -1;

            // find empty frame
            for (int j = 0; j < frame_count; j++) {
                if (frames[j] == -1) {
                    emptyIndex = j;
                    break;
                }
            }

            if (emptyIndex != -1) {
                frames[emptyIndex] = page;
            } else {
                int lruIndex = 0;
                int minRecent = recent[frames[0]];

                for (int j = 1; j < frame_count; j++) {
                    if (recent[frames[j]] < minRecent) {
                        minRecent = recent[frames[j]];
                        lruIndex = j;
                    }
                }

                frames[lruIndex] = page; // replace SAME position
            }
        }

        recent[page] = i;

        // store snapshot
        for (int j = 0; j < frame_count; j++) {
            table[j][i] = frames[j];
        }
    }

    cout << "\n=== LRU ===\n";
    printTable(table, pages, fault);

    return faults;
}

// ---------------- OPTIMAL ----------------
int OPTIMAL(vector<int> pages, int frame_count) {

    vector<int> frames(frame_count, -1);
    vector<vector<int>> table(frame_count, vector<int>(pages.size(), -1));
    vector<bool> fault(pages.size(), false);

    int faults = 0;

    for (int i = 0; i < pages.size(); i++) {
        int page = pages[i];
        bool found = false;

        for (int f : frames) {
            if (f == page) {
                found = true;
                break;
            }
        }

        if (!found) {
            faults++;
            fault[i] = true;

            int emptyIndex = -1;

            for (int j = 0; j < frame_count; j++) {
                if (frames[j] == -1) {
                    emptyIndex = j;
                    break;
                }
            }

            if (emptyIndex != -1) {
                frames[emptyIndex] = page;
            } else {
                int farthest = -1;
                int replaceIndex = -1;

                for (int j = 0; j < frame_count; j++) {
                    int k;
                    for (k = i + 1; k < pages.size(); k++) {
                        if (frames[j] == pages[k])
                            break;
                    }

                    if (k > farthest) {
                        farthest = k;
                        replaceIndex = j;
                    }
                }

                frames[replaceIndex] = page; // SAME position replace
            }
        }

        // store snapshot
        for (int j = 0; j < frame_count; j++) {
            table[j][i] = frames[j];
        }
    }

    cout << "\n=== OPTIMAL ===\n";
    printTable(table, pages, fault);

    return faults;
}

int main() {
    int n, frames;

    cout << "Enter number of pages: ";
    cin >> n;

    vector<int> pages(n);
    cout << "Enter reference string:\n";
    for (int i = 0; i < n; i++)
        cin >> pages[i];

    cout << "Enter number of frames: ";
    cin >> frames;

    // Run all algorithms and store faults
    int f1 = FIFO(pages, frames);
    int f2 = LRU(pages, frames);
    int f3 = OPTIMAL(pages, frames);

    // ---------------- COMPARISON TABLE ----------------
    cout << "\n===== COMPARISON TABLE =====\n";

    cout << "---------------------------------\n";
    cout << "| Algorithm | Page Faults |\n";
    cout << "---------------------------------\n";

    cout << "| FIFO      | " << setw(11) << f1 << " |\n";
    cout << "| LRU       | " << setw(11) << f2 << " |\n";
    cout << "| OPTIMAL   | " << setw(11) << f3 << " |\n";

    cout << "---------------------------------\n";

    // Best algorithm
    int best = min({f1, f2, f3});

    cout << "\nBest Algorithm: ";
    if (best == f3)
        cout << "OPTIMAL (Minimum Page Faults)\n";
    else if (best == f2)
        cout << "LRU (Minimum Page Faults)\n";
    else
        cout << "FIFO (Minimum Page Faults)\n";

    return 0;
}