#include "mikilib.h"

size_t getFileSize(FILE* fp) {
    if (fp == nullptr) {
        cerr << "Cannot query filesize. File corrupted or not exist." << endl;
        exit(1);
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

mkarchive buildFromDir(string path) {
    DIR* dir = opendir(path.c_str());
    dirent* entry;
    mkarchive rArchive;

    if (dir == nullptr) {
        cerr << "Cannot open current directory." << endl;
        exit(1);
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
        if (delimiter == -1) {
            temp.path = "./";
        }
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

unsigned writeArchive(mkarchive& archive, string filename, uint32_t chunk_size) {
    unsigned tbw = 0; // total bytes written
    // Archive file
    FILE* farchive = fopen(filename.c_str(), "wb");

    if (farchive == nullptr) {
        cerr << "Archive file cannot be created. Terminating..." << endl;
        exit(1);
    }

    char signal[8];
    memcpy(signal, "MIKIMIKI", 8);
    tbw += fwrite(signal, 1, 8, farchive);

    // Write the quantity of files
    const uint16_t nfiles = static_cast<uint16_t>(archive.size());
    tbw += fwrite(&nfiles, 1, sizeof(nfiles), farchive);

    // Write the chunk size
    tbw += fwrite(&chunk_size, 1, sizeof(chunk_size), farchive);

    for (unsigned i = 0; i < nfiles; i++) {
        mkfile& entry = archive[i];

        // Read the file
        FILE* ifp = fopen(getFullPath(entry).c_str(), "rb");

        if (ifp == nullptr) {
            cerr << "File corrupted or not exist: " << entry.filename << endl;
            exit(1);
        }

        // Write the filename's size
        const char* ifilename = entry.filename.c_str();
        const uint16_t ifilenamel = strlen(ifilename) + 1;
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
                cerr << "Error: " << read_bytes << " bytes read in 1 chunk instead of " << chunk_size << " while reading " << entry.filename << endl;
                exit(1);
            }
            // Write the content
            unsigned short wrote_bytes = fwrite(buffer, 1, buffer_size, farchive);
            tbw += wrote_bytes;

            if (wrote_bytes != buffer_size) {
                cerr << "Error: " << wrote_bytes << " bytes written in 1 chunk instead of " << chunk_size << " while writing " << entry.filename << endl;
                exit(1);
            }
        }  
        fclose(ifp);
    }
    fclose(farchive);
    return tbw;
}

unsigned extractArchive(mkarchive& archive, string filename, string extract_path) {
    unsigned tbr = 0; // total bytes written
    // Archive file
    FILE* farchive = fopen(filename.c_str(), "rb");

    if (farchive == nullptr) {
        cerr << "Error: " << filename << " is corrupted or not exist." << endl;
        exit(1);
    }

    // Read the signal
    char signal[8];
    char compr_signal[8];
    memcpy(compr_signal, "MIKIMIKI", 8);

    tbr += fread(signal, 1, 8, farchive);
    if (strcmp(signal, compr_signal) == 0) {
        cerr << "Error: not a valid MikiArchiver format." << endl;
        exit(1);
    }

    // Read the quantity of files
    uint16_t nfiles = 0;
    tbr += fread(&nfiles, 1, sizeof(nfiles), farchive);

    uint32_t chunk_size = 0;
    // Read the chunk size
    tbr += fread(&chunk_size, 1, sizeof(chunk_size), farchive);

    for (unsigned i = 0; i < nfiles; i++) {
        mkfile entry;

        // Read the filename's size
        uint16_t ifilenamel = 0;
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
            cerr << "Error: cannot create file: " << entry.filename << endl;
            exit(1);
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
                cerr << "Error: " << read_bytes << " bytes read in 1 chunk instead of " << chunk_size << " while reading " << entry.filename << endl;
                exit(1);
            }
            // Write the content
            unsigned short wrote_bytes = fwrite(buffer, 1, buffer_size, ifp);
            tbr += wrote_bytes;

            if (wrote_bytes != buffer_size) {
                cerr << "Error: " << wrote_bytes << " bytes written in 1 chunk instead of " << chunk_size << " while writing " << entry.filename << endl;
                exit(1);
            }
        }  
        if (ifp != nullptr) {
            fclose(ifp);
        }
        archive.push_back(entry);
    }
    fclose(farchive);
    return tbr;
}

void printArchiveContent(const mkarchive& archive) {
    unsigned maxfilenamew = archive[0].filename.size();
    unsigned maxfilesizew = to_string(archive[0].filesize).size();

    for (mkfile file : archive) {
        if (file.filename.size() > maxfilenamew) {
            maxfilenamew = file.filename.size();
        }
        if (to_string(file.filesize).size() > maxfilesizew) {
            maxfilesizew = to_string(file.filesize).size();
        }
    }

    maxfilenamew = (maxfilenamew > 30) ? 30 : maxfilenamew;
    maxfilenamew = (maxfilenamew < 9) ? 9 : maxfilenamew;
    maxfilesizew = (maxfilesizew < 4) ? 4 : maxfilesizew;

    cout << left << setw(maxfilenamew + 2) << "Filename" << right << setw(maxfilesizew + 2) << "Size" << endl;
    for (unsigned i = 0; i < maxfilenamew + maxfilesizew + 4; i++) {
        cout << "-";
    }
    cout << endl;
    
    for (mkfile file : archive) {
        cout << left << setw(maxfilenamew + 2) << file.filename << right << setw(maxfilesizew + 2) << file.filesize << endl;
    }
}