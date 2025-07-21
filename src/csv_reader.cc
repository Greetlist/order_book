#include "csv_reader.h"

CsvReader::CsvReader(const std::string& csv_path, bool skip_header) : skip_header_(skip_header), csv_path_(csv_path) {}

void CsvReader::ResetFileStream() {
  input_ = std::ifstream(csv_path_);
  //std::ifstream input(csv_path_);
  if (!input_.is_open()) {
    std::cout << "Open [" << csv_path_ << "] Error, Exit" << std::endl;
    std::exit(1);
  }
}

std::vector<std::string> CsvReader::ReadColumnByIndex(int index) {
  std::vector<std::string> res;

  int line_index = 0;
  while (true) {
    std::string line;
    std::getline(input_, line);
    std::stringstream line_stream(line);
    std::string cell;
    if (line_index++ == 0) continue;

    int cell_index = 0;
    std::string target;
    while (std::getline(line_stream, cell, ',')) {
      if (cell_index == index) {
        res.push_back(cell);
        break;
      }
      cell_index++;
    }
    if (input_.eof()) {
      break;
    }
  }
  return res;
}

bool CsvReader::HasNextLine() {
  return !input_.eof();
}

std::string CsvReader::ReadLine() {
  std::string line;
  std::getline(input_, line);
  return (skip_header_ && line_index_++ == 0) ? std::string{""} : line;
}
