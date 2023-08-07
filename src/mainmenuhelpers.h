#include <iostream>
#include "product.h"
#include "productDatabase.h"

using namespace std;

void addNewProduct();
void deleteProduct();
void updateProduct();
int acceptNumber(const string& prompt);
void addMenu();
void addBarcode();

void addNewProduct(){
    productDatabase manage("data/products.json");
    string id, name, description, category, sku, barcode, expiration_date;
    int quantity;
    double price;

    cout << "ID: ";
    cin >> id;

    if(manage.exists(id)){
        cout << "ID already exists in the inventory" << endl;
        return;
    }

    cout << "Name: ";
    cin.ignore(); 
    getline(cin, name);
    cout << "Description: ";
    getline(cin, description);
    cout << "Category: ";
    getline(cin, category);
    cout << "SKU: ";
    cin >> sku;
    cout << "Barcode: ";
    cin >> barcode;
    cout << "Expiration Date: ";
    cin >> expiration_date;
    quantity = acceptNumber("Quantity");
    price = acceptNumber("Price");

    Product newProduct(id, name, description, price, quantity, category, sku, barcode, expiration_date);
    manage.addProduct(newProduct);
}

void deleteProduct(){
    //use if we are allowing different ways to delete a rpoduct
    // int ans;

    // cout << "1. ID\n2. Name\n3. Barcode\n";
    // cin >> ans;

    // switch(ans){
    //     case 1:
    //         cout << ": ";

    //         break;
    // }
    string productID;
    cout << "ID: ";
    cin >> productID;

    productDatabase manage("data/products.json");
    manage.delProduct(productID);

}

void updateProduct(){
    string id;
    int option;
    string whatoption;
    string change = "";
    productDatabase manage("data/products.json");

    cout << "ID: ";
    cin >> id;

    if(!manage.exists(id)){
        cout << "ID does not exist in the inventory" << endl;
        return;
    }

    cout << "Choose an option " << endl;
    cout << "1. Name" << endl;
    cout << "2. Description" << endl;
    cout << "3. Category" << endl;
    cout << "4. SKU" << endl;
    cout << "5. Barcode" << endl;
    cout << "6. Expiration Date" << endl;
    cout << "7. Quantity" << endl;
    cout << "8. Price" << endl;
    cout << "9. Cancel" << endl;
    cin >> option;

    switch (option) {
        case 1:
            cout << "New name: ";
            cin.ignore();
            getline(cin, change);
            whatoption = "name";
            break;
        case 2:
            cout << "New description: ";
            cin.ignore();
            getline(cin, change);
            whatoption = "description";
            break;
        case 3:
            cout << "New category: ";
            cin.ignore();
            getline(cin, change);
            whatoption = "category";
            break;
        case 4:
            cout << "New SKU: ";
            cin >> change;
            whatoption = "SKU";
            break;
        case 5:
            cout << "New barcode: ";
            cin >> change;
            whatoption = "barcode";
            break;
        case 6:
            cout << "New expiration date: ";
            cin >> change;
            whatoption = "expiration_date";
            break;
        case 7:
            change = to_string(acceptNumber("New quantity"));
            whatoption = "quantity";
            break;
        case 8:
            change = to_string(acceptNumber("New price"));
            whatoption = "price";
            break;
        case 9:
            cout << "Canceled" << endl;
            break;
        default:
            cout << "Invalid Option" << endl;
            break;
    }

    if(change != ""){
        manage.updateProduct(id, whatoption, change);
    }


}

int acceptNumber(const string& prompt) {
    string input;
    int number;

    while (true) {
        cout << prompt << ": ";
        cin >> input;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        try {
            number = stoi(input);
            break; // If stoi() succeeds, exit the loop
        } catch (const invalid_argument& e) {
            cout << "Invalid input. Please enter a valid number." << endl;
        } catch (const out_of_range& e) {
            cout << "Invalid input. Number out of range." << endl;
        }
    }

    return number;
}

void addMenu(){
    int temp = 0;
    while(temp != 1 && temp != 2){
        temp = acceptNumber("1. Product ID\n2. Barcode\n");
        if(!(temp == 1 || temp == 2)){
            cout << "Invalid option. Try again." << endl;
        }
    }
    if(temp == 1){
        addNewProduct();
    }
    else{
        addBarcode();
    }
}

void addBarcode(){
    string barcode;
    cout << "Please scan a barcode: ";
    getline(cin, barcode);
    cout << "Scanned barcode: " << barcode << endl;
}