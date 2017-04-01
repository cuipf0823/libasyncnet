#!/usr/bin/python
# coding=utf-8

import sys
import threading
import tcp_con as connect

#  客户端退出标志
g_quit = [False]


def welcome():
    print('*' * 60 + '\n')
    print(' ' * 25 + 'Client\n')
    print('*' * 60 + '\n')


def std_loop(connection):
    while True:
        data = sys.stdin.readline().strip('\n').lower().lstrip().rstrip()
        if data:
            connection.send(data)
        else:
            print 'Invalid command! \n'


def net_loop(connection):
    while not g_quit[0]:
        rs, _, _ = connection.select(1)
        for s in rs:
            # 如果返回data为空，表示服务端断开了连接
            data = connection.recv()
            print data
            if data:
                print data
            else:
                print('Server close connection Try reconnect...\n')
                connection.connect()
    connection.close()


def main():
    welcome()
    connection = connect.TcpConnection('192.168.208.129', 8888)
    thread1 = threading.Thread(target=net_loop, args=(connection, ))
    thread2 = threading.Thread(target=std_loop, args=(connection, ))
    thread1.start()
    thread2.start()
    pass


if __name__ == '__main__':
    main()
    # os.system('pause')

