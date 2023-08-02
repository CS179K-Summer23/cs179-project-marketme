#include <iostream>
#include "../../src/product.h"
#include "../../src/productDatabase.h"

using namespace std;

void removeCorrectly(const string& name);
void invalidProductName(const string& name);


int main(){
    removeCorrectly("hotdog");
    invalidProductName("nothing");
}

void removeCorrectly(const string& name){
    productDatabase manage("../../data/products.json");
    manage.delProduct(name);
}

void invalidProductName(const string& name){
    productDatabase manage("../../data/products.json");
    manage.delProduct(name);
}
