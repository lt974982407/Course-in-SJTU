import psycopg2
import socket
import threading
import time
import sounddevice as sd
from numpy import *


class server(object):
    def __init__(self, addr, port):
        self.addr = addr
        self.port = port
        self.connection = []
        self.userlist = []
        self.add_user = {}
        self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.conn = psycopg2.connect(database="chatroom", user="postgres", password="litie2000", host="127.0.0.1",port="5432")#连接数据库
        self.cursor = self.conn.cursor()
        self.cursor.execute("UPDATE chatroom set state = false")
        self.conn.commit()
        self.s.bind((self.addr,self.port))

    def plisten(self):
        """
        进入侦听模式 处理来自客户端的连接
        :return:
        """
        self.s.listen(128)
        while True:
            sock, address = self.s.accept()
            self.connection.append(sock)
            fun = threading.Thread(target=self.receive,args=(sock,)) #多线程，为每一个连接的客户端分配线程
            fun.start()

    def receive(self, t):
        """
        接受来自客户端的消息 调用消息处理函数
        :param t:
        :return:
        """
        while True:
            message = eval(t.recv(111111024))
            print(message)
            if message["type"] == "login":
                self.login(message,t.getpeername())
            elif message["type"] == "signup":
                self.signup(message,t.getpeername())
            elif message["type"] == "msg":
                self.dealmsg(message)

    def login(self, msg, addresst):
        """
        登陆消息处理函数
        :param msg:
        :param addresst:
        :return:
        """
        username = msg ["name"]
        self.cursor.execute("SELECT * FROM chatroom WHERE username = '"+username+"'") #查找要登录的用户名
        row = self.cursor.fetchall()
        print(row)

        if(row ==[]): #用户名不存在
            back = {"type": "loginBack",
                    "info": "username does not exist"}
        if(row != []):
            if(row[0][1].rstrip()!=msg["password"]): #密码错误
                back = {"type": "loginBack",
                        "info": "password error"}
            elif( row[0][2] == True): #已经登录
                back = {"type": "loginBack",
                        "info": "login already"}
            else: #登陆成功
                self.cursor.execute("UPDATE chatroom set state = true where username= '"+username+"'")
                self.conn.commit()
                self.cursor.execute("SELECT username  FROM chatroom ")
                row = self.cursor.fetchall()
                print(row)
                self.userlist.clear()
                for item in row :
                    self.userlist.append(item[0].rstrip())

                back = {"type": "loginBack",
                        "info": "loginSucc",
                        "userlist": self.userlist}
        for c in self.connection: #发送反馈消息
            c_addr = c.getpeername()
            print(c_addr)
            print(addresst)
            if addresst == c_addr :
                self.add_user[username] = addresst
                c.send(str(back).encode())
                print(str(back).encode())
                print(c)

    def signup(self,msg, addresst):
        """
        注册消息处理函数
        :param msg:
        :param addresst:
        :return:
        """
        username = msg["name"]
        self.cursor.execute("SELECT * FROM chatroom WHERE username = '"+username+"'") #查找用户名
        row = self.cursor.fetchall()
        if row ==[]: #注册成功
            strsql = "INSERT INTO chatroom values('"
            strsql+= username +"','"+msg["password"]+"',false)"
            self.cursor.execute(strsql)
            self.conn.commit()

            back = {"type": "rgtrBack",
                    "info": "rgtrSucc"}
        else : #用户名已经存在
            back = {"type": "rgtrBack",
                    "info": "rgtrFail"}
        for c in self.connection:
            c_addr = c.getpeername()
            if addresst == c_addr :
                c.send(str(back).encode())
                print(str(back))

    def dealmsg(self, msg):
        """
        处理消息的函数
        :param msg:
        :return:
        """
        dname = msg["dname"]
        sname = msg ["sname"]
        self.cursor.execute("SELECT * FROM chatroom WHERE username = '" + dname + "'")
        row = self.cursor.fetchall()
        if (msg["dname"] != "group"):#不是群组消息
            if row[0][2] == False:
                for item in self.connection :
                    if(item.getpeername() == self.add_user[sname]): #用户不在线 发送失败
                        item.send(str({"type": "msg",
                            "mtype": "msgerr",
                            "dname": sname,
                            "sname": "server",
                            "time": time.time(),
                            "msg": "not online"}).encode())
            elif(msg["mtype"] == "msg"): #文字消息处理
                for item in self.connection :
                    if (item.getpeername() == self.add_user[dname]):
                        item.send(str({"type": "msg",
                                       "mtype": "msg",
                                       "dname": dname,
                                       "sname": sname,
                                       "ssname":sname,
                                       "time": time.time(),
                                       "msg": msg["msg"]}).encode())
                    if (item.getpeername() == self.add_user[sname]):
                        item.send(str({"type": "msg",
                                       "mtype": "msg",
                                       "dname": sname,
                                       "sname": dname,
                                       "ssname": sname,
                                       "time": time.time(),
                                       "msg": msg["msg"]}).encode())
            elif (msg["mtype"] == "emoji"): #表情消息处理
                for item in self.connection:
                    if (item.getpeername() == self.add_user[dname]):
                        item.send(str({"type": "msg",
                                       "mtype": "emoji",
                                       "dname": dname,
                                       "sname": sname,
                                       "ssname": sname,
                                       "time": time.time(),
                                       "msg": msg["msg"]}).encode())
                    if (item.getpeername() == self.add_user[sname]):
                        item.send(str({"type": "msg",
                                       "mtype": "emoji",
                                       "dname": sname,
                                       "sname": dname,
                                       "ssname": sname,
                                       "time": time.time(),
                                       "msg": msg["msg"]}).encode())
            elif (msg["mtype"] == "file"): #文件消息处理
                for item in self.connection:
                    if (item.getpeername() == self.add_user[dname]):
                        filehin = sname+" has sent "+msg["fname"]+" to you."
                        item.send(str({"type": "msg",
                                       "mtype": "file",
                                       "dname": dname,
                                       "sname": sname,
                                       "ssname": sname,
                                       "time": time.time(),
                                       "msg": filehin,
                                       "fname":msg["fname"],
                                       "file":msg["msg"]}).encode())
                    if (item.getpeername() == self.add_user[sname]):
                        filehin = "you" + " has sent " + msg["fname"]+" to "+dname+". "
                        item.send(str({"type": "msg",
                                       "mtype": "file",
                                       "dname": sname,
                                       "sname": dname,
                                       "ssname": sname,
                                       "time": time.time(),
                                       "msg": filehin,
                                       "fname": msg["fname"],
                                       "file":msg["msg"]}).encode())
            elif (msg["mtype"] == "voice"): #语音消息处理
                for item in self.connection:
                    if (item.getpeername() == self.add_user[dname]):
                        vhin = sname+" has sent a voice message to you."
                        item.send(str({"type": "msg",
                                       "mtype": "voice",
                                       "dname": dname,
                                       "sname": sname,
                                       "ssname": sname,
                                       "time": time.time(),
                                       "msg": vhin,
                                       "len": msg["len"],
                                       "voice":msg["msg"]}).encode())
                    if (item.getpeername() == self.add_user[sname]):
                        vhin = "you" + " has sent a voice message to "+dname+". "
                        item.send(str({"type": "msg",
                                       "mtype": "voice",
                                       "dname": sname,
                                       "sname": dname,
                                       "ssname": sname,
                                       "time": time.time(),
                                       "msg": vhin,
                                       "len":msg["len"],
                                       "voice":msg["msg"]}).encode())
        else : #群组消息处理 逻辑同上
            if (msg["mtype"] == "msg"):
                for item in self.connection:
                    item.send(str({"type": "msg",
                                   "mtype": "msg",
                                   "dname": sname,
                                   "sname": "group",
                                   "ssname": sname,
                                   "time": time.time(),
                                   "msg": msg["msg"]}).encode())
            elif (msg["mtype"] == "emoji"):
                for item in self.connection:
                    item.send(str({"type": "msg",
                                   "mtype": "emoji",
                                   "dname": sname,
                                   "sname":"group",
                                   "ssname": sname,
                                   "time": time.time(),
                                   "msg": msg["msg"]}).encode())
            elif (msg["mtype"] == "file"):
                for item in self.connection:
                    filehin = sname + " has sent " + msg["fname"] + " to the group."
                    item.send(str({"type": "msg",
                                   "mtype": "file",
                                   "dname": sname,
                                   "sname": "group",
                                   "ssname": sname,
                                   "time": time.time(),
                                   "msg": filehin,
                                   "fname": msg["fname"],
                                   "file": msg["msg"]}).encode())
            elif (msg["mtype"] == "voice"):
                for item in self.connection:
                    vhin = sname + " has sent a voice message to the group."
                    item.send(str({"type": "msg",
                                   "mtype": "voice",
                                   "dname": sname,
                                   "sname": "group",
                                   "ssname": sname,
                                   "time": time.time(),
                                   "msg": vhin,
                                   "len": msg["len"],
                                   "voice": msg["msg"]}).encode())

    def run(self):
        fun1 = threading.Thread(target=self.plisten) #多线程，为侦听的套接字分配线程
        fun1.start()

    def __del__(self):
        self.conn.close()
        for c in self.connection:
            c.close()
        self.s.close()


if __name__ == '__main__':
    se = server(addr = "127.0.0.1",port = 7879)
    se.run()