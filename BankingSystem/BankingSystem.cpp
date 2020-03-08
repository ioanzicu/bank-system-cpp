// BankingSystem.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;


/* Account Class */

class Account {
private:
    static int count;
    size_t number;
    string firstName;
    string lastName;
    float balance;
public:
    Account(string firstName="Unknown", string lastName="Unknown", float balance=0.0f)
    {
        count++;
        number = count;
        this->firstName = firstName;
        this->lastName = lastName;
        this->balance = balance;
    }
    
    // Accessors and Mutators
    void setFirstName(string n) { firstName = n; }
    string getFirstName() const { return firstName; }

    void setLastName(string s) { lastName = s; }
    string getLastName() const { return lastName; }
    
    size_t getNumber() const { return number; }

    void setBalance(float b) { balance = b; }
    float getBalance() const { return balance; }

    string getFullName() { return firstName + " " + lastName; }

    // Serialize - Override << operator to write the object's state into a file
    friend ofstream& operator <<(ofstream& ofs, Account& a);
    // Deserialize - Override >> operator to read the opbject's stat from the file
    friend ifstream& operator >>(ifstream& ifs, Account& a);
    // Override cout to print the object's state
    friend ostream& operator <<(ostream& out, const Account& a);
};

int Account::count = 0;

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
    cout << "Balance: " << a.balance << endl;
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
        for (auto account : list) {
            delete account;
        }
        list.clear();
    }

    void openAccount();
    void balanceEnquiry(size_t n) const;
    void showAllAcounts();
    void readAccounts();
    void saveAcounts();
    void displayMenu() const;
};

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

void Bank::balanceEnquiry(size_t n) const {

    auto itr = find_if(list.begin(), list.end(), [&n](Account *const& obj) {return  obj->getNumber() == n; });

    if (itr != list.end()) {
        // Element found - an iterator to the first matching element.
        auto index = distance(list.begin(), itr);

        cout << endl << "The Balance of the User:"
            << list[index]->getFullName() << " is: "
            << list[index]->getBalance() << "$" << endl;
    } else
        cout << "There is no Acount with such number!" << endl;
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
    {
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

        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice)
        {
            case 1: // Open an Account
                bank.openAccount();
                break;

            case 2: // Balance Enquiry 
                {
                    int accountNumber = 0;
                    cout << "Enter the Number of the Account: ";
                    cin >> accountNumber;
                    bank.balanceEnquiry(accountNumber);
                }
                break;

            case 3: // Deposit
                break;

            case 4: // Withdrawal
                break;

            case 5: // Close an Account
                break;

            case 6: // Show All 
                bank.showAllAcounts();
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