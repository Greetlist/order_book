import argh
import pandas as pd
import os
import random
import time

def mock_order(sid='', exchange_id='', finance_type='', start_price=1.00, end_price=0.00, record_number=100000000):
    assert start_price <= end_price, "start_price must <= end_price";
    uid = '{sid}-{exchange_id}-{finance_type}'.format(**locals())
    mock_data = list()
    cur_time = int(time.time())
    req_id = 1
    for i in range(record_number):
        item = {
            'Uid': uid,
            'RequestId': req_id,
            'Volume': random.randint(1, 20) * 100,
            'VendorTime': cur_time,
            'LocalTime': cur_time + 1,
            'Price': round(random.uniform(start_price, end_price), 2),
            'Side': 'Buy' if random.randint(1, 2) == 1 else 'Sell',
        }
        cur_time += 1
        req_id += 1
        mock_data.append(item)

    df = pd.DataFrame(mock_data)
    df.to_csv('{}_order.csv'.format(uid), index=False)

if __name__ == '__main__':
    argh.dispatch_commands([
        mock_order
    ])
