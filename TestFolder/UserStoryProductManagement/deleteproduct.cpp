#include <iostream>
#include "../../src/product.h"
#include "../../src/productDatabase.h"

using namespace std;

void removeCorrectly(const string& id);
void invalidProductName(const string& id);


int main(){
    removeCorrectly("1234");
    invalidProductName("14124");
}

void removeCorrectly(const string& id){
    productDatabase manage("../../data/products.json");
    manage.delProduct(id);
}

void invalidProductName(const string& id){
    productDatabase manage("../../data/products.json");
    manage.delProduct(id);
}
