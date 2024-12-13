#include <algorithm>
#include <chrono>
#include <iostream>
#include <limits>
#include <queue>
#include <stack>
#include <vector>
#include "string"
#include "thread"

using namespace std;
using namespace chrono;

struct Edge {
    int to, weight;
};

vector<int> primeFactors(long long n) {
    vector<int> factors;
auto s = high_resolution_clock::now();
    // Проверяем количество 2 в числе
    while (n % 2 == 0) {
        factors.push_back(2);
        n /= 2;
    }

    // Проверяем остальные нечетные числа
    for (int i = 3; i * i <= n; i += 2) {
        while (n % i == 0) {
            factors.push_back(i);
            n /= i;
        }
    }

    // Если n остается и больше 2, это простое число
    if (n > 2) {
        factors.push_back(n);
    }
    auto e = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(e - s);
    cout << "================" << endl;
    cout << "Total time: " << duration.count() << " ms" << endl;
    cout << "================" << endl;
    return factors;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cerr << "Using: " << argv[0] << " <number> <countOfThreads>\n";
        return 1;
    }
  long long number = stoll(argv[1]);
    int countOfThreads = stoi(argv[2]);


    if (number <= 1) {
        cout << "Number must be greater than 1." << endl;
        return 1;
    }

    vector<thread> threads;

    for (int i = 0; i < countOfThreads; i++) {
        threads.push_back(thread([=]() {
            primeFactors(number);
        }));
    }

    for (auto& t : threads) {
        t.join();
    }

    threads.clear();

    return 0;
}