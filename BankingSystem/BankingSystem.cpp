// BankingSystem.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <iomanip>

#define MIN_BALANCE 100

using namespace std;

class UnknownAccount {};

/* Account Class */

class Account {
private:
    static int NextAccountNumber;
    long number;
    string firstName;
    string lastName;
    float balance;
public:
    Account(string firstName="Unknown", string lastName="Unknown", float balance=0.0f)
    {
        NextAccountNumber++;
        number = NextAccountNumber;
        this->firstName = firstName;
        this->lastName = lastName;
        this->balance = balance;
    }
    
    // Accessors
    string getFirstName() const { return firstName; }
    string getLastName() const { return lastName; }
    
    long getAccNumber() const { return number; }
    static void setLastAccountNumber(long n) { NextAccountNumber = n; }

    void setBalance(float b) { balance = b; }
    float getBalance() const { return balance; }

    string getFullName() { return firstName + " " + lastName; }

    bool Deposit(float amount);
    bool Withdrawal(float amount);

    // Serialize - Override << operator to write the object's state into a file
    friend ofstream& operator <<(ofstream& ofs, Account& a);
    // Deserialize - Override >> operator to read the opbject's stat from the file
    friend ifstream& operator >>(ifstream& ifs, Account& a);
    // Override cout to print the object's state
    friend ostream& operator <<(ostream& out, const Account& a);
};

int Account::NextAccountNumber = 0;

bool Account::Deposit(float amount) {
    if (amount < 0) {
        cout << "The deposit cannot be less than 0." << endl;
        return false;
    } else {
        balance += amount;
        return true;
    }
}

bool Account::Withdrawal(float amount) {
    if (balance - amount < MIN_BALANCE) {
        cout << "The amount of the Withdrawal should be less than Balance." << endl;
        return false;
    } 

    balance -= amount;
    return true;
}

// Serialize - write 
ofstream& operator <<(ofstream& ofs, Account& a)
{
    ofs << a.firstName << endl;
    ofs << a.lastName << endl;
    ofs << a.number << endl;
    ofs << a.balance << endl;
    return ofs;
}

// Deserialize - read
ifstream& operator >>(ifstream& ifs, Account& a)
{
    ifs >> a.firstName;
    ifs >> a.lastName;
    ifs >> a.number;
    ifs >> a.balance;
    return ifs;
}

ostream& operator <<(ostream& out, const Account& a)
{
    cout << "First Name: " << a.firstName << endl;
    cout << "Last Name: " << a.lastName << endl;
    cout << "Account: " << a.number << endl;
    cout << "Balance: " << a.balance << "$" << endl;
    return out;
}


/* Bank Class */

class Bank {
private:
   map<long, Account> accounts;
   const string FILE_NAME = "account.txt";
public:
    Bank()
    {
        // Get accounts from the file
        readAccounts();
    }
    ~Bank() {
        saveAcounts();
    }
    
    Account findAccount() const throw(UnknownAccount);

    void OpenAccount();
    void BalanceEnquiry() const;
    void Deposit() const;
    void Withdrawal() const;
    void CloseAccount();
    void ShowAllAcounts();

    void readAccounts();
    void saveAcounts();
    void displayMenu() const;
};

Account Bank::findAccount() const throw(UnknownAccount) {
    long accNumber = 0;
    auto itr = accounts.find(accNumber);
    
    do {
        cout << "Enter the Number of the Account: ";
        cin >> accNumber;

        itr = accounts.find(accNumber);
        if (itr == accounts.end()) {
            cout << "There is no Account with Number: " << accNumber << endl
                 << "Please try again!" << endl;
        }

    } while (itr == accounts.end());

    return itr->second;
}

void Bank::OpenAccount() {
    string name, surname;
    float b = 0.0f;

    cout << "Enter First Name: ";
    cin >> name;
    cout << "Enter Last Name: ";
    cin >> surname;
    cout << "Enter the initial Balance: ";
    cin >> b;

    Account newAcc(name, surname, b);

    // Add new account to the list
    accounts.insert(pair<long, Account>(newAcc.getAccNumber(), newAcc));

    cout << "Congratulations, New Acount was Successfull Created!!!" << endl;
    cout << newAcc << endl << endl;
}

