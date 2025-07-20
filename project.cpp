

#include <iostream>
#include <fstream>
#include <cstdlib> // for system("cls")
#include <unordered_map>
#include <string>
using namespace std;

#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define RESET   "\033[0m"


class Citizen {
public:
    string cnic;
    string name;
    int age;
    string city;
    string password;
    bool hasVoted = false;
    string votedFor = "";
    void display() const {
        cout << cnic;
        for (int i = cnic.length(); i < 18; i++) {
            cout << " ";
        }

        cout << name;
        for (int i = name.length(); i < 19; i++) {
            cout << " ";
        }

        cout << age;
        if (age < 10) {
            cout << "     ";
        }
        else if (age < 100) {
            cout << "    ";
        }
        else {
            cout << "   ";
        }

        cout << city;
        for (int i = city.length(); i < 16; i++) cout << " ";

        if (hasVoted) {
            cout << votedFor << endl;
        }
        else {
            cout << "No" << endl;
        }
    }
};

class Candidate {
public:
    string name;
    string party;
    int votes;
    Candidate* left;
    Candidate* right;

    Candidate(string n, string p, int v = 0) {
        name = n;
        party = p;
        votes = v;
        left = nullptr;
        right = nullptr;
    }
};

class CandidateTree {
public:
    Candidate* root;

    CandidateTree() { 
        root = nullptr;
    }

    Candidate* insert(Candidate* node, string name, string party, int votes = 0) {
        if (!node) {
            return new Candidate(name, party, votes);
        }
        if (name < node->name) {
            node->left = insert(node->left, name, party, votes);
        }
        else {
            node->right = insert(node->right, name, party, votes);
        }
        return node;
    }

    void insertCandidate(string name, string party, int votes = 0) {
        root = insert(root, name, party, votes);
    }

    void display() {
        if (!root) {
            cout << YELLOW << "No candidates available." << RESET << endl;
            return;
        }

        cout << YELLOW << "Candidate         Party               Votes" << RESET << endl;
        cout << "--------------------------------------------" << endl;
        printCandidates(root);
    }

    void printCandidates(Candidate* node) {
        if (!node) return;
        printCandidates(node->left);

        cout << node->name;
        for (int i = node->name.length(); i < 18; i++) cout << " ";

        cout << node->party;
        for (int i = node->party.length(); i < 20; i++) cout << " ";

        cout << node->votes << endl;

        printCandidates(node->right);
    }

    void saveToFile(Candidate* node, ofstream& fout) {
        if (!node) {
            return;
        }
        saveToFile(node->left, fout);
        fout << node->name << "," << node->party << "," << node->votes << endl;
        saveToFile(node->right, fout);
    }

    Candidate* search(Candidate* node, string name) {
        if (!node) {
            return nullptr;
        }
        if (name == node->name) {
            return node;
        }
        if (name < node->name) {
            return search(node->left, name);
        }
        else {
            return search(node->right, name);
        }
    }

    Candidate* findCandidate(string name) {
        return search(root, name);
    }
    Candidate* getMaxVotes(Candidate* node, Candidate*& candMax, int& maxVotes) {
        if (node == nullptr) {
            return nullptr;
        }

        getMaxVotes(node->left, candMax, maxVotes);

        if (node->votes > maxVotes) {
            maxVotes = node->votes;
            candMax = node;
        }

        getMaxVotes(node->right, candMax, maxVotes);

        return candMax;
    }
    Candidate* getMaxVotes() {
        Candidate* candMax = nullptr;
        int maxVotes = -1; 
        return getMaxVotes(root, candMax, maxVotes);
    }
};


unordered_map<string, Citizen> usersMap;

CandidateTree electionTree;


class RecentVoterQueue {
private:
    string data[10];
    int front, rear, count;

public:
    RecentVoterQueue() {
        front = 0;
        rear = -1;
        count = 0;
    }
    bool isEmpty()  {
        return count == 0;
    }
    void enqueue(const string& voterCNIC) {
        if (count == 10) {
            front = (front + 1) % 10;
            count--;
        }
        rear = (rear + 1) % 10;
        data[rear] = voterCNIC;
        count++;
    }

    void display(const unordered_map<string, Citizen>& usersMap) {
        if (count == 0) {
            cout << "No recent voters.\n";
            return;
        }
        cout << "\n--- Recent Voters ---\n";
        for (int i = 0; i < count; i++) {
            int index = (front + i) % 10;
            string cnic = data[index];
            auto it = usersMap.find(cnic);
            if (it != usersMap.end()) {
                cout << "- " << it->second.name << " from " << it->second.city << endl;
            }
        }
    }
};

RecentVoterQueue recentVoters;

