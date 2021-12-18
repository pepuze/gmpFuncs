#include <chrono>
#include <iostream>
#include <algorithm>
#include <string>
#include <random>
#include <iomanip>
#include <ctime>
#include <fstream>
#include <windows.h>
#include <gmp.h>

#define DEFAULT_PREC 524288

//Арифметические функции с целочисленным типом:

std::chrono::nanoseconds sumArr_i(const unsigned int* arr, const unsigned int size){
    mpz_t sum; mpz_init(sum);

    auto start = std::chrono::steady_clock::now();
    for(unsigned int i = 0; i < size; ++i){
        mpz_add_ui(sum, sum, arr[i]);
    }
    auto end = std::chrono::steady_clock::now();
    auto deltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(end-start);

    std::cout << sum << "\n";
    mpz_clear(sum);
    return deltaTime;
}

std::chrono::nanoseconds subArr_i(const unsigned int* arr, const unsigned int size){
    mpz_t sub; mpz_init(sub);

    auto start = std::chrono::steady_clock::now();
    for(unsigned int i = 0; i < size; ++i){
        mpz_sub_ui(sub, sub, arr[i]);
    }
    auto end = std::chrono::steady_clock::now();
    auto deltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(end-start);
    
    std::cout << sub << "\n";
    mpz_clear(sub);
    return deltaTime;
}

std::chrono::nanoseconds mulArr_i(const unsigned int* arr, const unsigned int size){
    mpz_t mul; mpz_init_set_ui(mul, arr[0]);

    auto start = std::chrono::steady_clock::now();
    for(unsigned int i = 1; i < size; ++i){
        mpz_mul_ui(mul, mul, arr[i]);
    }
    auto end = std::chrono::steady_clock::now();
    auto deltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(end-start);

    std::cout << "check product_int.txt\n";
    std::ofstream fout("product_int.txt");
    fout << mul;
    fout.close();
    mpz_clear(mul);
    return deltaTime;
}

std::chrono::nanoseconds quotArr_i(const unsigned int* arr, const unsigned int size){
    unsigned int quot = arr[0];

    auto start = std::chrono::steady_clock::now();
    for(unsigned int i = 1; i < size; ++i){
        quot /= arr[i];
    }
    auto end = std::chrono::steady_clock::now();
    auto deltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(end-start);

    std::cout << quot << "\n";
    return deltaTime;
}

//Арифметические функции с типом с плавающей точкой:

std::chrono::nanoseconds sumArr_f(const double* arr, const unsigned int size, const int prec){
    mpf_t num; mpf_init(num);
    mpf_set_default_prec(prec);
    mpf_t sum; mpf_init(sum);

    auto start = std::chrono::steady_clock::now();
    for(unsigned int i = 0; i < size; ++i){
        mpf_set_d(num, arr[i]);
        mpf_add(sum, sum, num);
    }
    auto end = std::chrono::steady_clock::now();
    auto deltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(end-start);

    std::cout << std::fixed << sum << "\n";
    mpf_clear(sum); mpf_clear(num);
    return deltaTime;
}

std::chrono::nanoseconds subArr_f(const double* arr, const unsigned int size, const int prec){
    mpf_t num; mpf_init(num);
    mpf_set_default_prec(prec);
    mpf_t sub; mpf_init(sub);

    auto start = std::chrono::steady_clock::now();
    for(unsigned int i = 0; i < size; ++i){
        mpf_set_d(num, arr[i]);
        mpf_sub(sub, sub,num);
    }
    auto end = std::chrono::steady_clock::now();
    auto deltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(end-start);

    std::cout << std::fixed <<  sub << "\n";
    mpf_clear(sub); mpf_clear(num);
    return deltaTime;
}

