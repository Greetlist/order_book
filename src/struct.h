#ifndef STRUCT_H_
#define STRUCT_H_

#include <list>
#include <string>

enum class OrderSide {
  Buy = 1,
  Sell = 2,
};

struct Order {
  Order();
  Order(std::string&& uid, uint64_t id, uint64_t volume, uint64_t vendor_time, uint64_t local_time, double price, OrderSide side) : uid(uid), order_id(id), volume(volume), vendor_time(vendor_time), local_time(local_time), price(price), side(side) {}
  ~Order() = default;
  std::string uid;
  uint64_t order_id;
  uint64_t volume;
  uint64_t vendor_time;
  uint64_t local_time;
  double price;
  OrderSide side;
};

struct Price {
  Price(double p) : price(p), total_volume(0) {}
  ~Price() = default;
  double price;
  uint64_t total_volume;
  std::list<Order*> orders;
};

#endif
