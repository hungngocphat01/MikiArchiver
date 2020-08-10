#include "mikilib.h"
#include "arguments.h"

int main () {
    mkarchive archive = buildCurrentDir("./testdir");
    for (mkfile i : archive) {
        cout << i.filename << endl;
    }

    unsigned n = writeArchive(archive, "test.mka", 1024);
    cout << n << " bytes written" << endl;

    n = extractArchive("test.mka", "./extract");
    cout << n << " bytes read" << endl;

    return 0;
}