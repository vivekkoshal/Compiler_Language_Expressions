#include <vector>
#include <algorithm>
#include <stdexcept>
#include <cstdint>
#include "unlimitedint.h"
using namespace std;

#define DIGITS_PER_INT 9
#define BASE 1000000000

int vector_compare(vector<int>& a, vector<int>& b) {
    if (a.size() > b.size())
        return 1;
    if (a.size() < b.size())
        return -1;

    for (int i = a.size() - 1; i >= 0; i--) {
        if (a[i] > b[i])
            return 1;
        if (a[i] < b[i])
            return -1;
    }

    return 0;
}

void vector_subtract(vector<int>& a, vector<int>& b, vector<int>& result) {
    result.clear();

    for (int i = 0; i < a.size(); i++)
        result.push_back(a[i] - (i < b.size() ? b[i] : 0));

    for (int i = 0; i < result.size(); i++) {
        if (result[i] < 0) {
            result[i+1]--;
            result[i] += BASE;
        }
    }

    int n = result.size();
    for (int i = n-1; i > 0; i--)
        if (result[i] == 0)
            result.pop_back();
}

void vector_multiply_by_int(vector<int>& a, int b, vector<int>& product) {
    vector<uint64_t> temp;
    temp.resize(a.size());

    for (int i = 0; i < a.size(); i++)
        temp[i] = a[i] * b;

    for (int i = 0; i < a.size(); i++) {
        if (temp[i] >= BASE) {
            if (i + 1 >= temp.size())
                temp.push_back(0);
            temp[i+1] += temp[i] / BASE;
            temp[i] %= BASE;
        }
    }

    product.clear();
    for (int i = 0; i < temp.size(); i++)
        product.push_back((int) temp[i]);
}

int vector_divide(vector<int>& a, vector<int>& b, vector<int>& remainder) {
    remainder.clear();

    int cmp = vector_compare(a, b);
    if (cmp < 0) {
        remainder = a;
        return 0;
    }
    if (cmp == 0) {
        return 1;
    }

    int low = 1;
    int high = a[a.size()-1] / b[a.size()-1];
    if (high == 1) {
        vector_subtract(a, b, remainder);
        return 1;
    }

    high += 1;
    int mid;
    vector<int> temp;
    while (low < high - 1) {
        mid = (high + low) / 2;
        vector_multiply_by_int(b, mid, temp);

        cmp = vector_compare(temp, a);
        if (cmp == 0) {
            return mid;
        }

        if (cmp < 0)
            low = mid;
        else
            high = mid;
    }

    vector_multiply_by_int(b, low, temp);
    vector_subtract(a, temp, remainder);
    return low;
}

void vector_long_divide(vector<int>& a, vector<int>& b, vector<int>& quotient, vector<int>& remainder) {
    quotient.clear();
    remainder.clear();

    int cmp = vector_compare(a, b);
    if (cmp < 0) {
        quotient.push_back(0);
        remainder = a;
        return;
    }
    if (cmp == 0) {
        quotient.push_back(1);
        remainder.push_back(0);
        return;
    }

    int n1 = a.size();
    int n2 = b.size();

    vector<int> temp;
    int cur_digit;
    for (int i = n1 - n2; i < n1; i++)
        temp.push_back(a[i]);
    cur_digit = n1 - n2;

    while (cur_digit >= 0) {
        int q = vector_divide(temp, b, remainder);
        quotient.push_back(q);

        cur_digit--;
        if (cur_digit >= 0) {
            temp.clear();
            temp.push_back(a[cur_digit]);
            temp.insert(temp.end(), remainder.begin(), remainder.end());
        }
    }

    reverse(quotient.begin(), quotient.end());
}

static bool is_zero(UnlimitedInt* num) {
    int* arr = num->get_array();
    for (int i = 0; i < num->get_size(); i++)
        if (arr[i] != 0)
            return false;
    return true;
}

static int compare_unlimited_ints(UnlimitedInt* a, UnlimitedInt* b) {
    if (a->get_sign() == 1 && b->get_sign() == -1)
        return 1;
    if (a->get_sign() == -1 && b->get_sign() == 1)
        return -1;

    if (a->get_size() > b->get_size())
        return a->get_sign();
    if (a->get_size() < b->get_size())
        return -(a->get_sign());

    int* arr1 = a->get_array();
    int* arr2 = b->get_array();
    for (int i = a->get_size() - 1; i >= 0; i--) {
        if (arr1[i] > arr2[i])
            return a->get_sign();
        if (arr1[i] < arr2[i])
            return -(a->get_sign());
    }

    return 0;
}

UnlimitedInt::UnlimitedInt() {
    size = 0;
    sign = 1;
    data = NULL;
}

