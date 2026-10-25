#include <stdio.h>

// I'll continue with this...

int decimalToBaseX(int n, int base, int *counter) {
    (*counter)++;
    if (n < base) {
        return n;
    }
    
    return decimalToBaseX(n / base, base, counter) * 10 + (n % base);
}

int combination(int n, int r, int *counter) {
    (*counter)++;
    if (r == 0 || r == n) {
        return 1;
    }
    return combination(n - 1, r - 1, counter) + combination(n - 1, r, counter);
}

int GCD(int a, int b, int *counter) {
    (*counter)++;
    if(a%b==0){
        return b;
    }
    return GCD(b, a%b, counter);
}

int main() {
    int number = 13;
    int base = 7;
    int counter = 0;
    int result = decimalToBaseX(number, base, &counter);
    printf("Decimal %d number's equivalent in base %d is: %d\n", number, base, result);
    printf("Counter: %d\n", counter);

    counter = 0;
    int n = 5;
    int r = 2;
    int binomResult = combination(n, r, &counter);
    printf("C(%d, %d) = %d\n", n, r, binomResult);
    printf("Counter: %d\n", counter);

    int a = 100;
    int b = 44;
    counter = 0;
    int gcd = GCD(a, b, &counter);
    printf("GCD(%d, %d) = %d\n", a, b, gcd);
    printf("Counter: %d\n", counter);
    return 0;
}