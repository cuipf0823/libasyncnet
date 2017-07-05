#ifndef ASYNCNET_LOG_FILE_SINKS_H
#define ASYNCNET_LOG_FILE_SINKS_H
##include <string>

namespace asyncnet
{
namespace log
{

class Sinks
{
public:
  Sinks();
  virtual ~Sinks();
  Sinks& operator=(const Sinks&) = delete;
  Sinks(const Sinks&) = delete;

  virtual void append(const std::string& buffer) = 0;
  virtual void append(const char* buffer, int len) = 0;
  virtual void flush() = 0;
}

class FileSinks : public Sinks
{
public:
  FileSinks(std::string& path, size_t roll_size);
  ~FileSinks();
  FileSinks& operator=(const Sinks&) = delete;
  FileSinks(const Sinks&) = delete;

  virtual void append(const std::string& buffer);
  virtual void append(const char* buffer, int len);
  virtual void flush();
private:
  const std::string path_;
  const size_t roll_size_;
};

}
}
#endif
