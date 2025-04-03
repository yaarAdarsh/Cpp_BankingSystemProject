#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <sstream>

class BankAccount {
private:
    int accountNumber;
    std::string customerName;
    double balance;
    int pin;

public:
    // Constructor
    BankAccount(int accNo, std::string name, double initialBalance, int pinCode)
        : accountNumber(accNo), customerName(name), balance(initialBalance), pin(pinCode) {}

    // Getters
    int getAccountNumber() const { return accountNumber; }
    std::string getCustomerName() const { return customerName; }
    double getBalance() const { return balance; }

    bool verifyPin(int enteredPin) const{
        return enteredPin == pin;
    }

    // Deposit function
    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            std::cout << "Deposit successful!\n";
        } else {
            std::cout << "Invalid deposit amount!\n";
        }
    }

    // Withdraw function
    bool withdraw(double amount, int enteredPin) {
        if(!verifyPin(enteredPin))
        {
            std::cout<<"Incorrect PIN!  Transaction denied.\n";
            return false;
        }
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            std::cout << "Withdrawal successful!\n";
            return true;
        } else {
            std::cout << "Insufficient balance or invalid amount!\n";
            return false;
        }
    }

    // Display account details
    void displayAccount() const {
        std::cout << "Account Number: " << accountNumber << "\n";
        std::cout << "Customer Name: " << customerName << "\n";
        std::cout << "Balance: $" << std::fixed << std::setprecision(2) << balance << "\n";
    }

    int getPin() const { return pin; }
};


class Bank {
private:
    std::vector<BankAccount> accounts;
    int nextAccountNumber;

public:
    Bank() : nextAccountNumber(1000) {}  // Initialize with starting account number 1000

    // Create a new account
    void createAccount() {
        std::string name;
        double initialDeposit;
        int pin;

        std::cout << "Enter Customer Name: ";
        std::cin.ignore(); // To ignore leftover newline character
        std::getline(std::cin, name);
        std::cout << "Enter Initial Deposit: ";
        std::cin >> initialDeposit;

        // Set a 4 - digit pin
        do {
            std::cout << "Set a 4-digit PIN: ";
            std::cin >> pin;
            if (pin < 1000 || pin > 9999) {
                std::cout << "Invalid PIN! Please enter a 4-digit number.\n";
            }
        } while (pin < 1000 || pin > 9999);

        // Create a new account with a unique account number
        BankAccount newAccount(nextAccountNumber, name, initialDeposit, pin);
        nextAccountNumber++;  // Increment the account number for the next account

        accounts.push_back(newAccount);

        std::cout << "Account created successfully! Your account number is " 
                  << newAccount.getAccountNumber() << ".\n";
    }

    // Search for an account by account number
    BankAccount* searchAccount(int accountNumber) {
        for (auto& account : accounts) {
            if (account.getAccountNumber() == accountNumber) {
                return &account;
            }
        }
        return nullptr;
    }

    // Deposit to an account
    void depositToAccount(int accountNumber, double amount) {
        BankAccount* account = searchAccount(accountNumber);
        if (account) {
            account->deposit(amount);
        } else {
            std::cout << "Account not found!\n";
        }
    }

    // Withdraw from an account
    void withdrawFromAccount(int accountNumber, double amount) {
        BankAccount* account = searchAccount(accountNumber);
        if (account) {
            int enteredPin;
            std::cout<<"Enter PIN: ";
            std::cin>>enteredPin;

            account->withdraw(amount, enteredPin);
        } else {
            std::cout << "Account not found!\n";
        }
    }

    // Transfer funds between accounts
    void transferFunds(int fromAccount, int toAccount, double amount) {
        BankAccount* fromAcc = searchAccount(fromAccount);
        BankAccount* toAcc = searchAccount(toAccount);

        if (fromAcc && toAcc) {
            int enteredPin;
            std::cout << "Enter PIN for Account " << fromAccount << ": ";
            std::cin >> enteredPin;
            if(fromAcc->verifyPin(enteredPin)){
                if (fromAcc->withdraw(amount, enteredPin)) {
                    toAcc->deposit(amount);
                    std::cout << "Transfer successful!\n";
                }
            }
            else{
                std::cout << "Incorrect PIN! Transfer denied.\n";
            }
        } else {
            std::cout << "One or both accounts not found!\n";
        }
    }