void loadCitizensFromFile(string citizens) {
    ifstream fin(citizens);
    string cnic, name, ageStr, city, password, votedFor;
    while (getline(fin, cnic, ',')) {
        getline(fin, name, ',');
        getline(fin, ageStr, ',');
        getline(fin, city, ',');
        getline(fin, password, ',');
        getline(fin, votedFor);

        if (cnic.empty() || name.empty() || ageStr.empty() || city.empty())
            continue;

        Citizen c;
        c.cnic = cnic;
        c.name = name;
        c.age = stoi(ageStr);
        c.city = city;
        if (password == "-") {
            c.password = "";
        }
        else {
            c.password = password;
        }
        if (votedFor == "-") {
            c.votedFor = "";
        }
        else {
            c.votedFor = votedFor;
        }
        c.hasVoted = (c.votedFor != "");

        usersMap[c.cnic] = c;
    }
    fin.close();
}

void saveCitizensToFile(string citizens) {
    ofstream fout(citizens);
    for (auto& pair : usersMap) {
        Citizen c = pair.second;

        if (c.cnic.empty()) {
            continue;
        }

        if (c.password.empty()) {
            fout << c.cnic << ",";
            fout << c.name << ",";
            fout << c.age << ",";
            fout << c.city << ",";
            fout << "-" << ",";
            if (c.votedFor.empty()) {
                fout << "-" << endl;
            }
            else {
                fout << c.votedFor << endl;
            }
        }
        else {
            fout << c.cnic << ",";
            fout << c.name << ",";
            fout << c.age << ",";
            fout << c.city << ",";
            fout << c.password << ",";
            if (c.votedFor.empty()) {
                fout << "-" << endl;
            }
            else {
                fout << c.votedFor << endl;
            }
        }
    }
    fout.close();
}

void saveCandidatesToFile(string candidates) {
    ofstream fout(candidates);
    electionTree.saveToFile(electionTree.root, fout);
    fout.close();
}

void loadCandidatesFromFile(string candidates) {
    ifstream fin(candidates);
    string name;
    string party;
    string votesStr;
    while (getline(fin, name, ',')) {
        getline(fin, party, ',');
        getline(fin, votesStr);
        int votes = stoi(votesStr);
        electionTree.insertCandidate(name, party, votes);
    }
    fin.close();
}




void vote(string cnic) {
    if (usersMap[cnic].hasVoted) {
        cout << RED << "You have already voted." << RESET << endl;
        return;
    }

    cout << "--- Candidate List ---" << endl;
    electionTree.display();
    cout << "\nEnter candidate name to vote: ";
    cin.ignore();
    string voteFor;
    getline(cin, voteFor);

    Candidate* cand = electionTree.findCandidate(voteFor);
    if (!cand) {
        cout << RED << "Candidate not found." << RESET << endl;
        return;
    }

    cand->votes++;
    usersMap[cnic].hasVoted = true;
    usersMap[cnic].votedFor = cand->name;
    recentVoters.enqueue(cnic);
    cout << GREEN << "Vote cast successfully." << RESET << endl;
}

void displayTurnoutByCity() {
    cout << "--- Turnout by City ---" << endl;
    unordered_map<string, int> cityVotes;
    unordered_map<string, int> cityTotal;
    int totalCitizens = 0;
    int totalVoted = 0;

    for (auto& pair : usersMap) {
        string city = pair.second.city;
        cityTotal[city]++;
        totalCitizens++;
        if (pair.second.hasVoted) {
            cityVotes[city]++;
            totalVoted++;
        }
    }

    for (auto& entry : cityTotal) {
        string city = entry.first;
        int total = entry.second;
        int voted = cityVotes[city];
        float percent;
        if (total == 0) {
            percent = 0;
        }
        else {
            percent = (voted * 100.0 / total);
        }
        cout << city << ": " << voted << "/" << total << " (" << percent << "%)" << endl;
    }

    float totalPercent;
    if (totalCitizens == 0) {
        totalPercent = 0;
    }
    else {
        totalPercent = (totalVoted * 100.0 / totalCitizens);
    }
    cout << YELLOW << "Total Turnout: " << totalVoted << "/" << totalCitizens
        << " (" << totalPercent << "%)" << endl << RESET;
}

