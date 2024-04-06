//---------------------------------------------------------------------------
// 
// Programmer: Sierra Nelson
// Project Name: Shopping Cart Project Part 3
// Date: 4/7/4
// Program Description:
//
// The Shopping Cart program allows customers to purchase items using
// their store credit. Featuring user authentication, item selection,
// account updates, and receipt generation. Customers can also browse, 
// purchase items, and manage their accounts.
//
//---------------------------------------------------------------------------


#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

// Constants
#define MAX_LINES 1000
#define PRODUCTS_FILE "products.csv"
#define MAX_CUSTOMERS 4
#define MAX_ATTEMPTS 3

using namespace std;

// Customer Account Data
string customers[MAX_CUSTOMERS];
string usernames[MAX_CUSTOMERS];
string passwords[MAX_CUSTOMERS];
string accountNumbers[MAX_CUSTOMERS];
string memberLevels[MAX_CUSTOMERS];
double storeCredits[MAX_CUSTOMERS];
string addresses[MAX_CUSTOMERS];

// Product Data
string productSKUs[MAX_LINES];
string productNames[MAX_LINES];
int productItemsInUnit[MAX_LINES];
double productPrices[MAX_LINES];
int productQuantities[MAX_LINES];
int numProducts = 0; // Number of products loaded

// Function to load Customer Account Data from accounts.dat
void loadCustomerData() {
    ifstream accountsFile("accounts.dat");
    if (!accountsFile.is_open()) {
        cout << "Error opening accounts.dat file." << endl;
        exit(1);
    }

    for (int i = 0; i < MAX_CUSTOMERS; ++i) {
        getline(accountsFile, customers[i], ';');
        getline(accountsFile, usernames[i], ';');
        getline(accountsFile, passwords[i], ';');
        getline(accountsFile, accountNumbers[i], ';');
        getline(accountsFile, memberLevels[i], ';');
        accountsFile >> storeCredits[i];
        accountsFile.ignore();
        getline(accountsFile, addresses[i]);
        accountsFile.ignore();
    }

    accountsFile.close();
}

// Function to load Product Data from products.csv
void loadProductData() {
    ifstream productsFile(PRODUCTS_FILE);
    if (!productsFile.is_open()) {
        cout << "Error opening " << PRODUCTS_FILE << " file." << endl;
        exit(1);
    }

    for (int i = 0; i < MAX_LINES; ++i) {
        if (getline(productsFile, productSKUs[i], ',') &&
            getline(productsFile, productNames[i], ',') &&
            (productsFile >> productItemsInUnit[i]) &&
            (productsFile.ignore(), productsFile >> productPrices[i]) &&
            (productsFile.ignore(), productsFile >> productQuantities[i])) {
            ++numProducts;
        } else {
            break;
        }
    }

    productsFile.close();
}

// Authenticate User
int authenticate(const string& enteredUsername, const string& enteredPassword) {
    for (int i = 0; i < MAX_CUSTOMERS; ++i) {
        if (enteredUsername == usernames[i] && enteredPassword == passwords[i]) {
            return i; // Return the index of the authenticated user
        }
    }
    return -1; // Return -1 if authentication fails
}

// Function to display customer account information
void displayCustomerInfo(int index) {
    cout << "\nCustomer Details:" << endl;
    cout << "Customer: " << customers[index] << endl;
    cout << "Username: " << usernames[index] << endl;
    cout << "Password: " << passwords[index] << endl;
    cout << "Account #: " << accountNumbers[index] << endl;
    cout << "Member Level: " << memberLevels[index] << endl;
    cout << "Store Credit: $" << fixed << setprecision(2) << storeCredits[index] << endl;
    cout << "Address: " << addresses[index] << endl;
}

// Function to display product information
void displayProductInfo() {
    ifstream productsFile(PRODUCTS_FILE);
    if (!productsFile.is_open()) {
        cout << "Error opening " << PRODUCTS_FILE << " file." << endl;
        return;
    }

    cout << "Product Information:" << endl;
    cout << "SKU     Name     Items in unit     Price per unit     Quantity on Hand" << endl;
    cout << "------------------------------------------------------------------------------" << endl;

    string array[MAX_LINES];
    int lines = 0;

    while (!productsFile.eof()) {
        getline(productsFile, array[lines]);
        lines++;

        if (lines == MAX_LINES) {
            cout << "Max storage reached" << endl;
            break;
        }
    }

    productsFile.close();

    for (int i = 0; i < lines; i++) {
        cout << array[i] << endl;
    }
}

