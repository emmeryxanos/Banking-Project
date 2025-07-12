#include<bits/stdc++.h>
#include<cstring>
#include<fstream>
using namespace std;

struct Account {
    int accountNumber;
    char name[100];
};

//creating filenames
string accfilename (int accNumber) {
    return "account_" + to_string(accNumber) + ".txt";
}

string balfilename (int accNumber) {
    return "balance_" + to_string(accNumber) + ".bin";
}

//checking if account exists
bool accountExists(int accNumber) {
    ifstream accFile(accfilename(accNumber));
    return accFile.good();
}
vector<int> accountNumbers;

//file to store account numbers
void loadAcc() {
    ifstream indexFile("index.txt");
    int accNumber;
    while (indexFile >> accNumber) {
        accountNumbers.push_back(accNumber);
    }
    indexFile.close();
}
void addAccount(int accNumber) {
    accountNumbers.push_back(accNumber);
    ofstream indexFile("index.txt", ios::app);
    indexFile << accNumber << endl;
    indexFile.close();
}
void removeAccount(int accNumber) {
    accountNumbers.erase(remove(accountNumbers.begin(), accountNumbers.end(), accNumber), accountNumbers.end());


    ofstream indexFile("index.txt");
    for (int num : accountNumbers) {
        indexFile << num << endl;
    }
    indexFile.close();
}


//creating account
void createAccount() {
    Account acc;
    float balance;
    cout<<"Enter your account number: ";
    cin>>acc.accountNumber;
    if (accountExists(acc.accountNumber)) {
        cout << "Account already exists!\n";
        return;
    }
    cout<<"Enter your name: ";
    cin.ignore();
    cin.getline(acc.name,100);
    cout<<"Enter your initial balance: ";
    cin>>balance;
    ofstream accFile(accfilename((acc.accountNumber)));
    ofstream balFile(balfilename(acc.accountNumber), ios::binary);
    accFile<<acc.accountNumber<<endl;
    accFile<<acc.name<<endl;
    balFile.write(reinterpret_cast<const char*>(&balance),sizeof(balance));
    addAccount(acc.accountNumber); 
    accFile.close();
    balFile.close();
    cout << "File " << accfilename(acc.accountNumber) << " created successfully!\n";
}

//viewing account
void viewAccount() {
    int accNumber;
    cout << "Enter your account number: ";
    cin >> accNumber;
    ifstream accFile(accfilename(accNumber));
    ifstream balFile(balfilename(accNumber), ios::binary); 
    if (!accFile || !balFile) {
        cout << "Account does not exist!\n";
        return;
    }
    Account acc;
    float balance; 
    accFile >> acc.accountNumber;
    accFile.ignore(); 
    accFile.getline(acc.name, 100); 
    balFile.read(reinterpret_cast<char*>(&balance), sizeof(balance)); 
    cout << "Account Number: " << acc.accountNumber << endl;
    cout << "Name: " << acc.name << endl;
    cout << "Current balance: " << fixed << setprecision(2) << balance << endl; 
    accFile.close();
    balFile.close();
}

// deposit
void deposit() {
    int accNumber;
    float amount;
    cout << "Enter your account number: ";
    cin >> accNumber;
    ifstream balFile(balfilename(accNumber), ios::binary);
    if (!balFile) {
        cout << "Account does not exist!\n";
        return;
    }
    balFile.read(reinterpret_cast<char*>(&amount), sizeof(amount)); 
    cout << "Current balance: " << amount << endl;
    cout << "Enter amount to deposit: ";
    float depAmount;
    cin >> depAmount;
    amount += depAmount;
    ofstream balFileOut(balfilename(accNumber), ios::binary);
    balFileOut.write(reinterpret_cast<const char*>(&amount), sizeof(amount));
    balFileOut.close();
    cout << "Deposit successful! New balance: " << amount << endl;
}

// withdraw
void withdraw() {
    int accNumber;
    float amount;
    cout << "Enter your account number: ";
    cin >> accNumber;
    ifstream balFile(balfilename(accNumber), ios::binary);
    if (!balFile) {
        cout << "Account does not exist!\n";
        return;
    }
    balFile.read(reinterpret_cast<char*>(&amount), sizeof(amount));
    cout << "Current balance: " << amount << endl;
    cout << "Enter amount to withdraw: ";
    float withAmount;
    cin >> withAmount;
    if (withAmount > amount) {
        cout << "Insufficient balance!\n";
        return;
    }
    amount -= withAmount;
    ofstream balFileOut(balfilename(accNumber), ios::binary);
    balFileOut.write(reinterpret_cast<const char*>(&amount), sizeof(amount));
    balFileOut.close();
    cout << "Withdrawal successful! New balance: " << amount << endl;
}

