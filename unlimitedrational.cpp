#include "ulimitedrational.h"
using namespace std;

UnlimitedInt* ZERO = new UnlimitedInt(0);

static bool isZero(UnlimitedInt* value) {
    int* arr = value->get_array();
    for (int i = 0; i < value->get_size(); i++)
        if (arr[i] != 0)
            return false;
    return true;
}

UnlimitedInt* gcdHelper(UnlimitedInt* a, UnlimitedInt* b) {
    if (isZero(b))
        return a;
    UnlimitedInt* remainder = UnlimitedInt::mod(a, b);
    return gcdHelper(b, remainder);
}

UnlimitedInt* findGCD(UnlimitedInt* a, UnlimitedInt* b) {
    if (a->get_sign() == -1 || b->get_sign() == -1) {
        UnlimitedInt* posA = new UnlimitedInt(a->get_array(), 1, 1, a->get_size());
        UnlimitedInt* posB = new UnlimitedInt(b->get_array(), 1, 1, b->get_size());
        return gcdHelper(posA, posB);
    }
    return gcdHelper(a, b);
}

void simplifyFraction(UnlimitedInt*& numerator, UnlimitedInt*& denominator) {
    int sign = (numerator->get_sign()) * (denominator->get_sign());
    UnlimitedInt* gcd = findGCD(numerator, denominator);
    UnlimitedInt* tempNum = new UnlimitedInt(numerator->get_array(), 1, 1, numerator->get_size());
    UnlimitedInt* tempDen = new UnlimitedInt(denominator->get_array(), 1, 1, denominator->get_size());
    numerator = tempNum;
    denominator = tempDen;
    numerator = UnlimitedInt::div(numerator, gcd);
    denominator = UnlimitedInt::div(denominator, gcd);
    tempNum = new UnlimitedInt(numerator->get_array(), 1, sign, numerator->get_size());
    tempDen = new UnlimitedInt(denominator->get_array(), 1, 1, denominator->get_size());
    numerator = tempNum;
    denominator = tempDen;
}

UnlimitedRational::UnlimitedRational() {
    numerator = new UnlimitedInt();
    denominator = new UnlimitedInt();
}

UnlimitedRational::UnlimitedRational(UnlimitedInt* num, UnlimitedInt* den) {
    numerator = new UnlimitedInt(num->get_array(), num->get_capacity(), num->get_sign(), num->get_size());
    denominator = new UnlimitedInt(den->get_array(), den->get_capacity(), den->get_sign(), den->get_size());
}

UnlimitedRational::~UnlimitedRational() {
    delete numerator;
    delete denominator;
}

UnlimitedInt* UnlimitedRational::getNumerator() {
    return numerator;
}

UnlimitedInt* UnlimitedRational::getDenominator() {
    return denominator;
}

string UnlimitedRational::getNumeratorStr() {
    return numerator->to_string();
}

string UnlimitedRational::getDenominatorStr() {
    return denominator->to_string();
}

string UnlimitedRational::getFractionStr() {
    return numerator->to_string() + "/" + denominator->to_string();
}

UnlimitedRational* UnlimitedRational::add(UnlimitedRational* r1, UnlimitedRational* r2) {
    UnlimitedInt* part1 = UnlimitedInt::mul(r1->getNumerator(), r2->getDenominator());
    UnlimitedInt* part2 = UnlimitedInt::mul(r2->getNumerator(), r1->getDenominator());
    UnlimitedInt* num = UnlimitedInt::add(part1, part2);
    UnlimitedInt* den = UnlimitedInt::mul(r1->getDenominator(), r2->getDenominator());
    if (isZero(den))
        return new UnlimitedRational(ZERO, ZERO);
    UnlimitedRational* result = new UnlimitedRational(num, den);

    delete part1;
    delete part2;
    delete num;
    delete den;

    return result;
}

UnlimitedRational* UnlimitedRational::subtract(UnlimitedRational* r1, UnlimitedRational* r2) {
    UnlimitedInt* part1 = UnlimitedInt::mul(r1->getNumerator(), r2->getDenominator());
    UnlimitedInt* part2 = UnlimitedInt::mul(r2->getNumerator(), r1->getDenominator());
    UnlimitedInt* num = UnlimitedInt::sub(part1, part2);
    UnlimitedInt* den = UnlimitedInt::mul(r1->getDenominator(), r2->getDenominator());
    if (isZero(den))
        return new UnlimitedRational(ZERO, ZERO);
    UnlimitedRational* result = new UnlimitedRational(num, den);

    delete part1;
    delete part2;
    delete num;
    delete den;

    return result;
}

UnlimitedRational* UnlimitedRational::multiply(UnlimitedRational* r1, UnlimitedRational* r2) {
    UnlimitedInt* num = UnlimitedInt::mul(r1->getNumerator(), r2->getNumerator());
    UnlimitedInt* den = UnlimitedInt::mul(r1->getDenominator(), r2->getDenominator());
    if (isZero(den))
        return new UnlimitedRational(ZERO, ZERO);
    UnlimitedRational* result = new UnlimitedRational(num, den);

    delete num;
    delete den;

    return result;
}

UnlimitedRational* UnlimitedRational::divide(UnlimitedRational* r1, UnlimitedRational* r2) {
    UnlimitedInt* num = UnlimitedInt::mul(r1->getNumerator(), r2->getDenominator());
    UnlimitedInt* den = UnlimitedInt::mul(r1->getDenominator(), r2->getNumerator());
    if (isZero(den))
        return new UnlimitedRational(ZERO, ZERO);
    UnlimitedRational* result = new UnlimitedRational(num, den);

    delete num;
    delete den;

    return result;
}
