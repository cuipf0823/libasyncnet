#include "file_util.h"
#include <stdio.h>
namespace asyncnet
{
namespace log
{
    WritableFile::WritableFile(const std::string& fname)
    :filename_(fname)
    {
        file_ = fopen(fname.c_str(), "a");
        assert(file_ != nullptr);
    }

    WritableFile::~WritableFile()
    {
        if (file_ != nullptr)
        {
            fclose(file_);
        }
    }

    bool WritableFile::Append(const char* buffer, size_t length)
    {
        size_t ret = fwrite_unlocked(buffer, 1, length, file_);
        if(ret != length)
        {
            return false;
        }
        return true;
    }

    void WritableFile::Close()
    {
        fclose(file_);
        file_ = nullptr;
    }

    void WritableFile::Flush()
    {
        fflush(file_);
    }

}
}
