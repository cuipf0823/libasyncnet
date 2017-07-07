#ifndef ASYNCNET_FILE_UTIL_H
#define ASYNCNET_FILE_UTIL_H
#include <string>
#include <stdio.h>

namespace asyncnet
{
namespace log
{

class WritableFile
{
public:
    WritableFile(const std::string& fname);
    ~WritableFile();
    WritableFile(const WritableFile&) = delete;
    WritableFile& operator==(const WritableFile&) = delete;

    bool Append(const char* buffer, size_t length);
    void Close();
    void Flush();
private:
    std::string filename_;
    FILE* file_;

};

}
}
#endif
