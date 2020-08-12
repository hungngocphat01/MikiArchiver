#include <string>
#include <vector>
using namespace std;

struct argument {
    string flag;
    vector<string> arglst;
};

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

argument* getArg(vector<argument>& args, string flag) {
    for (unsigned i = 0; i < args.size(); i++) {
        if (args[i].flag == flag) {
            return &(args[i]);
        }
    }
    return nullptr;
}