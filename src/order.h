#ifndef ORDER_H_
#define ORDER_H_

// Order struct is represented as a double linked list

class Order {
public:
  Order();
  ~Order();
private:
  uint64_t order_id_;
  OrderSide side_;
  uint64_t volume_;
  int order_time_;
  int entry_time_;
  Order* next_;
  Order* prev_;
  Price* belonged_price_;
};

#endif
