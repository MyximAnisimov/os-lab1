#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <chrono>
#include <queue>
#include <stack>
#include <algorithm>
#include <limits>
#include <random>
#include <fcntl.h>
#include <io.h>
#include <sys/stat.h>
#include "string"

using namespace std;
using namespace chrono;

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
    cout<<"prime factors"<<endl;
    cout << "================" << endl;
    cout << "Total time: " << duration.count() << " ms" << endl;
    cout << "================" << endl;
    return factors;
}

void measureWriteSpeed(const string& filename, size_t blockSize, size_t totalBlocks) {
    vector<char> buffer(blockSize, 'A');

    int fd = open(filename.c_str(), O_WRONLY | O_CREAT | O_TRUNC | O_BINARY, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

    if (fd == -1) {
        cerr << "Error opening file!" << endl;
        return;
    }

    auto start = chrono::high_resolution_clock::now();

    for (size_t i = 0; i < totalBlocks; ++i) {
        int bytesWritten = _write(fd, buffer.data(), blockSize);
        if (bytesWritten == -1) {
            cerr << "Error writing to file!" << endl;
            _close(fd);
            return;
        }
    }

    auto end = chrono::high_resolution_clock::now();

    _close(fd);

    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    double speed = (blockSize * totalBlocks) / (duration.count() * 1024.0);
    cout<<"measure write speed"<<endl;
    cout << "================" << endl;
    cout << "Total time: " << duration.count() / 1000 << " s" << endl;
    cout << "Write speed: " << speed << " MB/s" << endl;
    cout << "================" << endl;
}

int main(int argc, char* argv[]) {
  if (argc != 6) {
    cerr << "Usage: " << argv[0] << " <number> <filename> <blockSize> <totalBlocks> <iterations>\n";
    return 1;
  }

  long long number = atoll(argv[1]);
  string filename = argv[2];
  size_t blockSize = static_cast<size_t>(atoi(argv[3]));
  size_t totalBlocks = static_cast<size_t>(atoi(argv[4]));
  size_t iterations = static_cast<size_t>(atoi(argv[5]));

    vector<thread> threads;

    for (int i = 0; i < iterations; i++) {
        threads.push_back(thread([=]() {
            measureWriteSpeed(filename + to_string(i), blockSize, totalBlocks);
        }));
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
