#include <iostream>
#include "../../src/product.h"

using namespace std;

void correctAccept();

int main(){
    correctAccept();
}

void correctAccept(){
    Product newProduct("hotdog", 11, "its a dog in a hot", 400);

    addProduct(newProduct, "../../data/products.json");
}