std::chrono::nanoseconds mulArr_f(const double* arr, const unsigned int size, const int prec){
    mpf_t num; mpf_init(num);
    mpf_set_default_prec(prec);
    mpf_t mul; mpf_init(mul);
    mpf_set_d(mul, arr[0]);

    auto start = std::chrono::steady_clock::now();
    for(unsigned int i = 1; i < size; ++i){
        mpf_set_d(num, arr[i]);
        mpf_mul(mul, mul, num);
    }
    auto end = std::chrono::steady_clock::now();
    auto deltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(end-start);

    std::cout << "check product_float.txt\n";
    std::ofstream fout("product_float.txt");
    fout << std::fixed << std::setprecision(1000000) << mul;
    fout.close();
    mpf_clear(mul); mpf_clear(num);
    return deltaTime;
}

std::chrono::nanoseconds quotArr_f(const double* arr, const unsigned int size, const int prec){
    mpf_t num; mpf_init(num);
    mpf_set_default_prec(prec);
    mpf_t quot; mpf_init(quot);
    mpf_set_d(quot, arr[0]);

    auto start = std::chrono::steady_clock::now();
    for(unsigned int i = 1; i < size; ++i){
        mpf_set_d(num, arr[i]);
        mpf_div(quot, quot, num);
    }
    auto end = std::chrono::steady_clock::now();
    auto deltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(end-start);

    std::cout << "check quotient_float.txt\n";
    std::ofstream fout("quotient_float.txt");
    fout << std::fixed << std::setprecision(1000000) << quot;
    fout.close();
    mpf_clear(quot); mpf_clear(num);
    return deltaTime;
}


//Функция измеряющая время выполнения арифметических функций (INT)
template <class T>
void measureFuncTimeInt(std::chrono::nanoseconds func(const T*, const unsigned int), const T* arr, const unsigned int size) {
    auto deltaTime = func(arr, size);
    std::cout << "Calculation time:\n";
    std::cout << deltaTime.count()/1000000 << " milliseconds\n";
    std::cout << deltaTime.count() << " nanoseconds\n\n";
    return;
}

//Функция измеряющая время выполнения арифметических функций (FLOAT)
template <class T>
void measureFuncTimeFloat(std::chrono::nanoseconds func(const T*, const unsigned int, const int prec), const T* arr, const unsigned int size, const int prec) {
    auto deltaTime = func(arr, size, prec);
    std::cout << "Calculation time:\n";
    std::cout << deltaTime.count()/1000000 << " milliseconds\n";
    std::cout << deltaTime.count() << " nanoseconds\n\n";
    return;
}

//генерация массива типа double в диапазоне [l; r] (0 < l <= r)
void rand_fArray(double* arr, unsigned int size, double l, double r){
    auto start = std::chrono::steady_clock::now();
    for (unsigned int i = 0; i < size; ++i) {
        arr[i] = l + (double)(static_cast<float> (rand() / static_cast <float> (RAND_MAX / (r - l))));
    }
    auto end = std::chrono::steady_clock::now();
    auto deltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::cout << "Array generation time:\n";
    std::cout << deltaTime.count()/1000000 << " milliseconds\n";
    std::cout << deltaTime.count() << " nanoseconds\n\n";
    return;
}

//генерация массива типа unsigned int в диапазоне [l; r] (0 < l <= r)
void rand_iArray(unsigned int* arr, unsigned int size, unsigned int l, unsigned int r){
    auto start = std::chrono::steady_clock::now();
    for(unsigned int i = 0; i < size; ++i){
        arr[i] = l + (rand() % ( r - l + 1 ));
    }
    auto end = std::chrono::steady_clock::now();
    auto deltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::cout << "Array generation time:\n";
    std::cout << deltaTime.count()/1000000 << " milliseconds\n";
    std::cout << deltaTime.count() << " nanoseconds\n\n";
    return;
}

template <class T>
void printArray(const T* arr, const unsigned int size){
    std::ofstream fout;
    fout.open("array.txt");
    for(unsigned int i = 0; i < size; ++i){
        fout << arr[i] << " ";
        if((i+1)%15 == 0) fout << "\n";
    }
}


//Проверка введенного размера
bool checkSize(std::string size){
    if(size.find_first_not_of("1234567890.") != -1) return 0;
    double sizeDb = stod(size);
    if(sizeDb < 1 || round(sizeDb) != sizeDb) return 0;
    return 1;
}

