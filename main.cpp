#include "mikilib.h"
#include "arguments.h"

#define VERSION "2.0"

int main (int argc, char** argv) {
    try {
        vector<argument> args = parseArguments(argc, argv);

        argument* e_arg = getArg(args, "-e");
        argument* o_arg = getArg(args, "-o");

        // Extract mode
        if (e_arg) {
            if (o_arg) {
                extractArchive(e_arg->arglst[0], o_arg->arglst[0]);
            }
            else {
                throw runtime_error("No outfile specified.");
            }
            exit(0);
        }

        argument* a_arg = getArg(args, "-a");
        argument* chunksize_arg = getArg(args, "--chunksize");

        if (a_arg) {
            short chunksize = 1024;
            if (chunksize_arg) {
                chunksize = stoi(chunksize_arg->arglst[0]);
            }

            if (o_arg) {
                mkarchive filelst = buildFromFiles(a_arg->arglst);
                writeArchive(filelst, o_arg->arglst[0], chunksize);

                cout << "Archive created." << endl;
                printArchiveContent(filelst);
            }
            else {
                throw runtime_error("No output directory specified.");
            }
            exit(0);
        }

        argument* d_arg = getArg(args, "-d");

        if (d_arg) {
            short chunksize = 1024;
            if (chunksize_arg) {
                chunksize = stoi(chunksize_arg->arglst[0]);
            }

            if (o_arg) {
                mkarchive filelst = buildFromDir(d_arg->arglst[0]);
                writeArchive(filelst, o_arg->arglst[0], chunksize);

                cout << "Archive created." << endl;
                printArchiveContent(filelst);
            }
            else {
                throw runtime_error("No output directory specified.");
            }
            exit(0);
        }
        else {
            cout << "MikiArchiver v" << VERSION << endl;
            cout << "Simple file archiver by hungngocphat01." << endl;
            cout << "Powered by C++14." << endl;
            cout << endl;
            cout << "Usage: mk [-e|-a|-d] [filename|filenames|path] -o [outfile|outdir]" << endl;
            cout << endl;
            cout << "-e: Extract from archive. Uses -o to specify the output directory." << endl;
            cout << "-a: Create archive a list of files. Uses -o to specify the output archive filename." << endl;
            cout << "-d: Create archive a directory. Uses -o to specify the output archive filename." << endl;
        }
    }
    catch (const exception& e) {
        cout << "An error has occurred: " << e.what() << endl;
        return 1;
    }
    return 0;
}