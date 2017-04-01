#!/usr/bin/python
# coding=utf-8

import socket
import select
import struct
import time


MAX_RECEIVE_BUF = 64 * 1024  # 64k


class TcpConnection(object):
    """
    tcp连接、接收和发送
    """
    def __init__(self, ip, port):
        self.__ip = ip
        self.__port = port
        self.__sock = 0
        self.connect()
        pass

    def __del__(self):
        self.__sock.close()

    def connect(self):
        address = (self.__ip, self.__port)
        while True:
            try:
                self.__sock = socket.socket()
                self.__sock.connect(address)
            except socket.error as err:
                print ('Connect ip:%s:port:%d failed %s...Try reconnect\n' % (self.__ip, self.__port, err))
                time.sleep(2)
                continue
            break
        print ('Connect Server successfully !\n')
        return

    def select(self, time_out):
        """
        select mode
        """
        return select.select([self.__sock], [], [], time_out)

    def send(self, data):
        data_len = len(data)
        ret = 0
        while True:
            ret = self.__sock.send(data[ret:])
            if ret >= data_len:
                break
        return

    def recv_raw(self):
        """
        接收原始数据
        """
        all_data = ""
        while True:
            rec_data = self.__sock.recv(MAX_RECEIVE_BUF)
            if len(rec_data) < 0:
                self.__sock.close()
                print('receive error')
            elif len(rec_data) == 0:
                print('Server close connection')
                break
        return all_data

    def recv(self):
        """
        接收数据包 保证接收的是完整包
        :return 返回完整包数据
        """
        all_data = ""
        while True:
            rec_data = self.__sock.recv(MAX_RECEIVE_BUF)
            if len(rec_data) > 0:
                all_data += rec_data
                if len(all_data) > 4:
                    msg_len = struct.unpack('=I', all_data[:4])
                else:
                    continue
                if len(all_data) >= msg_len[0]:
                    break
            else:
                # 服务端主动关闭
                break
        return all_data

    def close(self):
        self.__sock.close()
