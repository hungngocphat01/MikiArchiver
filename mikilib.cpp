#include "mikilib.h"

size_t getFileSize(FILE* fp) {
    if (fp == nullptr) {
        throw runtime_error("Cannot query filesize. File corrupted or not exist.");
    }

    fseek(fp, 0, SEEK_END);
    size_t filesize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    return filesize;
}

string getFullPath(mkfile f) {
    if (f.path[f.path.size() - 1] == '/') {
        return (f.path + f.filename);
    }
    return (f.path + "/" + f.filename);
}

mkarchive buildCurrentDir(string path) {
    DIR* dir = opendir(path.c_str());
    dirent* entry;
    mkarchive rArchive;

    if (dir == nullptr) {
        throw runtime_error("Cannot open current directory.");
    }

    do {
        entry = readdir(dir);
        if (entry != nullptr) {
            // Only push if name is not "." and ".."
            if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {
                mkfile temp = {path, string(entry->d_name), 0};
                rArchive.push_back(temp);
            }
        }
    } while(entry != nullptr);

    closedir(dir);
    return rArchive;
}

mkarchive buildFromFiles(vector<string> filelst) {
    mkarchive rArchive;

    for (string filename : filelst) {
        int delimiter = filename.find_last_of("/\\");
        mkfile temp = {filename.substr(0, delimiter), filename.substr(delimiter + 1), 0};
        rArchive.push_back(temp);
    }

    return rArchive;
}

inline string err_gen(vector<string> v) {
    string result;
    for (string i : v) {
        result += i;
    }
    return result;
}

unsigned writeArchive(mkarchive& archive, string filename, short chunk_size) {
    unsigned tbw = 0; // total bytes written
    // Archive file
    FILE* farchive = fopen(filename.c_str(), "wb");

    if (farchive == nullptr) {
        throw runtime_error("Archive file cannot be created. Terminating...");
    }

    // Write the quantity of files
    const short nfiles = static_cast<short>(archive.size());
    tbw += fwrite(&nfiles, 1, sizeof(nfiles), farchive);

    // Write the chunk size
    tbw += fwrite(&chunk_size, 1, sizeof(chunk_size), farchive);

    for (unsigned i = 0; i < nfiles; i++) {
        mkfile entry = archive[i];

        // Read the file
        FILE* ifp = fopen(getFullPath(entry).c_str(), "rb");

        if (ifp == nullptr) {
            throw runtime_error(string("File corrupted or not exist: ") + entry.filename);
        }

        // Write the filename's size
        const char* ifilename = entry.filename.c_str();
        const short ifilenamel = strlen(ifilename) + 1;
        tbw += fwrite(&ifilenamel, 1, sizeof(ifilenamel), farchive);
        // Write the filename
        tbw += fwrite(ifilename, 1, ifilenamel, farchive);
        
        // The filesize
        entry.filesize = getFileSize(ifp);
        // Write the filesize
        tbw += fwrite(&entry.filesize, 1, sizeof(entry.filesize), farchive);

        // Split into chunks
        char buffer[chunk_size];
        size_t chunks = entry.filesize / chunk_size;
        size_t remainder = entry.filesize % chunk_size;
        
        // Copy content
        for (unsigned i = 0; i < chunks + 1; i++) {
            size_t buffer_size = (i < chunks) ? chunk_size : remainder;
            // Read the content
            unsigned short read_bytes = fread(buffer, 1, buffer_size, ifp);
            if (read_bytes != buffer_size) {
                vector<string> err = {to_string(read_bytes), " bytes read in 1 chunk instead of ", to_string(chunk_size), " while reading ", entry.filename};
                throw runtime_error(err_gen(err));
            }
            // Write the content
            unsigned short wrote_bytes = fwrite(buffer, 1, buffer_size, farchive);
            tbw += wrote_bytes;

            if (wrote_bytes != buffer_size) {
                vector<string> err = {to_string(wrote_bytes), " bytes wrote in 1 chunk instead of ", to_string(chunk_size), " while writing ", entry.filename};
                throw runtime_error(err_gen(err));
            }
        }  
        fclose(ifp);
    }
    fclose(farchive);
    return tbw;
}

unsigned extractArchive(string filename, string extract_path) {
    unsigned tbr = 0; // total bytes written
    // Archive file
    FILE* farchive = fopen(filename.c_str(), "rb");

    if (farchive == nullptr) {
        throw runtime_error("Archive file cannot be opened. Terminating...");
    }

    // Read the quantity of files
    short nfiles = 0;
    tbr += fread(&nfiles, 1, sizeof(nfiles), farchive);

    short chunk_size = 0;
    // Read the chunk size
    tbr += fread(&chunk_size, 1, sizeof(chunk_size), farchive);

    for (unsigned i = 0; i < nfiles; i++) {
        mkfile entry;

        // Read the filename's size
        short ifilenamel = 0;
        tbr += fread(&ifilenamel, 1, sizeof(ifilenamel), farchive);
        // Read the filename
        char ifilename[ifilenamel];
        tbr += fread(ifilename, 1, ifilenamel, farchive);
        entry.filename = string(ifilename);
        
        // Prepare file for writing
        entry.path = extract_path;
        FILE* ifp = nullptr;
        ifp = fopen(getFullPath(entry).c_str(), "wb");

        if (ifp == nullptr) {
            throw runtime_error(string("Cannot create file: ") + entry.filename);
        }

        // Read the filesize
        tbr += fread(&entry.filesize, 1, sizeof(entry.filesize), farchive);

        // Split into chunks
        char buffer[chunk_size];
        size_t chunks = entry.filesize / chunk_size;
        size_t remainder = entry.filesize % chunk_size;

        // Copy content
        for (unsigned i = 0; i < chunks + 1; i++) {
            size_t buffer_size = (i < chunks) ? chunk_size : remainder;
            // Read the content
            unsigned short read_bytes = fread(buffer, 1, buffer_size, farchive);
            if (read_bytes != buffer_size) {
                vector<string> err = {to_string(read_bytes), " bytes read in 1 chunk instead of ", to_string(chunk_size), " while reading ", entry.filename};
                throw runtime_error(err_gen(err));
            }
            // Write the content
            unsigned short wrote_bytes = fwrite(buffer, 1, buffer_size, ifp);
            tbr += wrote_bytes;

            if (wrote_bytes != buffer_size) {
                vector<string> err = {to_string(wrote_bytes), " bytes wrote in 1 chunk instead of ", to_string(chunk_size), " while writing ", entry.filename};
                throw runtime_error(err_gen(err));
            }
        }  
        if (ifp != nullptr) {
            fclose(ifp);
        }
    }
    fclose(farchive);
    return tbr;
}