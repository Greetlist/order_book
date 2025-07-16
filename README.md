## 讨论

### 关于选取的数据结构
在阅读一些资料之后，需要分两种情况来讨论:

1. 无限深度的order book
更倾向于用map数据结构来存价格，在价格下面用双向链表来存order

*为了更方便地找到某个order，需要用order的unique id来作为key，value为指向order的指针或者iterator(`std::list`的iterator是稳定的)*
*数据结构选`std::unordered_map`, 不用考虑key是uint64还是string*

- Add/Find: 添加一个新的价格、寻找一个特定的价格时间复杂度: O(logM), M是已经有的价格档位, 找到对应价格的链表之后，插入直接是在尾部，所以是O(1)

- Modify: O(1), 常数级别, 根据`order_id`找到对应的单子直接修改即可

- Delete: O(1), 常数级别, 对应撤单，撤单肯定是对应到某一个特定的单子上，直接把这个`iterator`删掉即可

2. 有限深度的order book
更倾向与用`sorted vector`来存价格，在价格下面还是用双向链表来存order,

使用sorted vector那就要面临每一次插入的排序或者直接分配好价格空间, 需要权衡跟取舍

- Add/Find: 
添加一个新的价格: 如果是新价格的话，那么需要O(logM) + O(M), M是已经有的价格档位, 如果是存在的价格，那就是O(logM)

寻找一个特定的价格时间复杂度: O(logM)二分, M是已经有的价格档位

- Modify: O(logM), 因为 `vector` 插入/删除之后，迭代器可能会失效, 所以需要重新找, 根据`order_id`找到对应的单子直接修改即可

- Delete: O(logM) + O(M), 需要重新找价格，再找对应的order

### 一些想法
1. 按照现在最小价格变动单位为0.01元的情况来看, 涨跌幅10%来算, [300/688 20%, 北交所30%]

10元股, 有(10*0.1*100) = 100个价格档位(Ask or Bid), 按照价格最高的茅台:(2628*0.1*100) = 26280个价格档位(Ask or Bid)

2. 经过benchmark，可以发现，档位在400左右的时候，无论是`std::find`、`std::binary_search`、`map.find`所需要的时间都是差不多的，所以400应该是一个关键的分水岭
如果是更大的，无限深度的档位的话，`std::find`这种O(N)的算法就开始逐渐疲软

3. 很多算法应该不会用到无限深度的order book, 所以用vector应该就可以实现，即使需要无限深度，在vector保持有序的情况下，效率也并不比map差

### Optiver 自动化交易leader的分享
[When Nanoseconds Matter: Ultrafast Trading Systems in C++ by David Gross](https://github.com/CppCon/CppCon2024)
从他的实现里面来看，也是选取了`sorted vector`来存价格，并且在预先分配好内存的情况下，并不会存在`vector`的数据移动跟迭代器失效这种问题

### RT的实现
使用了`btree_map + vector(存价格)`的组合，暂时没想明白为什么选这样的数据结构，可以实现出来对比一下

### 选型
1. `map + list` (easy impl)

2. `sorted_vector + list` (efficiency prefered)

3. `btree_map + vector`

### 盘口的变化如何实现



## Reference
1. [HFT-OrderBook](https://github.com/Crypto-toolbox/HFT-Orderbook)

2. [LOB](https://web.archive.org/web/20110219163448/http://howtohft.wordpress.com/2011/02/15/how-to-build-a-fast-limit-order-book/)
