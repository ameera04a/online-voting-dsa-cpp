\
# 🗳️ Online Voting System (DSA - C++)

This is a C++ based **Online Voting System** demonstrating key **Data Structures** like **Binary Search Trees**, **Unordered Maps**, and **Circular Queues**. The system allows two types of users: **Admins** and **Voters**, each with distinct functionality.

---

## 🔐 Admin Panel

After logging in with the password (`admin123`), the admin can:

### Features:

- **View Candidates:** Lists all candidates stored in a **Binary Search Tree**, sorted alphabetically.
- **View Citizens:** Displays all voters and whether they have voted.
- **View Election Results:** Shows vote count for each candidate.
- **View Recent Voters:** Maintains a **circular queue (size 10)** showing the names and cities of recent voters.
- **View Turnout by City:** Calculates and displays how many citizens in each city have voted using an **unordered map**.

---

## 🧑‍💻 Voter Panel

Once logged in with their CNIC and password, verified voters (age ≥ 18) can:


### Voter Features:

- **One-time Voting:** Voters can vote only once.
- **Password Setup:** First-time users are asked to set a password.
- **Candidate Display:** Candidates are shown in sorted order for selection.
- **Recent Voters:** Their CNIC is added to the recent voters queue.

---

## 📁 File Structure

### 1. `citizens.txt`  
Stores citizen records in this format:
35201-1234567-8,Ayesha Khan,22,Lahore,pass123,Ali Khan


### 2. `candidates.txt`  
Stores candidate information in this format:
Ali Khan,Liberty Party,2

---

## 🧠 Data Structures Used

- **Binary Search Tree (BST):**  
  Stores candidates in alphabetical order by name.

- **Unordered Map:**  
  `unordered_map<string, Citizen>` stores citizens with their CNIC as the key.

- **Circular Queue (size 10):**  
  Maintains a list of the **10 most recent voters**.

- **File I/O:**  
  All data (candidates and citizens) is loaded at startup and written back after updates.

---
## Live Link
[![Run on Replit](https://replit.com/badge/github/ameera04a/online-voting-dsa-cpp)](https://replit.com/new/github/ameera04a/online-voting-dsa-cpp)




