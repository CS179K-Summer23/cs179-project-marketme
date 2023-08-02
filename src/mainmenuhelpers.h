#include <iostream>
#include "product.h"
#include "productDatabase.h"

using namespace std;

void addNewProduct(){
    string id, name, description, category, sku, barcode, expiration_date;
    int quantity;
    double price;

    cout << "ID: ";
    cin >> id;
    cout << "Name: ";
    cin >> name;
    cout << "Description: ";
    cin >> description;
    cout << "Category: ";
    cin >> category;
    cout << "SKU: ";
    cin >> sku;
    cout << "Barcode: ";
    cin >> barcode;
    cout << "Expiration Date: ";
    cin >> expiration_date;
    cout << "Quantity: ";
    cin >> quantity;
    cout << "Price: ";
    cin >> price;

    Product newProduct(id, name, description, price, quantity, category, sku, barcode, expiration_date);
    productDatabase manage("data/products.json");
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

    productDatabase manage("data.products.json");
    manage.delProduct(productID);

}


void updateProduct(){
    string id;
    int option;
    string whatoption;
    string change = "";

    cout << "ID: ";
    cin >> id;

    cout << "Choose an option " << endl;
    cout << "1. Name" << endl;
    cout << "2. Description" << endl;
    cout << "3. Price" << endl;
    cout << "4. Quantity" << endl;
    cout << "5. SKU" << endl;
    cout << "6. Barcode" << endl;
    cout << "7. Expiration Date" << endl;
    cout << "8. Cancel" << endl;
    cin >> option;

    switch(option){
        case 1:
            cout << "New name: ";
            cin >> change; 
            whatoption = "name";
            break;
        case 2:
            cout << "New description: ";
            cin >> change;
            whatoption = "description";
            break;
        case 3:
            cout << "New price: ";
            cin >> change; 
            whatoption = "price";
            break;
        case 4:
            cout << "New quantity: ";
            cin >> change;
            whatoption = "quantity";
            break;
        case 5:
            cout << "New SKU: ";
            cin >> change; 
            whatoption = "sku";
            break;
        case 6:
            cout << "New barcode: ";
            cin >> change;
            whatoption = "barcode";
            break;
        case 7:
            cout << "New expiration date: ";
            cin >> change; 
            whatoption = "expiration_date";
            break;
        case 8:
            cout << "Canceled";
            break;
        default:
            cout << "Invalid Option" << endl;
            break;
    }

    if(change != ""){
        productDatabase manage("data/products.json");
        manage.updateProduct(id, whatoption, change);
    }


}