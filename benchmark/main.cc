#include <benchmark/benchmark.h>
#include <algorithm>
#include <string>
#include <sstream>
#include <unordered_map>

#include "order_book.h"
#include "csv_reader.h"

static void test_order_book(benchmark::State& state) {
  OrderBook *order_book = new OrderBook({"601225", "SH", "stock"}, 12.85);
  order_book->Init();
  CsvReader reader("/home/greetlist/github_project/order_book/mock/601225-SH-stock_order.csv", true);
  reader.ResetFileStream();
  for (auto _ : state) {
    while (reader.HasNextLine()) {
      std::string line = reader.ReadLine();
      std::cout << line << std::endl;
      if (line.size() == 0) continue;
      std::stringstream line_stream(line);
      std::string cell;

      std::string target;
      std::string uid;
      uint64_t req_id = 0;
      uint64_t volume = 0;
      uint64_t vendor_time = 0, local_time = 0;
      double price = 0.00;
      OrderSide side = OrderSide::Buy;

      int cell_index = 0;
      while (std::getline(line_stream, cell, ',')) {
        switch (cell_index) {
        case 0:
          uid = cell;
          break;
        case 1:
          req_id = std::stoull(cell);
          break;
        case 2:
          volume = std::stoull(cell);
          break;
        case 3:
          vendor_time = std::stoull(cell);
          break;
        case 4:
          local_time = std::stoull(cell);
          break;
        case 5:
          price = std::stod(cell);
          break;
        case 6:
          side = cell == "Buy" ? OrderSide::Buy : OrderSide::Sell;
          break;
        }
        cell_index++;
      }
      Order* order = new Order(std::move(uid), req_id, volume, vendor_time, local_time, price, side);
      order_book->AddOrder(order);
    }
  }
}
//BENCHMARK(map_find)->Unit(benchmark::kMillisecond);
BENCHMARK(test_order_book);

BENCHMARK_MAIN();
