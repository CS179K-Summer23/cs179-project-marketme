#include <iostream>
#include "../../src/product.h"

using namespace std;

void correctAccept();
void duplicateProduct();

int main(){
    correctAccept();
    duplicateProduct();
}

void correctAccept(){
    //adjust new product accordingly for right now
    // name, price, description, amount
    Product newProduct("ice cream", 4, "its a cream that is ice", 1000);

    addProduct(newProduct, "../../data/products.json");
}

void duplicateProduct(){
    Product dupProduct("hotdog", 11, "its a dog in a hot", 400);

    addProduct(dupProduct, "../../data/products.json");
}