    // Display all accounts
    void displayAllAccounts() {
        if (accounts.empty()) {
            std::cout << "No accounts available.\n";
        } else {
            for (const auto& account : accounts) {
                account.displayAccount();
                std::cout << "--------------------------\n";
            }
        }
    }

    // File handling functions
    void saveToFile() {
        std::ofstream outFile("bank_accounts.txt");
        if (outFile.is_open()) {
            for (const auto& account : accounts) {
                outFile << account.getAccountNumber() << " "
                        << account.getCustomerName() << " "
                        << account.getBalance() << " "
                        << account.getPin() << "\n";
            }
            outFile << nextAccountNumber << "\n";  // Save the next account number at the end
            outFile.close();
            std::cout << "Accounts saved to file successfully!\n";
        } else {
            std::cout << "Error opening file for saving.\n";
        }
    }

    void loadFromFile() {
        std::ifstream inFile("bank_accounts.txt");
        if (inFile.is_open()) {
            accounts.clear();
            int accountNumber, pin;
            std::string name;
            double balance;

            std::string line;
            while (std::getline(inFile, line)) {
                std::istringstream iss(line);

                if (iss >> accountNumber >> name >> balance >> pin) {
                    // This is an account entry, so add it to the accounts vector
                    accounts.push_back(BankAccount(accountNumber, name, balance, pin));
                } else {
                    // This line contains the next account number
                    iss.clear();
                    iss.str(line);
                    iss >> nextAccountNumber;
                }
            }
            inFile.close();
            std::cout << "Accounts loaded from file successfully! Next account number: " << nextAccountNumber << "\n";
        } else {
            std::cout << "Error opening file for loading.\n";
        }
    }
};


int main() {
    Bank bank;
    bank.loadFromFile(); // Load accounts from file at the start

    int choice;
    do {
        std::cout << "***** Welcome to Banking System *****\n";
        std::cout << "1. Create a New Account\n";
        std::cout << "2. Deposit Money\n";
        std::cout << "3. Withdraw Money\n";
        std::cout << "4. Balance Inquiry\n";
        std::cout << "5. Fund Transfer\n";
        std::cout << "6. View All Accounts\n";
        std::cout << "7. Save and Exit\n";
        std::cout << "-------------------------------------\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                bank.createAccount();
                break;
            case 2: {
                int accountNumber;
                double amount;
                std::cout << "Enter Account Number: ";
                std::cin >> accountNumber;
                std::cout << "Enter Deposit Amount: ";
                std::cin >> amount;
                bank.depositToAccount(accountNumber, amount);
                break;
            }
            case 3: {
                int accountNumber;
                double amount;
                std::cout << "Enter Account Number: ";
                std::cin >> accountNumber;
                std::cout << "Enter Withdrawal Amount: ";
                std::cin >> amount;
                bank.withdrawFromAccount(accountNumber, amount);
                break;
            }
            case 4: {
                int accountNumber;
                std::cout << "Enter Account Number: ";
                std::cin >> accountNumber;
                BankAccount* account = bank.searchAccount(accountNumber);
                if (account) {
                    account->displayAccount();
                } else {
                    std::cout << "Account not found!\n";
                }
                break;
            }
            case 5: {
                int fromAccount, toAccount;
                double amount;
                std::cout << "Enter Source Account Number: ";
                std::cin >> fromAccount;
                std::cout << "Enter Destination Account Number: ";
                std::cin >> toAccount;
                std::cout << "Enter Transfer Amount: ";
                std::cin >> amount;
                bank.transferFunds(fromAccount, toAccount, amount);
                break;
            }
            case 6:
                bank.displayAllAccounts();
                break;
            case 7:
                bank.saveToFile();  // Save accounts and nextAccountNumber to file
                std::cout << "Exiting...\n";
                break;
            default:
                std::cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 7);

    return 0;
}