UnlimitedInt::UnlimitedInt(string s) {
    size = 0;
    sign = 1;

    vector<int> temp;
    int offset = -1;

    int first_digit = 0;
    while (first_digit < s.size() && s[first_digit] == ' ')
        first_digit++;
    if (first_digit == s.size())
        return;

    if (s[first_digit] == '-') {
        sign = -1;
        first_digit++;
    } else if (s[first_digit] == '+') {
        sign = 1;
        first_digit++;
    }
    if (first_digit == s.size())
        return;
    if (s[first_digit] < '0' || s[first_digit] > '9')
        return;

    int last_digit;
    for (last_digit = first_digit + 1; last_digit < s.size(); last_digit++)
        if (s[last_digit] < '0' || s[last_digit] > '9')
            break;
    last_digit--;

    int num_digits = 0, multiplier = 1;
    for (int digit = last_digit; digit >= first_digit; digit--) {
        if (num_digits % 9 == 0) {
            temp.push_back(0);
            offset++;
            multiplier = 1;
        }
        temp[offset] += ((int)(s[digit] - '0')) * multiplier;
        multiplier *= 10;
        num_digits++;
    }

    size = temp.size();
    data = new int[size];
    for (int i = 0; i < size; i++)
        data[i] = temp[i];
}

UnlimitedInt::UnlimitedInt(int i) {
    size = 1;
    sign = i < 0 ? -1 : 1;
    data = new int[1];
    data[0] = i < 0 ? -i : i;
}

UnlimitedInt::UnlimitedInt(int* arr, int capacity, int sgn, int sz) {
    data = new int;
    *data = *arr;
    size = sz;
    sign = sgn;
}

UnlimitedInt::~UnlimitedInt() {
    if (data)
        delete data;
}

int UnlimitedInt::get_size() {
    return size;
}

int* UnlimitedInt::get_array() {
    return data;
}

int UnlimitedInt::get_sign() {
    return sign;
}

int UnlimitedInt::get_capacity() {
    return 1;
}

