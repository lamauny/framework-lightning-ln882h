from ntpath import join
import os
import sys
import time
import binascii
import struct
import math

APP_KEY_LEN = 0x1000

def add_key(flash_file_path , key_file_path , start_addr = 0x7000) -> bool:
    bin_len  = os.path.getsize(flash_file_path)
    key_len  = os.path.getsize(key_file_path)

    key_file = open(key_file_path, 'rb')
    bin_file = open(flash_file_path, 'rb')

    bin_val = bin_file.read()
    key_val = key_file.read()
    key_val = key_val + bytearray(APP_KEY_LEN - key_len)
    
    if bin_len < start_addr + APP_KEY_LEN:
        empty_val = bytearray(start_addr + APP_KEY_LEN - bin_len)
        for i in range(0,len(empty_val)):
            empty_val[i] = 0
        bin_val = bin_val + empty_val
        bin_len = start_addr + APP_KEY_LEN
    if key_len > APP_KEY_LEN:
        print("Error APP_KEY.bin is too large.")
        bin_file.close()
        key_file.close()
        return 0

    new_bin_val = bytearray(bin_val)

    index = 0
    for data in key_val:
        new_bin_val[start_addr + index] = data
        index = index + 1
    bin_file.close()
    key_file.close()

    bin_file = open(flash_file_path, 'wb+')
    bin_file.seek(0)
    bin_file.write(new_bin_val)
    bin_file.close()
    return 1


if __name__ == '__main__':
    # ret = add_key('0x200001.bin','0x120.bin')
    arg1 = sys.argv[1]  
    arg2 = sys.argv[2]  
    ret = add_key(arg1,arg2)
    if ret == 1 :
        now_time = time.localtime()
        time_string = "{yyyy}-{MM:02d}-{DD:02d} {hh:02d}:{mm:02d}:{ss:02d}".format(yyyy=now_time.tm_year,
        MM=now_time.tm_mon, DD=now_time.tm_mday,
        hh=now_time.tm_hour, mm=now_time.tm_min, ss=now_time.tm_sec)
        print("----------" * 7)
        print("APP KEY has been added, generating a new Flash bin. @ {t}".format(t=time_string))
        print("----------" * 7)
    else:
        print("APP KEY Add Error!!!")