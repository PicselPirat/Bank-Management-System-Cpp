#include <iostream>
#include <fstream>
#include <iomanip>
#include <cctype>
using namespace std;

class Account {
private:
    int accountNumber;
    char name[50];
    char accountType;
    double balance;

public:
    void createAccount();
    void showAccount() const;
    void modifyAccount();
    void deposit(double);
    void withdraw(double);
    int getAccountNumber() const;
    double getBalance() const;
    char getAccountType() const;
};

void Account::createAccount() {
    cout << "\nEnter Account Number: ";
    cin >> accountNumber;
    cin.ignore();
    cout << "Enter Account Holder Name: ";
    cin.getline(name, 50);
    
    do {
        cout << "Enter Account Type (C for Current / S for Savings): ";
        cin >> accountType;
        accountType = toupper(accountType);
    } while (accountType != 'C' && accountType != 'S');

    do {
        cout << "Enter Initial Deposit (>=500 for S, >=1000 for C): ";
        cin >> balance;
        if ((accountType == 'S' && balance < 500) || (accountType == 'C' && balance < 1000))
            cout << "❌ Minimum balance not met. Please try again.\n";
    } while ((accountType == 'S' && balance < 500) || (accountType == 'C' && balance < 1000));

    cout << "\n✅ Account Created Successfully!\n";
}

void Account::showAccount() const {
    cout << "\nAccount No: " << accountNumber;
    cout << "\nName: " << name;
    cout << "\nType: " << accountType;
    cout << "\nBalance: ₹" << fixed << setprecision(2) << balance << endl;
}

void Account::modifyAccount() {
    cout << "\nModify Account Holder Name: ";
    cin.ignore();
    cin.getline(name, 50);
    
    do {
        cout << "Modify Account Type (C/S): ";
        cin >> accountType;
        accountType = toupper(accountType);
    } while (accountType != 'C' && accountType != 'S');

    do {
        cout << "Modify Balance (>=500 for S, >=1000 for C): ";
        cin >> balance;
        if ((accountType == 'S' && balance < 500) || (accountType == 'C' && balance < 1000))
            cout << "❌ Invalid. Try again.\n";
    } while ((accountType == 'S' && balance < 500) || (accountType == 'C' && balance < 1000));
}

void Account::deposit(double amount) {
    if (amount <= 0) {
        cout << "\n❌ Invalid Deposit Amount!";
        return;
    }
    balance += amount;
}

void Account::withdraw(double amount) {
    if (amount <= 0) {
        cout << "\n❌ Invalid Withdrawal Amount!";
        return;
    }
    double minBal = (accountType == 'S') ? 500 : 1000;
    if (balance - amount < minBal) {
        cout << "\n❌ Withdrawal Denied! Minimum balance must be maintained.";
    } else {
        balance -= amount;
    }
}

int Account::getAccountNumber() const { return accountNumber; }
double Account::getBalance() const { return balance; }
char Account::getAccountType() const { return accountType; }

// File Functions (same as before, no logic change required)
void writeAccount() {
    Account ac;
    ofstream outFile("accounts.dat", ios::binary | ios::app);
    ac.createAccount();
    outFile.write(reinterpret_cast<char*>(&ac), sizeof(Account));
    outFile.close();
}

void displayAccount(int n) {
    Account ac;
    ifstream inFile("accounts.dat", ios::binary);
    bool found = false;
    while (inFile.read(reinterpret_cast<char*>(&ac), sizeof(Account))) {
        if (ac.getAccountNumber() == n) {
            ac.showAccount();
            found = true;
        }
    }
    inFile.close();
    if (!found)
        cout << "\n❌ Account Not Found!";
}