UnlimitedInt* UnlimitedInt::add(UnlimitedInt* a, UnlimitedInt* b) {
    if (a->sign == 1 && b->sign == -1) {
        b->sign = 1;
        UnlimitedInt* result = sub(a, b);
        b->sign = -1;
        return result;
    } else if (a->sign == -1 && b->sign == 1) {
        a->sign = 1;
        UnlimitedInt* result = sub(b, a);
        a->sign = -1;
        return result;
    }

    if (a->size < b->size) {
        UnlimitedInt* temp = a;
        a = b;
        b = temp;
    }

    vector<int> temp_vec;
    for (int i = 0; i < a->size; i++)
        temp_vec.push_back(a->data[i]);
    for (int i = 0; i < b->size; i++)
        temp_vec[i] += b->data[i];

    for (int i = 0; i < temp_vec.size(); i++) {
        if (temp_vec[i] >= BASE) {
            if (i == temp_vec.size() - 1)
                temp_vec.push_back(0);
            temp_vec[i+1]++;
            temp_vec[i] -= BASE;
        }
    }

// Subtract secondInt from firstInt
UnlimitedInt* UnlimitedInt::subtract(UnlimitedInt* firstInt, UnlimitedInt* secondInt) {
    if (firstInt->sign == 1 && secondInt->sign == -1) {
        secondInt->sign = 1;
        UnlimitedInt* result = add(firstInt, secondInt);
        secondInt->sign = -1;
        return result;
    }
    else if (firstInt->sign == -1 && secondInt->sign == 1) {
        firstInt->sign = 1;
        UnlimitedInt* result = add(firstInt, secondInt);
        firstInt->sign = -1;
        result->sign = -1;
        return result;
    }

    int comparison = ui_compare(firstInt, secondInt);
    if (comparison == 0)
        return new UnlimitedInt(0);

    if (comparison < 0) {
        UnlimitedInt* temp = firstInt;
        firstInt = secondInt;
        secondInt = temp;
    }

    vector<int> tempVec;
    for (int i = 0; i < firstInt->size; i++)
        tempVec.push_back(firstInt->unlimited_int[i]);
    for (int i = 0; i < secondInt->size; i++)
        tempVec[i] -= secondInt->unlimited_int[i];

    for (int i = 0; i < tempVec.size(); i++) {
        if (tempVec[i] < 0) {
            tempVec[i+1]--;
            tempVec[i] += MAX_VALUE;
        }
    }

    UnlimitedInt* result = new UnlimitedInt();
    result->size = tempVec.size();
    if (comparison < 0)
        result->sign = -1;

    for (int i = tempVec.size()-1; tempVec[i] == 0; i--)
        result->size--;

    if (result->size == 0) {
        result->unlimited_int = new int[1];
        result->unlimited_int[0] = 0;
    }
    else {
        result->unlimited_int = new int[result->size];
        for (int i = 0; i < result->size; i++)
            result->unlimited_int[i] = tempVec[i];
    }

    return result;
}

// Multiply firstInt by secondInt
UnlimitedInt* UnlimitedInt::multiply(UnlimitedInt* firstInt, UnlimitedInt* secondInt) {
    if ((firstInt->size == 1 && firstInt->unlimited_int[0] == 0) || (secondInt->size == 1 && secondInt->unlimited_int[0] == 0))
        return new UnlimitedInt(0);

    if (firstInt->size < secondInt->size) {
        UnlimitedInt* temp = firstInt;
        firstInt = secondInt;
        secondInt = temp;
    }

    vector<uint64_t> tempVec;
    tempVec.resize(firstInt->size + secondInt->size, 0);

    for (int j = 0; j < secondInt->size; j++) {
        for (int i = 0; i < firstInt->size; i++) {
            tempVec[i+j] += static_cast<uint64_t>(firstInt->unlimited_int[i]) * static_cast<uint64_t>(secondInt->unlimited_int[j]);
        }
        for (int i = 0; i < firstInt->size; i++) {
            if (tempVec[i+j] >= MAX_VALUE) {
                tempVec[i+j+1] += tempVec[i+j] / MAX_VALUE;
                tempVec[i+j] = tempVec[i+j] % MAX_VALUE;
            }
        }
    }

    UnlimitedInt* result = new UnlimitedInt();
    result->size = tempVec.size();
    result->sign = firstInt->sign * secondInt->sign;

    for (int i = tempVec.size()-1; tempVec[i] == 0; i--)
        result->size--;

    result->unlimited_int = new int[result->size];
    for (int i = 0; i < result->size; i++)
        result->unlimited_int[i] = static_cast<int>(tempVec[i]);

    return result;
}

// Divide firstInt by secondInt
UnlimitedInt* UnlimitedInt::divide(UnlimitedInt* firstInt, UnlimitedInt* secondInt) {
    if (ui_iszero(secondInt))
        throw runtime_error("Division by zero");

    int comparison = ui_compare(firstInt, secondInt);
    if (comparison == 0)
        return new UnlimitedInt(1);

    vector<int> vec1, vec2, quotient, remainder;
    for (int i = 0; i < firstInt->size; i++)
        vec1.push_back(firstInt->unlimited_int[i]);
    for (int i = 0; i < secondInt->size; i++)
        vec2.push_back(secondInt->unlimited_int[i]);
    vec_divide(vec1, vec2, quotient, remainder);

    UnlimitedInt* result = new UnlimitedInt();
    result->sign = firstInt->sign * secondInt->sign;
    result->size = quotient.size();
    result->unlimited_int = new int[result->size];
    for (int i = 0; i < result->size; i++)
        result->unlimited_int[i] = quotient[i];
    if(result->sign < 0)
        result = UnlimitedInt::subtract(result, new UnlimitedInt(1));
    
    return result;
}

// Modulus of firstInt by secondInt
UnlimitedInt* UnlimitedInt::modulus(UnlimitedInt* firstInt, UnlimitedInt* secondInt) {
    if (ui_iszero(secondInt))
        throw runtime_error("Division by zero");

    int comparison = ui_compare(firstInt, secondInt);
    if (comparison == 0)
        return new UnlimitedInt(0);

    if (comparison < 0) {
        UnlimitedInt* result = new UnlimitedInt();
        result->size = firstInt->size;
        result->sign = firstInt->sign;
        result->unlimited_int = new int[result->size];
        for (int i = 0; i < firstInt->size; i++)
            result->unlimited_int[i] = firstInt->unlimited_int[i];
        return result;
    }

    if (firstInt->size == 1 && secondInt->size == 1)
        return new UnlimitedInt(firstInt->unlimited_int[0] % secondInt->unlimited_int[0]);

    vector<int> vec1, vec2, quotient, remainder;
    for (int i = 0; i < firstInt->size; i++)
        vec1.push_back(firstInt->unlimited_int[i]);
    for (int i = 0; i < secondInt->size; i++)
        vec2.push_back(secondInt->unlimited_int[i]);
    vec_divide(vec1, vec2, quotient, remainder);

    UnlimitedInt* result = new UnlimitedInt();
    result->size = remainder.size();
    result->unlimited_int = new int[result->size];
    for (int i = 0; i < result->size; i++)
        result->unlimited_int[i] = remainder[i];

    return result;
}

// Convert to string
string UnlimitedInt::toStr() {
    string str = "";
    if (sign == -1)
        str.push_back('-');

    for (int i = size-1; i >= 0; i--) {
        string tempStr = std::to_string(unlimited_int[i]);
        if (i != size-1) {
            if (tempStr.size() < MAX_DIGITS) {
                tempStr = string(MAX_DIGITS-tempStr.size(), '0') + tempStr;
            }
        }
        str += tempStr;
    }

    return str;
}
