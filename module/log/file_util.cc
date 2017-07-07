#include "file_util.h"
#include <string.h> 
#include <assert.h>

namespace asyncnet
{
namespace log
{
	WritableFile::WritableFile(const std::string& fname)
	:filename_(fname)
	{
		file_ = fopen(fname.c_str(), "ae");
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
			fprintf(file_, "Writable::Append failed %s\n", strerror(errno));
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
