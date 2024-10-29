#include <iostream>
#include <iomanip>
using namespace std;

// Base Class: Account
class Account {
protected:
    double balance;
    int deposits;
    int withdrawals;
    double interestRate;
    double serviceCharges;
    bool status;

public:
    // Constructor
    Account() {
        balance = 0.00;
        interestRate = 0.05;  // Annual interest
        withdrawals = 0;
        deposits = 0;
        serviceCharges = 0.00;
        status = true;	      // Active by default
    }

    // Virtual functions for deposit, withdraw, interest calculation, and monthly processing
    virtual void deposit(double amount) {
        balance += amount;
        deposits++;
    }

    virtual void withdraw(double amount) {
        if (balance >= amount) {
            balance -= amount;
            withdrawals++;
        } else {
            cout << "Insufficient funds!" << endl;
        }
    }

    virtual void calcInt() {
        double monthlyInterestRate = interestRate / 12;
        double monthlyInterest = balance * monthlyInterestRate;
        balance += monthlyInterest;
    }

    virtual void monthlyProc() {
        balance -= serviceCharges;
        calcInt();
        withdrawals = 0;
        deposits = 0;
        serviceCharges = 0;
    }

    void getMonthlyStats() const {
        cout << "Balance: $" << fixed << setprecision(2) << balance << endl;
        cout << "Deposits: " << deposits << endl;
        cout << "Withdrawals: " << withdrawals << endl;
        cout << "Service Charges: $" << serviceCharges << endl;
    }
};

// Derived Class: Savings
class Savings : public Account {
public:
    Savings() : Account() {}

    // Override withdraw function
    void withdraw(double amount) override {
        if (status && balance >= amount) {
            balance -= amount;
            withdrawals++;
            if (balance < 25) status = false; // Inactivate if balance < $25
        } else {
            cout << "Account inactive or insufficient funds." << endl;
        }
    }

    // Override deposit function
    void deposit(double amount) override {
        balance += amount;
        deposits++;
        if (balance >= 25) status = true; // Reactivate if balance >= $25
    }

    // Override monthly processing function
    void monthlyProc() override {
        if (withdrawals > 4) {
            serviceCharges += (withdrawals - 4); // $1 per extra withdrawal
        }
        Account::monthlyProc(); // Call base class function
        if (balance < 25) status = false; // Inactivate if balance < $25
    }
};

// Derived Class: Checking
class Checking : public Account {
public:
    Checking() : Account() {}

    // Override withdraw function
    void withdraw(double amount) override {
        if (balance >= amount) {
            balance -= amount;
            withdrawals++;
        } else {
            cout << "Overdraft! Applying $15 service charge." << endl;
            balance -= 15; // Apply overdraft charge
        }
    }

    // Override monthly processing function
    void monthlyProc() override {
        serviceCharges += 5 + (withdrawals * 0.10); // $5 + $0.10 per withdrawal
        Account::monthlyProc(); // Call base class function
    }
};

// Function prototypes
void dispatch(int choice, Savings &savings, Checking &checking);

int main() {
    Savings savings;
    Checking checking;
    int choice;

    do {
        cout << "\n\n******** BANK ACCOUNT MENU ********\n\n";
        cout << "1.  Savings Account Deposit\n";
        cout << "2.  Checking Account Deposit\n";
        cout << "3.  Savings Account Withdrawal\n";
        cout << "4.  Checking Account Withdrawal\n";
        cout << "5.  Update and Display Account Statistics\n";
        cout << "6.  Exit\n\n";
        cout << "Your choice, please: (1-6)  ";
        cin >> choice;

        while (choice < 1 || choice > 6) {
            cout << "Enter a number from 1 through 6 please: ";
            cin >> choice;
        }

        dispatch(choice, savings, checking);
    } while (choice != 6);

    return 0;
}

// Function to handle user menu choices
void dispatch(int choice, Savings &savings, Checking &checking) {
    double amount;

    switch (choice) {
        case 1:
            cout << "Enter amount to deposit: ";
            cin >> amount;
            savings.deposit(amount);
            break;

        case 2:
            cout << "Enter amount to deposit: ";
            cin >> amount;
            checking.deposit(amount);
            break;

        case 3:
            cout << "Enter amount to withdraw: ";
            cin >> amount;
            savings.withdraw(amount);
            break;

        case 4:
            cout << "Enter amount to withdraw: ";
            cin >> amount;
            checking.withdraw(amount);
            break;

        case 5:
            savings.monthlyProc();
            checking.monthlyProc();
            cout << "\nSavings Account Stats:\n";
            savings.getMonthlyStats();
            cout << "\nChecking Account Stats:\n";
            checking.getMonthlyStats();
            break;

        case 6:
            cout << "Exiting program." << endl;
            break;
    }
}
