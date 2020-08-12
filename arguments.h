#include <string>
#include <cstring>
#include <vector>
#include <stdarg.h>
using namespace std;

struct argument {
    string flag;
    vector<string> arglst;
};

argument* getArg(int argc, char** argv, string flag) {
    argument* result = nullptr;

    for (unsigned i = 1; i < argc; i++) {
        // Encountered the needed flag
        if (!strcmp(argv[i], flag.c_str())) {
            if (result == nullptr) {
                result = new argument;
                result->flag = flag;
            }
            else {
                cerr << "Duplicated flag: " << flag << endl;
                exit(1);
            }
        }
        // Another flag encountered
        else if (result != nullptr && argv[i][0] == '-') {
            break;
        }
        // Process the current flag's sub-args
        else if (result != nullptr && argv[i][0] != '-') {
            (result->arglst).push_back(string(argv[i]));
        }
    }
    return result;
}

void delArg(int n, ...) {
    va_list args;
    va_start(args, n);

    for (unsigned i = 0; i < n; i++) {
        argument** arg = va_arg(args, argument**);
        if (*arg != nullptr) {
            delete *arg; 
        }
    }

    va_end(args);
}