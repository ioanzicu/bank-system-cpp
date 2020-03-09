// BankingSystem.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>

#define MIN_BALANCE 100

using namespace std;

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
    
    // Accessors and Mutators
    void setFirstName(string n) { firstName = n; }
    string getFirstName() const { return firstName; }

    void setLastName(string s) { lastName = s; }
    string getLastName() const { return lastName; }
    
    long getNumber() const { return number; }

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
   vector<Account*> list;
   const string FILE_NAME = "account.txt";
public:
    Bank()
    {
        // Get accounts from the file
        readAccounts();
    }
    ~Bank() {
        saveAcounts();

        // Release Memory
        for (auto account : list) 
            delete account;
        
        list.clear();
    }
    
    int findAccount() const;

    void openAccount();
    void balanceEnquiry() const;
    void deposit() const;
    void withdrawal() const;
    void closeAccount();
    void showAllAcounts();
    void readAccounts();
    void saveAcounts();
    void displayMenu() const;
};

int Bank::findAccount() const {
    int accNumber = 0;
    cout << "Enter the Number of the Account: ";
    cin >> accNumber;

    auto itr = find_if(list.begin(), list.end(), [&accNumber](Account* const& obj) {return  obj->getNumber() == accNumber; });

    if (itr != list.end()) {
        // Element found - an iterator to the first matching element.
        return distance(list.begin(), itr); // return index
    } else {
        cout << "There is no Acount with such number!" << endl;
        return -1;
    }
}

void Bank::openAccount() {
    string name, surname;
    float b = 0.0f;

    cout << "Enter First Name: ";
    cin >> name;
    cout << "Enter Last Name: ";
    cin >> surname;
    cout << "Enter the initial Balance: ";
    cin >> b;

    // Add new account to the list
    list.push_back(new Account(name, surname, b));

    cout << "Congratulations, New Acount was Successfull Created!!!" << endl;
}

void Bank::balanceEnquiry() const {

    auto index = findAccount();
    // Account Not Found
    if (index == -1)
        return;

    cout << endl << "The Balance of the User:"
        << list[index]->getFullName() << " is: "
        << list[index]->getBalance() << "$" << endl;
}

void Bank::deposit() const {
   
    auto index = findAccount();
    // Account Not Found
    if (index == -1)
        return;

    // Found
    float amount = 0.0f;
    cout << "Enter the amount of the Deposit: ";
    cin >> amount;

    if (list[index]->Deposit(amount)) {
        cout << "The balance of the " << list[index]->getFullName()
            << " was increased by: +" << amount << "$" << endl
            << "Current balance is: " << list[index]->getBalance() << "$" << endl;
    }
}

void Bank::withdrawal() const {

    auto index = findAccount();
    // Account Not Found
    if (index == -1)
        return;

    // Found
    float amount = 0.0f;
    cout << "Enter the amount of the Withdrawal: ";
    cin >> amount;

    if (list[index]->Withdrawal(amount)) {
        cout << "The balance of the " << list[index]->getFullName()
            << " was decreased by: -" << amount << "$" << endl
            << "Current balance is: " << list[index]->getBalance() << "$" << endl;
    }
}

void Bank::closeAccount() {

    auto index = findAccount();
    // Account Not Found
    if (index == -1)
        return;

    // Found
    list.erase(list.begin() + index);
    cout << "The Account was Successfully Closed!" << endl;
}

void Bank::showAllAcounts() {
    if (list.size() == 0) 
        cout << "The list of accounts is empty." << endl;
    else {
        for (auto account : list)
            cout << *account << endl;
    }
}

void Bank::readAccounts() {
    ifstream ifs(FILE_NAME);
    Account* temp;

    if (ifs.is_open())
        while (!ifs.eof()) 
        {
            // Create new pointer to the Account object in the Heap
            temp = new Account();

            // Read Account
            ifs >> *temp;

            if (!ifs.eof()) {
                // Add the Account to the list
                list.push_back(temp);
            }
        }

    ifs.close();
}

void Bank::saveAcounts() 
{
    ofstream ofs(FILE_NAME);

    for (auto account : list)
        ofs << *account << endl;
 
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
                bank.openAccount();
                break;

            case 2: // Balance Enquiry 
                bank.balanceEnquiry();
                break;

            case 3: // Deposit
                bank.deposit();
                break;

            case 4: // Withdrawal
                bank.withdrawal();
                break;

            case 5: // Close an Account
                bank.closeAccount();
                break;

            case 6: // Show All 
                bank.showAllAcounts();
                break;

            case 7: // Quit
                cout << "Have a nice day!!!" << endl;
                break;

            default:
                cout << "Unknown option: " << choice << endl;
                exit(0);
                break;
        }
    }
}