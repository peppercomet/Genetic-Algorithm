
#ifndef _PEA_GA_01_LOADFROMFILE_H
#define _PEA_GA_01_LOADFROMFILE_H

#include <fstream>

class LoadFromFile {
public:
    std::string nameOfFileOpti;

    bool openFile();

    int getDataFromFile();

    ~LoadFromFile() {
        file.close();
    }
    bool openFileName(const std::string& nameOfFile);

    std::string getFileName();


private:
    bool gotDimension = false;
    bool gotMatrix = false;
    std::fstream file;

};

#endif //_PEA_GA_01_LOADFROMFILE_H
