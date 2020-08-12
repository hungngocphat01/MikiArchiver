#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <cstring>
#include <exception>
#include <stdexcept>
#include <dirent.h>

using namespace std;

struct mkfile {
    string path;
    string filename;
    size_t filesize = 0;
};

using mkarchive = vector<mkfile>;

mkarchive buildFromDir(string path);
mkarchive buildFromFiles(vector<string> filelst);

unsigned writeArchive(mkarchive& archive, string filename, short chunk_size);
unsigned extractArchive(string filename, string extract_path);
void printArchiveContent(const mkarchive& archive);