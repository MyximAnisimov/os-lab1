#include <iostream>
#include <vector>
#include "string"
#include <chrono>
#include <fcntl.h>
#include <io.h>
#include <sys/stat.h>

using namespace std;

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

    cout << "================" << endl;
    cout << "Total time: " << duration.count() / 1000 << " s" << endl;
    cout << "Write speed: " << speed << " MB/s" << endl;
    cout << "================" << endl;
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        cerr << "Usage: " << argv[0] << " <filename> <blockSize> <totalBlocks> <count>\n";
        return 1;
    }

    string filename = argv[1];
    size_t blockSize = static_cast<size_t>(stoi(argv[2]));
    size_t totalBlocks = static_cast<size_t>(stoi(argv[3]));
    size_t repetitions = static_cast<size_t>(stoi(argv[4]));

    for (int i = 0; i < repetitions; i++) {
          measureWriteSpeed(filename, blockSize, totalBlocks);
    }

    return 0;
}



