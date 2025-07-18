#ifndef ORDER_BOOK_H_
#define ORDER_BOOK_H_

#include <string>
#include <cmath>
#include <map>
#include <list>
#include <vector>
#include <unordered_map>
#include <initializer_list>
#include <iostream>
#include <limits>
#include <glog/logging.h>

#include "struct.h"

class OrderBook {
public:
  OrderBook(std::initializer_list<std::string> info, double prev_close);
  ~OrderBook();
  bool Init();
  bool AddOrder(Order* order);
  void MatchOrder(std::vector<Price*>* side, Order* order);
  bool ModifyOrder(uint64_t order_id);
  bool DeleteOrder(uint64_t order_id);
  double GetLowestSellPrice();
  double GetHighestBuyPrice();
  int GetTotalVolume(OrderSide side, double price);
  int GetTotalVolume(OrderSide side, int start_level, int end_level);
  int GetOrderCount(double price);
  int GetOrderCount(int start_level, int end_level);
  void Print();
private:
  std::string uid_;
  std::string exchange_id_;
  std::string finance_type_;
  double change_ratio_ = 0.1; //涨跌停限制
  double price_change_unit_ = 0.01; //最小价格变动单位
  double prev_close_ = 0.00; //昨收
  double last_price_ = 0.00; //最新成交价
  double buy_best_price_ = 0.00; //买方最优
  double sell_best_price_ = 0.00; //卖方最优
  std::vector<Price*>* buy_ = nullptr;
  std::vector<Price*>* sell_ = nullptr;
  std::unordered_map<uint64_t, std::list<Order*>::iterator> order_map_;
  int mid_index_;
  int GetIndex(double price);
  void InitRatio();
  bool ApproximatelyEqual(double a, double b);
};

#endif