// Function to allow a customer to purchase an item
void purchaseItem(int loggedInUserIndex) {
    string sku;
    int quantity;
    bool validPurchase = true;

    cout << "Enter the SKU of the item you would like to purchase: ";
    string inputSKU;
    cin >> inputSKU;

    bool validSKU = false;
    double price = 0;
    int stock = 0;

    if (inputSKU == "HF-342" || inputSKU == "RG-324" || inputSKU == "LK-322" ||
        inputSKU == "PL-643" || inputSKU == "PM-263" || inputSKU == "KF-231") {
        validSKU = true;

        int inputQuantity;
        cout << "Enter the quantity of the item you would like to purchase: ";
        cin >> inputQuantity;

        bool validQuantity = (inputQuantity > 0);

        if (!validQuantity) {
            cout << "That is an invalid quantity. Please enter a positive number." << endl;
            return;
        }

        // Calculate price based on SKU
        if (inputSKU == "HF-342") {
            stock = 200;
            price = inputQuantity * 20.00;
        } else if (inputSKU == "RG-324") {
            stock = 174;
            price = inputQuantity * 20.00;
        } else if (inputSKU == "LK-322") {
            stock = 76;
            price = inputQuantity * 5.75;
        } else if (inputSKU == "PL-643") {
            stock = 35;
            price = inputQuantity * 35.67;
        } else if (inputSKU == "PM-263") {
            stock = 10;
            price = inputQuantity * 430.39;
        } else if (inputSKU == "KF-231") {
            stock = 100;
            price = inputQuantity * 15.23;
        }

        if (validSKU && validQuantity && inputQuantity <= stock) {
            double tax = 0.06 * price;
            double totalPrice = price + tax;

            if (storeCredits[loggedInUserIndex] >= totalPrice) {
                storeCredits[loggedInUserIndex] -= totalPrice; // Deduct price from store credit
                cout << "Purchase Successful!" << endl;
                cout << "Item: ";
                if (inputSKU == "HF-342") {
                    cout << "½ in Bolt";
                } else if (inputSKU == "RG-324") {
                    cout << "½ in Screw";
                } else if (inputSKU == "LK-322") {
                    cout << "¼ in Nail";
                } else if (inputSKU == "PL-643") {
                    cout << "Saw";
                } else if (inputSKU == "PM-263") {
                    cout << "Lawn mower";
                } else if (inputSKU == "KF-231") {
                    cout << "Hammer";
                }
                cout << endl;
                cout << "Quantity: " << inputQuantity << endl;
                cout << "Total Price: $" << fixed << setprecision(2) << totalPrice << endl;
                cout << "Store Credit Remaining: $" << fixed << setprecision(2) << storeCredits[loggedInUserIndex] << endl;
            } else {
                cout << "Insufficient store credit to make this purchase." << endl;
            }
        } else {
            cout << "Invalid SKU or quantity exceeds available stock." << endl;
        }
    } else {
        cout << "That is an invalid SKU. Please enter a valid SKU from the Product Information list." << endl;
    }
}

// Function to get an estimate
void getEstimate() {
    cout << "Enter the SKU of the item you would like to estimate: ";
    string inputSKU;
    cin >> inputSKU;

    bool validSKU = false;
    if (inputSKU == "HF-342" || inputSKU == "RG-324" || inputSKU == "LK-322" ||
        inputSKU == "PL-643" || inputSKU == "PM-263" || inputSKU == "KF-231") {
        validSKU = true;
    }

    if (!validSKU) {
        cout << "That is an invalid SKU. Please enter a valid SKU from the Product Information list." << endl;
        return;
    }

    int inputQuantity;
    cout << "Enter the quantity of the item: ";
    cin >> inputQuantity;

    bool validQuantity = (inputQuantity > 0);

    if (!validQuantity) {
        cout << "That is an invalid quantity. Please enter a positive number." << endl;
        return;
    }

    double price;
    if (inputSKU == "HF-342") {
        price = inputQuantity * 20.00;
    } else if (inputSKU == "RG-324") {
        price = inputQuantity * 20.00;
    } else if (inputSKU == "LK-322") {
        price = inputQuantity * 5.75;
    } else if (inputSKU == "PL-643") {
        price = inputQuantity * 35.67;
    } else if (inputSKU == "PM-263") {
        price = inputQuantity * 430.39;
    } else if (inputSKU == "KF-231") {
        price = inputQuantity * 15.23;
    }

    double tax = 0.06 * price;
    double totalPrice = price + tax;

    cout << "Estimated Total Price: $" << fixed << setprecision(2) << totalPrice << endl;
}

