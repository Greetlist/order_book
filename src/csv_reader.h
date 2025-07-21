#ifndef CSV_READER_H_
#define CSV_READER_H_
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>

enum class Exchange {
  SH = 1,
  SZ,
  BJ,
  Unknown,
};

class CsvReader {
public:
  CsvReader(const std::string&, bool skip_header=false);
  ~CsvReader() = default;
  void ResetFileStream();
  std::vector<std::string> ReadColumnByIndex(int index);
  bool HasNextLine();
  std::string ReadLine();
private:
  bool skip_header_;
  std::string csv_path_;
  int line_index_ = 0;
  std::ifstream input_;
};

#endif
