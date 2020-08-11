#include "mikilib.h"
#include "arguments.h"

int main (int argc, char** argv) {
    vector<argument> args = parseArguments(argc, argv);

    // Yeah I know. Primitive argument handling, very bad performance. Will enhance it later. 
    // This is just a WIP anyway
    
    // Extract mode
    if (ifexist(args, "-e")) {
        if (ifexist(args, "-o")) {
            extractArchive(getArg(args, "-e").arglst[0], getArg(args, "-o").arglst[0]);
        }
        else {
            throw runtime_error("No outfile specified.");
        }
    }
    else if (ifexist(args, "-a")) {
        short chunksize = 1024;
        if (ifexist(args, "--chunksize")) {
            chunksize = stoi(getArg(args, "--chunksize").arglst[0]);
        }

        if (ifexist(args, "-o")) {
            mkarchive filelst = buildFromFiles(getArg(args, "-a").arglst);
            writeArchive(filelst, getArg(args, "-o").arglst[0], chunksize);
        }
        else {
            throw runtime_error("No output directory specified.");
        }
    }
    else if (ifexist(args, "-d")) {
        short chunksize = 1024;
        if (ifexist(args, "--chunksize")) {
            chunksize = stoi(getArg(args, "--chunksize").arglst[0]);
        }

        if (ifexist(args, "-o")) {
            mkarchive filelst = buildCurrentDir(getArg(args, "-d").arglst[0]);
            writeArchive(filelst, getArg(args, "-o").arglst[0], chunksize);
        }
        else {
            throw runtime_error("No output directory specified.");
        }
    }
    else {
        cout << "MikiArchiver v1.0" << endl;
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