// Function to update customer account information
void updateAccount(int loggedInUserIndex) {
    cout << "Update Account Information for " << usernames[loggedInUserIndex] << endl;
    cout << "1 - Update Customer Name" << endl;
    cout << "2 - Update Password" << endl;
    cout << "3 - Update Member Level" << endl;
    cout << "4 - Update Address" << endl;
    cout << "Enter your choice: ";
    int update;
    cin >> update;

    switch (update) {
        case 1: {
            cout << "Enter new customer name: ";
            cin.ignore();
            getline(cin, customers[loggedInUserIndex]);
            cout << "Customer name updated successfully." << endl;
            break;
        }
        case 2: {
            cout << "Enter new password: ";
            cin >> passwords[loggedInUserIndex];
            cout << "Password updated successfully." << endl;
            break;
        }
        case 3: {
            cout << "Update Member Level" << endl;
            cout << "Enter new member level (Diamond, Gold, Blue): ";
            cin >> memberLevels[loggedInUserIndex];

            if (memberLevels[loggedInUserIndex] == "Diamond" || memberLevels[loggedInUserIndex] == "Gold" || memberLevels[loggedInUserIndex] == "Blue") {
                cout << "Member level updated successfully." << endl;
            } else {
                cout << "Invalid member level. Please enter Diamond, Gold, or Blue." << endl;
            }
            break;
        }
        case 4: {
            cout << "Enter new address: ";
            cin.ignore();
            getline(cin, addresses[loggedInUserIndex]);
            cout << "Address updated successfully." << endl;
            break;
        }
        default: {
            cout << "Invalid choice. Please enter a number between 1 and 4." << endl;
            break;
        }
    }

    cout << "\nUpdated Account Information:\n";
    cout << "Customer: " << customers[loggedInUserIndex] << endl;
    cout << "Username: " << usernames[loggedInUserIndex] << endl;
    cout << "Password: " << passwords[loggedInUserIndex] << endl;
    cout << "Member Level: " << memberLevels[loggedInUserIndex] << endl;
    cout << "Store Credit: $" << fixed << setprecision(2) << storeCredits[loggedInUserIndex] << endl;
    cout << "Address: " << addresses[loggedInUserIndex] << endl;
}

int main() {
    cout << "Welcome to the Shopping Cart!" << endl;

    // Load customer account data from accounts.dat
    loadCustomerData();

    // Load product information from products.csv
    loadProductData();

    // Authenticate the user
    string enteredUsername;
    string enteredPassword;
    int attempts = 0;
    bool authenticated = false;
    int loggedInUserIndex = -1; // Initialize to invalid index

    do {
        cout << "Enter your username: ";
        cin >> enteredUsername;
        cout << "Enter your password: ";
        cin >> enteredPassword;

        loggedInUserIndex = authenticate(enteredUsername, enteredPassword);

        if (loggedInUserIndex == -1) {
            cout << "Incorrect username or password. Please try again." << endl;
            attempts++;
        }
    } while (loggedInUserIndex == -1 && attempts < MAX_ATTEMPTS);

    if (loggedInUserIndex == -1) {
        cout << "Maximum login attempts reached. Exiting the program." << endl;
        return 1;
    }

    cout << "Authentication successful. Welcome, " << enteredUsername << "!" << endl;

    int transactionsCompleted = 0;

    int menu;
    do {
        cout << "\n1 - Purchase an item" << endl;
        cout << "2 - View all items" << endl;
        cout << "3 - Get an estimate" << endl;
        cout << "4 - Update Account" << endl;
        cout << "5 - Log Out" << endl;
        cout << "Enter your choice from the Menu above: ";
        cin >> menu;

        switch (menu) {
            case 1:
                displayProductInfo();
                purchaseItem(loggedInUserIndex); // Pass the loggedInUserIndex
                transactionsCompleted++;
                break;
            case 2:
                displayProductInfo();
                break;
            case 3:
                displayProductInfo();
                getEstimate();
                break;
            case 4:
                updateAccount(loggedInUserIndex); // Pass the loggedInUserIndex
                break;
            case 5:
                cout << "Logging out... Goodbye!" << endl;
                break;
            default:
                cout << "Invalid choice. Please enter a number between 1 and 5." << endl;
                break;
        }
        
        if (transactionsCompleted >= 3) {
            cout << "Maximum transactions completed. Exiting the program." << endl;
            break;
        }
    } while (menu != 5);

    return 0;
}