#include <iostream>
#include <vector>
#include <string>
#include <exception>
#include <stdexcept>
#include <cstring>
#include <dirent.h>

using namespace std;

struct mkfile {
    string path;
    string filename;
    size_t filesize = 0;
};

using mkarchive = vector<mkfile>;

mkarchive buildCurrentDir(string path);
mkarchive buildFromFiles(vector<string> filelst);

unsigned writeArchive(mkarchive& archive, string filename, short chunk_size);
unsigned extractArchive(string filename, string extract_path);