void Bank::BalanceEnquiry() const {

    auto acc = findAccount();
    
    cout << endl << "The Balance of the User:"
        << acc.getFullName() << " is: "
        << acc.getBalance() << "$" << endl;
}

void Bank::Deposit() const {
   
    auto acc = findAccount();
 
    // Found
    float amount = 0.0f;
    cout << "Enter the amount of the Deposit: ";
    cin >> amount;

    if (acc.Deposit(amount)) {
        cout << "The balance of the " << acc.getFullName()
            << " was increased by: +" << amount << "$" << endl
            << "Current balance is: " << acc.getBalance() << "$" << endl;
    }
}

void Bank::Withdrawal() const {

    auto acc = findAccount();
   
    // Found
    float amount = 0.0f;
    cout << "Enter the amount of the Withdrawal: ";
    cin >> amount;

    if (acc.Withdrawal(amount)) {
        cout << "The balance of the " << acc.getFullName()
            << " was decreased by: -" << amount << "$" << endl
            << "Current balance is: " << acc.getBalance() << "$" << endl;
    }
}

void Bank::CloseAccount() {

    long accNumber = 0;
    cout << "Enter the Number of the Account: ";
    cin >> accNumber;

    auto itr = accounts.find(accNumber);

    if (itr == accounts.end()) {
        cout << "There is no Acount with such number!" << endl;
        return;
    }
   
    accounts.erase(accNumber);
    cout << "The Account Number: " << accNumber << " was Successfully Closed!" << endl;
}

void Bank::ShowAllAcounts() {
    if (accounts.size() == 0) 
        cout << "The list of accounts is empty." << endl;
    else {
        for (auto account : accounts)
            cout << account.second << endl;
    }
}

void Bank::readAccounts() {
    ifstream ifs(FILE_NAME);
    Account temp;

    if (ifs.is_open())
        while (!ifs.eof()) 
        {
            // Read Account
            ifs >> temp;

            if (!ifs.eof()) {
                // Add the Account to the list
                accounts.insert(pair<long, Account>(temp.getAccNumber(), temp));
            }
        }

    // Set the Account Number to the last inserted 
    auto lastAccountNumber = accounts.end();
    long n;
    if (accounts.begin() != lastAccountNumber) {
        --lastAccountNumber;
        n = lastAccountNumber->first;
    }
    
    Account::setLastAccountNumber(n);

    ifs.close();
}

void Bank::saveAcounts() 
{
    ofstream ofs(FILE_NAME);

    for (auto account : accounts)
        ofs << account.second << endl;
 
    ofs.close();
}

void Bank::displayMenu() const
{
    cout << "\n\tSelect one option below:\n";
    cout << "\t1. Open an Account" << endl;
    cout << "\t2. Balance Enquiry" << endl;
    cout << "\t3. Deposit" << endl;
    cout << "\t4. Withdrawal" << endl;
    cout << "\t5. Close an Account" << endl;
    cout << "\t6. Show All Accounts" << endl;
    cout << "\t7. Quit" << endl;
}

int main()
{
    size_t choice = 0;
    Bank bank;

    while (choice != 7)
    {
        bank.displayMenu();

        cout << fixed << setprecision(2) << "\nEnter your choice: ";
        cin >> choice;

        switch (choice)
        {
            case 1: // Open an Account
                bank.OpenAccount();
                break;

            case 2: // Balance Enquiry 
                bank.BalanceEnquiry();
                break;

            case 3: // Deposit
                bank.Deposit();
                break;

            case 4: // Withdrawal
                bank.Withdrawal();
                break;

            case 5: // Close an Account
                bank.CloseAccount();
                break;

            case 6: // Show All 
                bank.ShowAllAcounts();
                break;

            case 7: // Quit
                cout << "Have a nice day!!!" << endl;
                break;

            default:
                cout << "Unknown option: " << choice << endl;
                break;
        }
    }
}