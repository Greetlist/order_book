#include "order_book.h"

OrderBook::OrderBook(std::initializer_list<std::string> info, double prev_close) : prev_close_(prev_close), last_price_(prev_close) {
  auto it = info.begin();
  uid_ = *(it);
  exchange_id_ = *(it+1);
  finance_type_ = *(it+2);
}

OrderBook::~OrderBook() {
}

bool OrderBook::Init() {
  InitRatio();
  int total_level = 2 * (std::ceil(prev_close_ * change_ratio_ / price_change_unit_)) + 10; //最后加的10为了防止小数计算时候的溢出
  buy_ = new std::vector<Price*>(total_level, nullptr);
  sell_ = new std::vector<Price*>(total_level, nullptr);
  double cur_price = std::floor(prev_close_ * (1 - change_ratio_) * 100) / 100;
  for (int i = 0; i < total_level; ++i) {
    buy_->at(i) = new Price(cur_price);
    sell_->at(i) = new Price(cur_price);
    if (ApproximatelyEqual(cur_price, prev_close_)) {
      mid_index_ = i;
    }
    cur_price += price_change_unit_;
  }
  buy_best_price_ = std::floor(prev_close_ * (1 - change_ratio_) * 100) / 100;
  sell_best_price_ = std::ceil(prev_close_ * (1 + change_ratio_) * 100) / 100;
  return true;
}

bool OrderBook::AddOrder(Order* order) {
  std::vector<Price*>* side = nullptr;
  if (order->side == OrderSide::Buy && order->price >= sell_best_price_) {
    MatchBuyOrder(order);
  } else if (order->side == OrderSide::Sell && order->price <= buy_best_price_) {
    MatchSellOrder(order);
  }

  if (order->volume <= 0) {
    return true;
  }

  side = order->side == OrderSide::Buy ? buy_ : sell_;
  int offset = GetIndex(order->price);
  Price* price = side->at(offset);
  price->orders.push_back(order);
  price->total_volume += order->volume;
  auto it = price->orders.end();
  it--;
  order_map_.insert({order->order_id, it});
  if (order->side == OrderSide::Buy) {
    buy_best_price_ = std::max(buy_best_price_, order->price);
  } else {
    sell_best_price_ = std::min(sell_best_price_, order->price);
  }
  return true;
}

void OrderBook::MatchBuyOrder(Order* order) {
  int start_index = GetIndex(sell_best_price_);
  int end_index = GetIndex(order->price);
  for (int index = start_index; index <= end_index; ++index) {
    //Price* price = sell_->at(index);
    Price* price = (*sell_)[index];
    if (price->total_volume <= 0) continue;
    auto begin = price->orders.begin();
    auto end = price->orders.end();
    for (; begin != end; begin++) {
      Order* cur_order = *begin;
      double trade_price = cur_order->price;
      if (cur_order->volume > order->volume) {
        order->volume = 0;
        cur_order->volume -= order->volume;
        price->total_volume -= order->volume;
      } else {
        order->volume -= cur_order->volume;
        begin = price->orders.erase(begin);
        price->total_volume -= cur_order->volume;
        order_map_.erase(cur_order->order_id);
      }
      last_price_ = trade_price;
      if (order->volume <= 0) return;
    }
  }
}

void OrderBook::MatchSellOrder(Order* order) {
  int start_index = GetIndex(order->price);
  int end_index = GetIndex(buy_best_price_);
  for (int index = end_index; index >= start_index; --index) {
    //Price* price = buy_->at(index);
    Price* price = (*buy_)[index];
    if (price->total_volume <= 0) continue;
    auto begin = price->orders.begin();
    auto end = price->orders.end();
    for (; begin != end; begin++) {
      Order* cur_order = *begin;
      double trade_price = cur_order->price;
      if (cur_order->volume > order->volume) {
        order->volume = 0;
        cur_order->volume -= order->volume;
        price->total_volume -= order->volume;
      } else {
        order->volume -= cur_order->volume;
        begin = price->orders.erase(begin);
        price->total_volume -= cur_order->volume;
        order_map_.erase(cur_order->order_id);
      }
      last_price_ = trade_price;
      if (order->volume <= 0) return;
    }
  }
}

bool OrderBook::ModifyOrder(uint64_t order_id) {
  auto it = order_map_.find(order_id);
  if (it == order_map_.end()) { [[unlikely]]
    LOG(ERROR) << "Cannot Find order_id: " << order_id << " in order map, market missing";
    return false;
  }
  return true;
}

bool OrderBook::DeleteOrder(uint64_t order_id) {
  auto it = order_map_.find(order_id);
  if (it == order_map_.end()) { [[unlikely]]
    LOG(ERROR) << "Cannot Find order_id: " << order_id << " in order map, market missing";
    return false;
  }
  return true;
}

double OrderBook::GetLowestSellPrice() {
  return sell_best_price_;
}

double OrderBook::GetHighestBuyPrice() {
  return buy_best_price_;
}

int OrderBook::GetTotalVolume(OrderSide side, double price) {
  int index = GetIndex(price);
  std::vector<Price*>* s = side == OrderSide::Buy ? buy_ : sell_;
  return s->at(index)->total_volume;
}

int OrderBook::GetTotalVolume(OrderSide side, int start_level, int end_level) {
  return 0;
}

int OrderBook::GetOrderCount(double price) {
  return 0;
}

int OrderBook::GetOrderCount(int start_level, int end_level) {
  return 0;
}

int OrderBook::GetIndex(double price) {
  return mid_index_ + (price - prev_close_) * 100;
}

void OrderBook::Print() {
  std::string spliter(100, '=');
  std::cout << "Buy" << std::endl;
  std::cout << spliter << std::endl;
  auto begin = buy_->begin();
  auto end = buy_->end();
  for (; begin != end; begin++) {
    Price* p = *begin;
    if (p->orders.size() > 0) {
      std::cout << p->price << ": ";
      for (Order* o : p->orders) {
        std::cout << "[" << o->volume << "] --> ";
      }
      std::cout << std::endl;
    }
  }
  std::cout << spliter << std::endl;

  std::cout << "Sell" << std::endl;
  std::cout << spliter << std::endl;
  begin = sell_->begin();
  end = sell_->end();
  for (; begin != end; ++begin) {
    Price* p = *begin;
    if (p->orders.size() > 0) {
      std::cout << p->price << ": ";
      for (Order* o : p->orders) {
        std::cout << "[" << o->volume << "] --> ";
      }
      std::cout << std::endl;
    }
  }
  std::cout << spliter << std::endl;
}

void OrderBook::InitRatio() {
  if (uid_.starts_with("300") || uid_.starts_with("688")) {
    change_ratio_ = 0.2;
  } else if (uid_.starts_with("920")) {
    change_ratio_ = 0.3;
  }
}

bool OrderBook::ApproximatelyEqual(double a, double b) {
  double max_num = std::max({1.0, std::fabs(a), std::fabs(b)});
  //LOG(INFO)
  //  << "[" << a << "] " << " [" << b << "] "
  //  << std::fabs(a - b) << ", " << max_num * std::numeric_limits<double>::epsilon()
  //  << ", " << (std::fabs(a - b) <= max_num * std::numeric_limits<double>::epsilon() * 100);
  return std::fabs(a - b) <= max_num * std::numeric_limits<double>::epsilon() * 100;
}
