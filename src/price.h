#ifndef PRICE_H_
#define PRICE_H_

// Price struct is represented as a BST

class Price {
public:
  Price();
  ~Price();
private:
  double price_;
  int order_count_;
  uint64_t total_volume_;
  Price* left_;
  Price* right_;
  Price* parent_;
  Order* head_;
  Order* tail_;
};

#endif