void adminPanel() {
    int choice;
    do {
        cout << "\n\n--- ADMIN PANEL ---" << endl;
        cout << "1. View Candidates" << endl;
        cout << "2. View Citizens" << endl; 
        cout << "3. View Election Results" << endl;
        cout << "4. View Recent Voters" << endl; 
        cout << "5. View Turnout by City" << endl; 
        cout << "6. Exit to Main Menu" << endl;
        cout << "Choice: ";
        cin >> choice;
        switch (choice) {
        case 1:
            electionTree.display();
            break;
        case 2:
            // View Citizens
            cout << YELLOW << "CNIC              Name               Age   City            Voted" << endl << RESET;
            cout << "---------------------------------------------------------------" << endl;
            for (auto& pair : usersMap) {
                pair.second.display();
            }
            break;

        case 3: {
            cout << "--- ELECTION RESULTS ---" << endl;
            electionTree.display();

            Candidate* maxCand= electionTree.getMaxVotes();

            if (maxCand)
                cout << YELLOW << ">> Leading: " << maxCand->name << " (" << maxCand->party << ") with " << maxCand->votes << " votes" << endl << RESET;

            int total = 0, voted = 0;
            for (auto& u : usersMap) {
                total++;
                if (u.second.hasVoted) {
                    voted++;
                }
            }

            break;
        }
        case 4: {
            if (recentVoters.isEmpty()) {
                cout << RED << "No Recent Voters" << endl << RESET;
                break;
            }

            recentVoters.display(usersMap);
            break;
        }

        case 5:
            displayTurnoutByCity();
            break;
        case 6:
            system("cls");
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
            break;
        
        }
    } while (choice != 6);
}

void voterPanel(string cnic) {
    int choice;
    do {
        cout << "\n\n--- VOTER PANEL ---" << endl;
        cout << "1. Vote" << endl; 
        cout << "2. View Voted Candidate" << endl; 
        cout << "3. View Results" << endl; 
        cout << "4. Exit to Main Menu" << endl; 
        cout << "Choice: ";
        cin >> choice;
        switch (choice) {
        case 1:
            vote(cnic);
            saveCitizensToFile("citizens.txt");
            saveCandidatesToFile("candidates.txt");
            break;
        case 2:
            if (!usersMap[cnic].votedFor.empty())
                cout << "You voted for: " << usersMap[cnic].votedFor << endl;
            else
                cout << YELLOW << "You haven't voted yet." << endl << RESET;
            break;
        case 3:
            electionTree.display();
            break;
        case 4:
            system("cls");

        default:
            cout << "Invalid choice. Please try again." << endl;
            break;
        }
    } while (choice != 4);
}


bool login(string& loggedCNIC) {
    for (int attempts = 0; attempts < 3; attempts++) {
        cout << "Enter CNIC: ";
        cin >> loggedCNIC;

        if (usersMap.find(loggedCNIC) == usersMap.end()) {
            cout << RED << "CNIC not found in records." << RESET << endl;
        }
        else {
            if (usersMap[loggedCNIC].password.empty()) {
                cout << "First-time login. Please set a password: ";
                string pw;
                cin >> pw;
                usersMap[loggedCNIC].password = pw;
                saveCitizensToFile("citizens.txt");
                cout << GREEN << "Password set successfully." << RESET << endl;
                return true;
            }
            else {
                cout << "Enter password: ";
                string pw;
                cin >> pw;
                if (pw == usersMap[loggedCNIC].password) {
                    cout << GREEN << "Login successful." << RESET << endl;
                    return true;
                }
                else {
                    cout << RED << "Incorrect password." << RESET << endl;
                }
            }
        }
    }
    cout << RED << "Too many attempts you failed. Back to main menu." << RESET << endl;
    return false;
}


int main() {
    loadCitizensFromFile("citizens.txt");
    loadCandidatesFromFile("candidates.txt");

    int choice;
    do {
        cout << "=== ONLINE VOTING SYSTEM ===" << endl;
        cout << "1. Admin Login" << endl; 
        cout << "2. Voter Login" << endl; 
        cout << "3. Exit" << endl; 
        cout << "Your Choice: ";
        cin >> choice;

        if (choice == 1) {
            string adminPass;
            int i=0;
            for (i = 0; i < 3; i++) {
                cout << "Enter admin password: ";
                cin >> adminPass;
                if (adminPass == "admin123") {
                    system("cls"); 
                    cout << GREEN << "Logged in as Admin" << endl << RESET;
                    adminPanel();
                    break;
                }
                else {
                    cout << RED << "Wrong Password. Access Denied." << endl << RESET;
                }
            }
            if(i==3){
                cout<<"\nToo many failed attempts. Returning to Main Menu\n\n";
            }
        }
        else if (choice == 2) {
            string userCNIC;
            if (login(userCNIC)) {
                Citizen& c = usersMap[userCNIC];
                if (c.age < 18) {
                    cout << RED << "You are under 18. You can only view results." << endl << RESET;
                    electionTree.display();
                }
                else {
                    voterPanel(userCNIC);
                }
            }
        }
        else if (choice != 3) {
            cout << RED << "Invalid choice. Please try again." << endl << RESET;
        }
    } while (choice != 3);

    saveCitizensToFile("citizens.txt");
    saveCandidatesToFile("candidates.txt");
    return 0;
}