#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <map>
#include <sstream>
#include <algorithm>

using namespace std;

// Function to check if the password meets all constraints
bool meetsConstraints(const string& password) {
    bool hasUpper = false, hasLower = false, hasDigit = false, hasSpecial = false;
    for (char ch : password) {
        if (isupper(ch)) hasUpper = true;
        if (islower(ch)) hasLower = true;
        if (isdigit(ch)) hasDigit = true;
        if (ispunct(ch)) hasSpecial = true;
    }
    return hasUpper && hasLower && hasDigit && hasSpecial;
}

// Function to generate a random password with constraints
string generatePassword(int length) {
    string characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                        "abcdefghijklmnopqrstuvwxyz"
                        "0123456789"
                        "!@#$%^&*()_+";
    string password;

    srand(time(0)); // Seed for random number generator

    do {
        password.clear();
        for (int i = 0; i < length; ++i) {
            password += characters[rand() % characters.size()];
        }
    } while (!meetsConstraints(password));

    return password;
}

// Function to add a new password
void addPassword(const string& filename) {
    string identifier, password, pin;
    int length;

    cout << "Enter identifier (e.g., website or application name): ";
    cin >> identifier;
    cout << "Enter desired password length (minimum 8): ";
    cin >> length;

    if (length < 8) {
        cout << "Password length must be at least 8.\n";
        return;
    }

    password = generatePassword(length);
    cout << "Enter a 4-digit PIN: ";
    cin >> pin;

    if (pin.length() != 4 || !all_of(pin.begin(), pin.end(), ::isdigit)) {
        cout << "Invalid PIN. It must be exactly 4 digits.\n";
        return;
    }

    ofstream outfile(filename, ios::app);
    if (outfile.is_open()) {
        outfile << identifier << " " << password << " " << pin << endl;
        outfile.close();
        cout << "Password added successfully.\n";
    } else {
        cerr << "Error opening file.\n";
    }
}

// Function to retrieve a password
void retrievePassword(const string& filename) {
    string identifier, enteredPin, line, storedIdentifier, storedPassword, storedPin;
    cout << "Enter identifier to retrieve password: ";
    cin >> identifier;
    cout << "Enter your 4-digit PIN: ";
    cin >> enteredPin;

    ifstream infile(filename);
    if (infile.is_open()) {
        bool found = false;
        while (getline(infile, line)) {
            istringstream iss(line);
            iss >> storedIdentifier; // Extract first string
            iss >> storedPassword;   // Extract second string
            iss >> storedPin;        // Extract third string
            if (storedIdentifier == identifier && storedPin == enteredPin) {
                cout << "Password for " << identifier << " is: " << storedPassword << endl;
                found = true;
                break;
            }
        }
        infile.close();
        if (!found) {
            cout << "Identifier not found or incorrect PIN.\n";
        }
    } else {
        cerr << "Error opening file.\n";
    }
}

// Function to list all stored passwords (without PINs)
void listPasswords(const string& filename) {
    ifstream infile(filename);
    string line, identifier, password;

    if (infile.is_open()) {
        while (getline(infile, line)) {
            istringstream iss(line);
            iss >> identifier; // Extract first string
            iss >> password;   // Extract second string
            cout << "Identifier: " << identifier << ", Password: " << password << endl;
        }
        infile.close();
    } else {
        cerr << "Error opening file.\n";
    }
}

// Function to delete a password
void deletePassword(const string& filename) {
    string identifier, enteredPin, line, storedIdentifier, storedPassword, storedPin;
    cout << "Enter identifier to delete password: ";
    cin >> identifier;
    cout << "Enter your 4-digit PIN: ";
    cin >> enteredPin;

    ifstream infile(filename);
    ofstream tempFile("temp.txt");
    bool found = false;

    if (infile.is_open() && tempFile.is_open()) {
        while (getline(infile, line)) {
            istringstream iss(line);
            iss >> storedIdentifier; // Extract first string
            iss >> storedPassword;   // Extract second string
            iss >> storedPin;        // Extract third string
            if (storedIdentifier != identifier || storedPin != enteredPin) {
                tempFile << storedIdentifier << " " << storedPassword << " " << storedPin << endl;
            } else {
                found = true;
            }
        }
        infile.close();
        tempFile.close();

        remove(filename.c_str());
        rename("temp.txt", filename.c_str());

        if (found) {
            cout << "Password for " << identifier << " deleted successfully.\n";
        } else {
            cout << "Identifier not found or incorrect PIN.\n";
        }
    } else {
        cerr << "Error opening file.\n";
    }
}

int main() {
    string filename = "passwords.txt";
    int choice;

    do {
        cout << "\nPassword Manager\n";
        cout << "1. Add a new password\n";
        cout << "2. Retrieve a password\n";
        cout << "3. List all stored passwords\n";
        cout << "4. Delete a password\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                addPassword(filename);
                break;
            case 2:
                retrievePassword(filename);
                break;
            case 3:
                listPasswords(filename);
                break;
            case 4:
                deletePassword(filename);
                break;
            case 5:
                cout << "Exiting.\n";
                break;
            default:
                cout << "Invalid choice, please try again.\n";
        }
    } while (choice != 5);

    return 0;
}
