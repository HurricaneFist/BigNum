#include <iostream>
#include "bignum.h"

int main() {
    
    BigNum::Num x("43564325864365296747647564357647"), 
                y("83284328472353842346892343274623");
                
    std::cout << multiply(x, y);
}