// list all accounts
void listAccounts() {
    cout << left << setw(15) << "Account Number" << setw(25) << "Name" << setw(15) << "Balance" << endl;

    if (accountNumbers.empty()) {
        cout << "No accounts found.\n";
        return;
    }
    for (int accNumber : accountNumbers) {
        ifstream accFile(accfilename(accNumber));
        ifstream balFile(balfilename(accNumber), ios::binary);

        if (accFile && balFile) {
            Account acc;
            float balance;

            accFile >> acc.accountNumber;
            accFile.ignore();
            accFile.getline(acc.name, 100);
            balFile.read(reinterpret_cast<char*>(&balance), sizeof(balance));

            cout << left << setw(15) << acc.accountNumber
                 << setw(25) << acc.name
                 << setw(15) << fixed << setprecision(2) << balance << endl;

            accFile.close();
            balFile.close();
        }
        else {
            cout << "Error reading account files for: " << accNumber << endl;
        }
    }
}

// delete account
void deleteAccount() {
    int accNumber;
    cout << "Enter your account number to delete: ";
    cin >> accNumber;
    if (!accountExists(accNumber)) {
        cout << "Account does not exist!\n";
        return;
    }
    if (remove(accfilename(accNumber).c_str()) != 0 ) {
        cout << "Error deleting file!\n";
    } else {
        removeAccount(accNumber); 
        cout << "Account deleted successfully!\n";
    }
    if (remove(balfilename(accNumber).c_str()) != 0) {
        cout << "Error deleting file!\n";
    } else {
        cout <<"Account deleted successfully!\n";
    }
}
//transfering money
void transferMoney() {
    int fromAcc, toAcc;
    float amount;
    
    cout << "Enter your account number: ";
    cin >> fromAcc;
    if (!accountExists(fromAcc)) {
        cout << "Account does not exist!\n";
        return;
    }
    
    cout << "Enter recipient's account number: ";
    cin >> toAcc;
    if (!accountExists(toAcc)) {
        cout << "Recipient's account does not exist!\n";
        return;
    }
    
    ifstream balFileFrom(balfilename(fromAcc), ios::binary);
    float fromBalance;
    balFileFrom.read(reinterpret_cast<char*>(&fromBalance), sizeof(fromBalance));
    balFileFrom.close();
    
    cout << "Current balance in your account: " << fixed << setprecision(2) << fromBalance << endl;
    
    cout << "Enter amount to transfer: ";
    cin >> amount;
    
    if (amount > fromBalance) {
        cout << "Insufficient balance!\n";
        return;
    }
    

    fromBalance -= amount;
    ofstream balFileOutFrom(balfilename(fromAcc), ios::binary);
    balFileOutFrom.write(reinterpret_cast<const char*>(&fromBalance), sizeof(fromBalance));
    balFileOutFrom.close();
    
    ifstream balFileTo(balfilename(toAcc), ios::binary);
    float toBalance;
    balFileTo.read(reinterpret_cast<char*>(&toBalance), sizeof(toBalance));
    balFileTo.close();
    
    toBalance += amount;
    ofstream balFileOutTo(balfilename(toAcc), ios::binary);
    balFileOutTo.write(reinterpret_cast<const char*>(&toBalance), sizeof(toBalance));
    balFileOutTo.close();
    
    cout << "Transfer successful! New balance in your account: " << fixed << setprecision(2) << fromBalance << endl;
}
int main() {

    loadAcc(); 
    int choice;
    
    do {
        cout << "\n=== Banking System Menu ===\n";
        cout << "1. Create Account\n";
        cout << "2. View Account\n";
        cout << "3. Deposit\n";
        cout << "4. Withdraw\n";
        cout << "5. List All Accounts\n";
        cout << "6. Delete Account\n";
        cout<< "7. Transfer Money\n";
        cout << "8. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: createAccount(); break;
            case 2: viewAccount(); break;
            case 3: deposit(); break;
            case 4: withdraw(); break;
            case 5: listAccounts(); break;
            case 6: deleteAccount(); break;
            case 7: transferMoney(); break;
            case 8: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice!\n";
        }
    } while (choice != 8);

    return 0;
}