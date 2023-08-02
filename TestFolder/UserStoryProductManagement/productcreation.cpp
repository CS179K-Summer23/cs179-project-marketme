#include <iostream>
#include "../../src/product.h"
#include "../../src/productDatabase.h"

using namespace std;

void correctAccept();
void duplicateProduct();

int main(){
    correctAccept();
    duplicateProduct();
}

void correctAccept(){
    //adjust new product aasccordingly for right now
    // ID, NAME, DESCRIPTION, PRICE, QUANTITY, CATEGORY, SKU, BARCODE, EXPIRATIONDATE
    Product newProduct("1234", "ice cream", "its a cream that is ice", 4.0, 1000, "food", "ABC123", "ABC123", "2023-12-31");
    productDatabase manage("../../data/products.json");

    manage.addProduct(newProduct);
}

void duplicateProduct(){
    Product dupProduct("2345", "hotdog", "its a dog in a hot", 11.0, 400, "food", "BCD234", "BCD234", "2023-12-31");
    productDatabase manage("../../data/products.json");

    manage.addProduct(dupProduct);
}