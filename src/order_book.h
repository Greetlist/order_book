#ifndef ORDER_BOOK_H_
#define ORDER_BOOK_H_

#include <string>
#include <map>

#include "struct.h"
#include "order.h"
#include "price.h"

class OrderBook {
public:
  OrderBook() = default;
  ~OrderBook() = default;
  bool Init();
  bool AddOrder(Order* order);
  int GetLowestSellPrice();
  int GetHighestBuyPrice();
  int GetTotalVolume(double price);
  int GetTotalVolume(int start_level, int end_level);
  int GetOrderCount(double price);
  int GetOrderCount(int start_level, int end_level);
  int CancelOrder(uint64_t order_id);
private:
  std::string uid_;
  std::string exchange_id_;
  std::string finance_type_;
  double last_price_;
  Price<OrderSide::Buy>* buy_;
  Price<OrderSide::Sell>* sell_;
  std::map<uint64_t, Order*> order_map_;
};

#endif