void modifyAccount(int n) {
    Account ac;
    fstream file("accounts.dat", ios::binary | ios::in | ios::out);
    bool found = false;
    while (!file.eof() && !found) {
        streampos pos = file.tellg();
        file.read(reinterpret_cast<char*>(&ac), sizeof(Account));
        if (ac.getAccountNumber() == n) {
            ac.showAccount();
            cout << "\nEnter New Details:\n";
            ac.modifyAccount();
            file.seekp(pos);
            file.write(reinterpret_cast<char*>(&ac), sizeof(Account));
            cout << "\n✅ Account Updated!";
            found = true;
        }
    }
    file.close();
    if (!found)
        cout << "\n❌ Account Not Found!";
}

void deleteAccount(int n) {
    Account ac;
    ifstream inFile("accounts.dat", ios::binary);
    ofstream outFile("temp.dat", ios::binary);
    while (inFile.read(reinterpret_cast<char*>(&ac), sizeof(Account))) {
        if (ac.getAccountNumber() != n)
            outFile.write(reinterpret_cast<char*>(&ac), sizeof(Account));
    }
    inFile.close();
    outFile.close();
    remove("accounts.dat");
    rename("temp.dat", "accounts.dat");
    cout << "\n✅ Account Deleted!";
}

void displayAll() {
    Account ac;
    ifstream inFile("accounts.dat", ios::binary);
    cout << "\nAll Accounts:\n";
    cout << left << setw(12) << "Acc No" << setw(20) << "Name" << setw(8) << "Type" << setw(12) << "Balance\n";
    while (inFile.read(reinterpret_cast<char*>(&ac), sizeof(Account))) {
        cout << left << setw(12) << ac.getAccountNumber()
             << setw(20) << ac.getAccountType()
             << setw(8) << ac.getAccountType()
             << setw(12) << ac.getBalance() << endl;
    }
    inFile.close();
}

void depositWithdraw(int n, int option) {
    Account ac;
    fstream file("accounts.dat", ios::binary | ios::in | ios::out);
    bool found = false;
    while (!file.eof() && !found) {
        streampos pos = file.tellg();
        file.read(reinterpret_cast<char*>(&ac), sizeof(Account));
        if (ac.getAccountNumber() == n) {
            ac.showAccount();
            double amount;
            if (option == 1) {
                cout << "\nEnter Amount to Deposit: ₹";
                cin >> amount;
                ac.deposit(amount);
            } else {
                cout << "\nEnter Amount to Withdraw: ₹";
                cin >> amount;
                ac.withdraw(amount);
            }
            file.seekp(pos);
            file.write(reinterpret_cast<char*>(&ac), sizeof(Account));
            cout << "\n✅ Transaction Completed!";
            found = true;
        }
    }
    file.close();
    if (!found)
        cout << "\n❌ Account Not Found!";
}

int main() {
    int choice, num;
    do {
        cout << "\n\n====== Bank Management System ======\n";
        cout << "1. New Account\n";
        cout << "2. View Account\n";
        cout << "3. Deposit Money\n";
        cout << "4. Withdraw Money\n";
        cout << "5. View All Accounts\n";
        cout << "6. Delete Account\n";
        cout << "7. Modify Account\n";
        cout << "8. Exit\n";
        cout << "Enter your choice (1-8): ";
        cin >> choice;

        switch (choice) {
            case 1: writeAccount(); break;
            case 2: cout << "\nEnter Account Number: "; cin >> num; displayAccount(num); break;
            case 3: cout << "\nEnter Account Number: "; cin >> num; depositWithdraw(num, 1); break;
            case 4: cout << "\nEnter Account Number: "; cin >> num; depositWithdraw(num, 2); break;
            case 5: displayAll(); break;
            case 6: cout << "\nEnter Account Number: "; cin >> num; deleteAccount(num); break;
            case 7: cout << "\nEnter Account Number: "; cin >> num; modifyAccount(num); break;
            case 8: cout << "\nThank you for using the system.\n"; break;
            default: cout << "\n❌ Invalid Choice. Try again.\n";
        }
    } while (choice != 8);

    return 0;
}
