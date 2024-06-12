#include <iostream>
#include <vector>
#include <memory>

using namespace std;

class Account {
protected:
    int accountNumber;
    double balance;

public:
    Account(int num, double initialBalance) : accountNumber(num), balance(initialBalance) {}

    virtual void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            cout << "Deposited: " << amount << " New Balance: " << balance << endl;
        } else {
            cout << "Invalid deposit amount." << endl;
        }
    }

    virtual bool withdraw(double amount) = 0; //  virtual function

    virtual void display() const {
        cout << "Account Number: " << accountNumber << " Balance: " << balance << endl;
    }

    virtual ~Account() {} // Virtual destructor to cleanup of derived classes
};

class CheckingAccount : public Account {
public:
    CheckingAccount(int num, double initialBalance) : Account(num, initialBalance) {}

    bool withdraw(double amount) override {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            cout << "Withdrawn: " << amount << " New Balance: " << balance << endl;
            return true;
        } else {
            cout << "Insufficient balance or invalid amount." << endl;
            return false;
        }
    }
};

class SavingsAccount : public Account {
public:
    SavingsAccount(int num, double initialBalance) : Account(num, initialBalance) {}

    bool withdraw(double amount) override {
        if (amount > 0 && amount <= (balance - 50)) { // Maintains a minimum balance of $50
            balance -= amount;
            cout << "Withdrawn: " << amount << " New Balance: " << balance << endl;
            return true;
        } else {
            cout << "Insufficient balance (min. $50 required) or invalid amount." << endl;
            return false;
        }
    }
};

class AccountManager {
private:
    vector<unique_ptr<Account>> accounts;

public:
    void addAccount(unique_ptr<Account> account) {
        accounts.push_back(move(account));
    }

    void displayAccounts() {
        for (const auto& account : accounts) {
            account->display();
        }
    }

    Account* getAccount(int accountNumber) {
        for (auto& account : accounts) {
            if (account->accountNumber == accountNumber) {
                return account.get();
            }
        }
        return nullptr;
    }
};

int main() {
    AccountManager manager;
    manager.addAccount(make_unique<CheckingAccount>(101, 500));
    manager.addAccount(make_unique<SavingsAccount>(102, 500));

    auto account = manager.getAccount(101);
    if (account) {
        account->deposit(200);
        account->withdraw(150);
    }

    account = manager.getAccount(102);
    if (account) {
        account->deposit(300);
        account->withdraw(100); // Should fail due to minimum balance requirement
    }

    manager.displayAccounts();

    return 0;
}
