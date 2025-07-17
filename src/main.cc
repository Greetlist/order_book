#include <iostream>

#include "order_book.h"

static int req_id = 0;

int main(int argc, char** argv) {
  OrderBook *order_book = new OrderBook({"688001", "SH", "stock"}, 12.85);
  order_book->Init();
  for (int i = 0; i < 100; ++i) {
    Order* order = new Order("688001-SH-stock", req_id++, 100, 123123, 123123, 12.30, OrderSide::Buy);
    order_book->AddOrder(order);
  }
  for (int i = 0; i < 99; ++i) {
    Order* order = new Order("688001-SH-stock", req_id++, 100, 123123, 123123, 12.30, OrderSide::Sell);
    order_book->AddOrder(order);
  }
  order_book->Print();
  return 0;
}
