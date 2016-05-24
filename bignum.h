#ifndef BIGNUM_H
#define BIGNUM_H

#include <sstream>

/**
 * This header file provides algorithms for addition, multiplication, and exponentiation
 * of unsigned arbitrary-precision integers (AKA big numbers). 
 *
 * https://github.com/HurricaneFist
 */

namespace BigNum {

    // convert a character to an integer
    int val(char c) {
        return c - '0';
    }

    // convert an integer to a character
    char unval(int n) {
        return n + '0';
    }

    /*
     * Arbitrary-precision unsigned integer. Usually used for big numbers.
     * Min value = 0
     * Max value = 9.999... x 10^4294967293
     * A Num of max value would theoretically consume over 4 GB of memory or disk space.
     */
    struct Num {
        std::string digits;
        
        Num() {
            digits = "";
        }
        
        Num(std::string digits_) {
            digits = digits_;
        }
        
        friend std::ostream &operator<<(std::ostream &os, const Num &n) {
            os << n.digits;
            return os;
        }
    };

    // Scientific notation with significant figures
    std::string scino(Num n, int sigfigs) {
        std::stringstream res;
        res << n.digits[0];
        
        if (sigfigs > 1) {
            res << '.';
            
            for (int i = 1; i < sigfigs; i++) {
                if (i > n.digits.length()-1)
                    res << '0';
                else
                    res << n.digits[i];
            }
        }
        
        res << 'E';
        res << n.digits.length()-1;
        return res.str();
    }

    std::string scino(Num n) {
        return scino(n, 5);
    }

    std::string scino(std::string n, int sigfigs) {
        return scino(Num(n), sigfigs);
    }

    std::string scino(std::string n) {
        return scino(Num(n), 5);
    }

    int compare(Num x, Num y) {
        if (x.digits.length() > y.digits.length())
            return 1;
        
        if (x.digits.length() < y.digits.length())
            return -1;
        
        for (int i = 0; i < x.digits.length(); i++) {
            if (val(x.digits[i]) > val(y.digits[i]))
                return 1;
            if (val(x.digits[i]) < val(y.digits[i]))
                return -1;
        }
        
        return 0;
    }

    Num add(Num m, Num n) {
        Num x, y;
        std::string sum = "";
        
        // x will always be greater or equal to y in terms of number of digits
        if (m.digits.length() >= n.digits.length()) {
            x = m;
            y = n;
        } else {
            x = n;
            y = m;
        }
        
        int xind = x.digits.length()-1, yind = y.digits.length()-1;
        
        int drem = 0, dsum;
        while (xind >= 0 || yind >= 0 || drem == 1) {
            dsum = drem;
            if (xind >= 0) dsum += val(x.digits[xind]);
            if (yind >= 0) dsum += val(y.digits[yind]);
            
            if (dsum >= 10) {
                dsum -= 10;
                drem = 1;
            } else drem = 0;
            
            sum = unval(dsum) + sum;

            xind--;
            yind--;
        }
        
        return Num(sum);
    }

    Num add(std::string m, std::string n) {
        return add(Num(m), Num(n));
    }


    Num multiply(Num m, Num n) {
        if (m.digits == "0" || n.digits == "0") return Num("0");
        
        Num x, y, product("0");
        
        // x will always be greater or equal to y in terms of number of digits
        if (m.digits.length() >= n.digits.length()) {
            x = m;
            y = n;
        } else {
            x = n;
            y = m;
        }
        
        int zeroes = 0;
        for (int yind = y.digits.length()-1; yind >= 0; yind--, zeroes++) {
            std::string dtotal = "";
            int drem = 0;
            
            for (int i = 0; i < zeroes; i++)
                dtotal = '0' + dtotal;
            
            for (int xind = x.digits.length()-1; xind >= 0; xind--) {
                int dprod = val(x.digits[xind]) * val(y.digits[yind]) + drem;
                
                if (dprod >= 10) {
                    drem = dprod / 10;
                    dprod -= drem * 10; 
                } else drem = 0;
                
                dtotal = unval(dprod) + dtotal;
            }
            
            if (drem > 0) dtotal = unval(drem) + dtotal;
            
            product = add(product, Num(dtotal));
        }
        
        return product;
    }

    Num multiply(std::string m, std::string n) {
        return multiply(Num(m), Num(n));
    }

    // Decrement a Num (down to zero)
    Num decrement(Num n) {
        std::string res = "";
        char digit;
        
        bool decr = true;
        for (int i = n.digits.length()-1; i >= 0; i--) {
            if (decr) {
                if (i == 0 && n.digits[i] == '1' && n.digits.length() > 1)
                    break;
                else if (n.digits[i] == '0')
                    digit = '9';
                else {
                    digit = unval(val(n.digits[i])-1);
                    decr = false;  
                }
            } else digit = n.digits[i];
            
            res = digit + res;
        }
        
        return Num(res);
    }

    Num decrement(std::string n) {
        return decrement(Num(n));
    }

    // Exponentiation. N to the power of P.
    Num power(Num n, Num p) {
        Num res("1");

        while (p.digits != "0") {
            res = multiply(n, res);
            p = decrement(p);
        }
        
        return res;
    }

    Num power(std::string n, std::string p) {
        return power(Num(n), Num(p));
    }

    // Compute the factorial of a Num.
    Num factorial(Num n) {
        if (n.digits != "0" && n.digits != "1")
            return multiply(n, factorial(decrement(n)));
        
        return Num("1");
    }

    Num factorial(std::string n) {
        return factorial(Num(n));
    }

}

#endif