//Проверка введенных границ
bool checkLims(std::string leftLim, std::string rightLim){
    if(leftLim.find_first_not_of("1234567890.") != -1 || rightLim.find_first_not_of("1234567890." )!= -1) return 0;
    double ll = stod(leftLim), lr = stod(rightLim);
    if(ll <= 0 || ceil(ll) > floor(lr)) return 0;
    return 1;
}

bool checkPrec(std::string prec){
    if(prec.find_first_not_of("1234567890") != -1) return 0;
    int precInt = std::stoi(prec);
    if(precInt < 0) return 0;
    return 1;
}

int main()
{
    std::ios::sync_with_stdio(0);
    srand(time(0));
    std::string type, sizeStr, leftLimStr, rightLimStr, prec;
    unsigned int size;
    double leftLim, rightLim;

    while(1){
        std::cout << "Select array type:\nI - Int\nF - Float\n";
        std::cin >> type;

        for(char& c : type) c = tolower(c);

        if(type == "int" || (type.length() == 1 && type[0] == 'i')){
            std::cout << "Enter array size:\n";
            std::cin >> sizeStr;
//Int
            if(!checkSize(sizeStr)){
                std::cout << "Invalid size\n\n";
                continue;
            }
            size = (unsigned int)(stod(sizeStr));

            std::cout << "Enter array range [a; b] (a > 0):\n";
            std::cin >> leftLimStr >> rightLimStr;

            if(!checkLims(leftLimStr, rightLimStr)){
                std::cout << "Invalid range\n\n";
                continue;
            }
            leftLim = stod(leftLimStr); rightLim = stod(rightLimStr);
            std::cout << "\n";

            unsigned int* arr = new unsigned int[size];
            rand_iArray(arr, size, (unsigned int)(ceil(leftLim)), (unsigned int)(floor(rightLim)));
            //printArray(arr, size);

            std::cout << "Sum: ";
            measureFuncTimeInt(sumArr_i, arr, size);
            std::cout << "Difference: ";
            measureFuncTimeInt(subArr_i, arr, size);
            std::cout << "Product: ";
            measureFuncTimeInt(mulArr_i, arr, size);
            std::cout << "Quotient: ";
            measureFuncTimeInt(quotArr_i, arr, size);

            std::cout << "\n\n";
            delete[] arr;
        }
//Float
        else if (type == "float" || (type.length() == 1 && type[0] == 'f')){
            double leftLim, rightLim;
            std::cout << "Enter array size:\n";
            std::cin >> sizeStr;

            if(!checkSize(sizeStr)){
                std::cout << "Invalid size\n\n";
                continue;
            }
            size = (unsigned int)(stod(sizeStr));

            std::cout << "Enter array range [a; b] (a > 0):\n";
            std::cin >> leftLimStr >> rightLimStr;

            if(!checkLims(leftLimStr, rightLimStr)){
                std::cout << "Invalid range\n\n";
                continue;
            }
            leftLim = stod(leftLimStr); rightLim = stod(rightLimStr);

            std::cout << "Enter desired precision (bytes, 0 = default = 524288 bytes):\n";
            std::cin >> prec;
            if(!checkPrec(prec)){
                std::cout << "Invalid precision (must be not negative integer)\n\n";
                continue;
            }
            int precInt = std::stoi(prec);
            if(precInt == 0) precInt = DEFAULT_PREC;
            std::cout << "\n";

            double* arr = new double[size];
            rand_fArray(arr, size, leftLim, rightLim);
            //printArray(arr, size);

            std::cout << "Sum: ";
            measureFuncTimeFloat(sumArr_f, arr, size, precInt);
            std::cout << "Difference: ";
            measureFuncTimeFloat(subArr_f, arr, size, precInt);
            std::cout << "Product: ";
            measureFuncTimeFloat(mulArr_f, arr, size, precInt);
            std::cout << "Quotient: ";
            measureFuncTimeFloat(quotArr_f, arr, size, precInt);

            std::cout << "\n\n";
            delete[] arr;
        }

        else std::cout << "Invalid type\n\n";

    }
}