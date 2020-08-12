#include "mikilib.h"
#include "arguments.h"

#define VERSION "2.0"

int main (int argc, char** argv) {
    mkarchive filelst;

    argument* e_arg = getArg(argc, argv, "-e");
    argument* o_arg = getArg(argc, argv, "-o");
    argument* a_arg = getArg(argc, argv, "-a");
    argument* chunksize_arg = getArg(argc, argv, "--chunksize");
    argument* d_arg = getArg(argc, argv, "-d");

    // Extract mode
    if (e_arg) {
        if (o_arg) {
            extractArchive(filelst, e_arg->arglst[0], o_arg->arglst[0]);

            cout << "Files extracted." << endl << endl;
            printArchiveContent(filelst);
        }
        else {
            cerr << "Error: no output directory specified." << endl;
        }
    }
    // Archive mode
    else if (a_arg) {
        short chunksize = 1024;
        if (chunksize_arg = getArg(argc, argv, "--chunksize")) {
            chunksize = stoi(chunksize_arg->arglst[0]);
        }

        if (o_arg) {
            filelst = buildFromFiles(a_arg->arglst);
            writeArchive(filelst, o_arg->arglst[0], chunksize);

            cout << "Archive created." << endl << endl;
            printArchiveContent(filelst);
        }
        else {
            cerr << "Error: no output archive file specified." << endl;
        }
    }
    // Archive mode (dir)
    else if (d_arg) {
        short chunksize = 1024;
        if (chunksize_arg = getArg(argc, argv, "--chunksize")) {
            chunksize = stoi(chunksize_arg->arglst[0]);
        }

        if (o_arg) {
            filelst = buildFromDir(d_arg->arglst[0]);
            writeArchive(filelst, o_arg->arglst[0], chunksize);

            cout << "Archive created." << endl << endl;
            printArchiveContent(filelst);
        }
        else {
            cerr << "Error: no output archive file specified." << endl;
        }
        exit(0);
    }
    // Print help message
    else {
        cout << "MikiArchiver v" << VERSION << endl;
        cout << "Simple file archiver by hungngocphat01." << endl;
        cout << "Powered by C++14." << endl;
        cout << endl;
        cout << "Usage: mk [-e|-a|-d] [filename|filenames|path] -o [outfile|outdir]" << endl;
        cout << endl;
        cout << "-e: Extract from archive. Uses -o to specify the output directory." << endl;
        cout << "-a: Create archive from a list of files. Uses -o to specify the output archive filename." << endl;
        cout << "-d: Create archive from a directory. Uses -o to specify the output archive filename." << endl < endl;
        cout << "Example: " << endl;
        cout << "# Extract from 'myarchive.mka' to 'mydirectory'" << endl;
        cout << "$ mk -e myarchive.mka -o mydirectory" << endl << endl;
        cout << "# Make an archive from 'file1.cpp', 'file2.exe' and 'file3.mp4'" << endl;
        cout << "$ mk -a file1.cpp file2.exe file3.mp4 -o myarchive.mka" << endl << endl;
        cout << "# Make an archive from 'mydir'" << endl;
        cout << "$ mk -d mydir -o myarchive.mka" << endl << endl;
    }

    delArg(5, &e_arg, &o_arg, &d_arg, &a_arg, &chunksize_arg);
    return 0;
}
