#include <string>
#include <vector>
using namespace std;

struct argument {
    string flag;
    vector<string> arglst;
};

argument EMPTY_ARG {"EMPTY"};

vector<argument> parseArguments(int argc, char** argv) {
    vector<argument> presult;
    int curr_arg = -1;

    for (unsigned i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            argument temp = {string(argv[i])};
            presult.push_back(temp);
            curr_arg++;
        }
        else {
            presult[curr_arg].arglst.push_back(string(argv[i]));
        }
    }
    return presult;
}

bool ifexist(vector<argument> args, string flag) {
    for (argument arg : args) {
        if (arg.flag == flag) {
            return true;
        }
    }
    return false;
}

argument getArg(vector<argument> args, string flag) {
    for (argument arg : args) {
        if (arg.flag == flag) {
            return arg;
        }
    }
    return EMPTY_ARG;
}