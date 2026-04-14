#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <unordered_map>
#include <iomanip>
#include <climits>
#include <string>
using namespace std;

/*
    Prints horizontal line for box table
*/
void printLine(int cols) {
    for (int i = 0; i < cols; i++)   // loop for number of columns
        cout << "--------";          // print fixed width line
    cout << "\n";                   // move to next line
}

/*
    Prints table in box format
*/
void printTable(vector<vector<int>> &table, vector<int> &pages, vector<bool> &fault) {
    int frames = table.size();      // number of rows (frames)
    int n = pages.size();           // number of columns (pages)

    printLine(n + 1);               // top border

    // Print reference string row
    cout << "|Ref  |";
    for (int i = 0; i < n; i++)
        cout << setw(5) << pages[i] << " |";  // setw aligns output
    cout << "\n";

    printLine(n + 1);

    // Print frame rows
    for (int i = 0; i < frames; i++) {
        cout << "|F" << i+1 << "   |";   // frame label
        for (int j = 0; j < n; j++) {
            if (table[i][j] == -1)       // -1 means empty
                cout << setw(5) << "-" << " |";
            else
                cout << setw(5) << table[i][j] << " |";
        }
        cout << "\n";
    }

    printLine(n + 1);

    // Print fault row
    cout << "|Flt  |";
    for (int i = 0; i < n; i++) {
        if (fault[i]) cout << setw(5) << "F" << " |";  // F means fault
        else cout << setw(5) << " " << " |";           // blank means hit
    }
    cout << "\n";

    printLine(n + 1);
}

/*
    FIFO Algorithm
*/
int FIFO(vector<int> pages, int capacity) {
    int n = pages.size();   // total pages
    vector<vector<int>> table(capacity, vector<int>(n, -1)); // table initialized with -1
    vector<bool> fault(n, false); // track faults

    queue<int> q;  // stores order of pages (FIFO)
    set<int> s;    // quick lookup if page exists
    int faults = 0;

    for (int i = 0; i < n; i++) {

        // If page not present → page fault
        if (s.find(pages[i]) == s.end()) {
            faults++;
            fault[i] = true;

            // If memory full → remove oldest page
            if (q.size() == capacity) {
                s.erase(q.front()); // remove from set
                q.pop();            // remove from queue
            }

            q.push(pages[i]); // insert new page
            s.insert(pages[i]);
        }

        // Copy queue to table for display
        queue<int> temp = q;   // copy queue
        vector<int> curr;

        while (!temp.empty()) {
            curr.push_back(temp.front()); // store elements
            temp.pop();
        }

        // Fill table column
        for (int j = 0; j < curr.size(); j++)
            table[j][i] = curr[j];

        // Fill remaining with previous column values
        for (int j = curr.size(); j < capacity; j++) {
            if (i > 0)
                table[j][i] = table[j][i-1];
        }
    }

    cout << "\n=== FIFO (FCFS) ===\n";
    printTable(table, pages, fault);

    int hits = n - faults; // hits = total - faults

    cout << "Total Faults = " << faults << "\n";
    cout << "Total Hits = " << hits << "\n";
    cout << "Hit Rate = " << (double)hits/n << "\n";
    cout << "Miss Rate = " << (double)faults/n << "\n";

    return faults;
}

/*
    LRU Algorithm
*/
int LRU(vector<int> pages, int capacity) {
    int n = pages.size();
    vector<vector<int>> table(capacity, vector<int>(n, -1));
    vector<bool> fault(n, false);

    vector<int> frames;  // stores pages in memory
    unordered_map<int, int> lastUsed; // stores last used index
    int faults = 0;

    for (int i = 0; i < n; i++) {

        // If page not found → fault
        if (find(frames.begin(), frames.end(), pages[i]) == frames.end()) {
            faults++;
            fault[i] = true;

            // If full → remove least recently used
            if (frames.size() == capacity) {
                int lru = frames[0];

                // Find page with smallest last used index
                for (int x : frames) {
                    if (lastUsed[x] < lastUsed[lru])
                        lru = x;
                }

                // Remove LRU page
                frames.erase(find(frames.begin(), frames.end(), lru));
            }

            frames.push_back(pages[i]); // insert new page
        }

        lastUsed[pages[i]] = i; // update last used time

        // Store in table
        for (int j = 0; j < frames.size(); j++)
            table[j][i] = frames[j];

        for (int j = frames.size(); j < capacity; j++) {
            if (i > 0)
                table[j][i] = table[j][i-1];
        }
    }

    cout << "\n=== LRU ===\n";
    printTable(table, pages, fault);

    int hits = n - faults;

    cout << "Total Faults = " << faults << "\n";
    cout << "Total Hits = " << hits << "\n";
    cout << "Hit Rate = " << (double)hits/n << "\n";
    cout << "Miss Rate = " << (double)faults/n << "\n";

    return faults;
}

/*
    OPTIMAL Algorithm
*/
int OPTIMAL(vector<int> pages, int capacity) {
    int n = pages.size();
    vector<vector<int>> table(capacity, vector<int>(n, -1));
    vector<bool> fault(n, false);

    vector<int> frames;
    int faults = 0;

    for (int i = 0; i < n; i++) {

        // If page not present → fault
        if (find(frames.begin(), frames.end(), pages[i]) == frames.end()) {
            faults++;
            fault[i] = true;

            // If full → find optimal page to replace
            if (frames.size() == capacity) {
                int index = -1, farthest = i;

                // Check future usage
                for (int j = 0; j < frames.size(); j++) {
                    int k;

                    for (k = i+1; k < n; k++) {
                        if (frames[j] == pages[k]) {
                            if (k > farthest) {
                                farthest = k;
                                index = j;
                            }
                            break;
                        }
                    }

                    // If page not used again → best to replace
                    if (k == n) {
                        index = j;
                        break;
                    }
                }

                frames[index] = pages[i]; // replace
            } else {
                frames.push_back(pages[i]); // add if space
            }
        }

        // Store state in table
        for (int j = 0; j < frames.size(); j++)
            table[j][i] = frames[j];

        for (int j = frames.size(); j < capacity; j++) {
            if (i > 0)
                table[j][i] = table[j][i-1];
        }
    }

    cout << "\n=== OPTIMAL ===\n";
    printTable(table, pages, fault);

    int hits = n - faults;

    cout << "Total Faults = " << faults << "\n";
    cout << "Total Hits = " << hits << "\n";
    cout << "Hit Rate = " << (double)hits/n << "\n";
    cout << "Miss Rate = " << (double)faults/n << "\n";

    return faults;
}

/*
    MAIN FUNCTION
*/
int main() {
    int n, frames;

    cout << "Enter number of pages: ";
    cin >> n;   // user inputs total pages

    vector<int> pages(n);
    cout << "Enter reference string:\n";
    for (int i = 0; i < n; i++)
        cin >> pages[i];   // input pages

    cout << "Enter number of frames: ";
    cin >> frames;

    // Run all algorithms
    int f1 = FIFO(pages, frames);
    int f2 = LRU(pages, frames);
    int f3 = OPTIMAL(pages, frames);

    // Comparison
    cout << "\n===== FINAL COMPARISON =====\n";
    cout << "FIFO Faults: " << f1 << "\n";
    cout << "LRU Faults: " << f2 << "\n";
    cout << "OPTIMAL Faults: " << f3 << "\n";

    int best = min({f1, f2, f3}); // find minimum faults

    if (best == f3)
        cout << "Conclusion: OPTIMAL is best\n";
    else if (best == f2)
        cout << "Conclusion: LRU is best\n";
    else
        cout << "Conclusion: FIFO is best\n";

    return 0;
}