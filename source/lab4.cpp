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

//Арифметические функции с целочисленным типом:

void sumArr_i(const unsigned int* arr, const unsigned int size){
    mpz_t sum;
    mpz_init(sum);

    for(unsigned int i = 0; i < size; ++i){
        mpz_add_ui(sum, sum, arr[i]);
    }

    std::cout << sum << "\n";
    mpz_clear(sum);
}

void subArr_i(const unsigned int* arr, const unsigned int size){
    mpz_t sub;
    mpz_init(sub);

    for(unsigned int i = 0; i < size; ++i){
        mpz_sub_ui(sub, sub, arr[i]);
    }

    std::cout << sub << "\n";
    mpz_clear(sub);
}

void mulArr_i(const unsigned int* arr, const unsigned int size){
    mpz_t mul;
    mpz_init_set_ui(mul, arr[0]);

    for(unsigned int i = 1; i < size; ++i){
        mpz_mul_ui(mul, mul, arr[i]);
    }

    std::cout << mul << "\n";
    mpz_clear(mul);
}

void quotArr_i(const unsigned int* arr, const unsigned int size){
    unsigned int quot = arr[0];

    for(unsigned int i = 1; i < size; ++i){
        quot /= arr[i];
    }

    std::cout << quot << "\n";
}

//Арифметические функции с типом с плавающей точкой:

void sumArr_f(const double* arr, const unsigned int size){
    mpf_set_default_prec(32);
    mpf_t sum; mpf_init(sum);
    mpf_t num; mpf_init(num);
    for(unsigned int i = 0; i < size; ++i){
        mpf_set_d(num, arr[i]);
        mpf_add(sum, sum, num);
    }

    std::cout << std::fixed << sum << "\n";
    mpf_clear(sum); mpf_clear(num);
}

void subArr_f(const double* arr, const unsigned int size){
    mpf_set_default_prec(32);
    mpf_t sub; mpf_init(sub);
    mpf_t num; mpf_init(num);

    for(unsigned int i = 0; i < size; ++i){
        mpf_set_d(num, arr[i]);
        mpf_sub(sub, sub,num);
    }

    std::cout << std::fixed <<  sub << "\n";
    mpf_clear(sub); mpf_clear(num);
}

void mulArr_f(const double* arr, const unsigned int size){
    mpf_set_default_prec(32);
    mpf_t mul; mpf_init(mul);
    mpf_t num; mpf_init(num);
    mpf_set_d(mul, arr[0]);

    for(unsigned int i = 1; i < size; ++i){
        mpf_set_d(num, arr[i]);
        mpf_mul(mul, mul, num);
    }

    std::cout << std::fixed << std::setprecision(50) << mul << "\n";
    mpf_clear(mul); mpf_clear(num);
}

void quotArr_f(const double* arr, const unsigned int size){
    mpf_set_default_prec(32);
    mpf_t quot; mpf_init(quot);
    mpf_t num; mpf_init(num);
    mpf_set_d(quot, arr[0]);

    for(unsigned int i = 1; i < size; ++i){
        mpf_set_d(num, arr[i]);
        mpf_div(quot, quot, num);
    }

    std::cout << std::fixed << std::setprecision(50) << quot << "\n";
    mpf_clear(quot); mpf_clear(num);
}


//Функция измеряющая время выполнения арифметических функций
template <class T>
void measureFuncTime(void func(const T*, const unsigned int), const T* arr, const unsigned int size) {
    auto start = std::chrono::steady_clock::now();

    func(arr, size);

    auto end = std::chrono::steady_clock::now();
    auto deltaTimeMl = std::chrono::duration_cast<std::chrono::microseconds>(end - start); auto deltaTimeMc = std::chrono::duration_cast<std::chrono::nanoseconds>(end-start);
    std::cout << deltaTimeMl.count() << " microseconds\n";
    std::cout << deltaTimeMc.count() << " nanoseconds\n";
    return;
}

//генерация массива типа double в диапазоне [l; r] (0 < l <= r)
void rand_fArray(double* arr, unsigned int size, double l, double r){
    for (unsigned int i = 0; i < size; ++i) {
        arr[i] = l + (double)(static_cast<float> (rand() / static_cast <float> (RAND_MAX / (r - l))));
    }
    return;
}

//генерация массива типа unsigned int в диапазоне [l; r] (0 < l <= r)
void rand_iArray(unsigned int* arr, unsigned int size, unsigned int l, unsigned int r){
    for(unsigned int i = 0; i < size; ++i){
        arr[i] = l + (rand() % ( r - l + 1 ));
    }
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

int main()
{
    srand(time(0));
    unsigned int size;
    char type;

    while(1){
        std::cout << "Select array type:\nI - Int\nF-Float\n";
        std::cin >> type;

        if(tolower(type) == 'i'){
            unsigned int leftLim, rightLim;
            std::cout << "Enter array size:\n";
            std::cin >> size;
            std::cout << "Enter array range [a; b]:\n";
            std::cin >> leftLim >> rightLim;

            if(rightLim - leftLim < 0){
                std::cout << "Invalid range\n\n";
                continue;
            }
            unsigned int* arr = new unsigned int[size];
            rand_iArray(arr, size, leftLim, rightLim);
            //printArray(arr, size);

            measureFuncTime(sumArr_i, arr, size);
            measureFuncTime(subArr_i, arr, size);
            measureFuncTime(mulArr_i, arr, size);
            measureFuncTime(quotArr_i, arr, size);

            std::cout << "\n\n";
            delete[] arr;
        }

        else if (tolower(type) == 'f'){
            double leftLim, rightLim;
            std::cout << "Enter array size:\n";
            std::cin >> size;
            std::cout << "Enter array range [a; b]:\n";
            std::cin >> leftLim >> rightLim;

            if(rightLim - leftLim < 0){
                std::cout << "Invalid range\n\n";
                continue;
            }
            double* arr = new double[size];
            rand_fArray(arr, size, leftLim, rightLim);
            //printArray(arr, size);

            measureFuncTime(sumArr_f, arr, size);
            measureFuncTime(subArr_f, arr, size);
            measureFuncTime(mulArr_f, arr, size);
            measureFuncTime(quotArr_f, arr, size);

            std::cout << "\n\n";
            delete[] arr;
        }

        else std::cout << "Invalid type\n\n";

    }
}
