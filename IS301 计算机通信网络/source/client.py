
from PyQt5 import QtCore, QtGui, QtWidgets
import sys
import  os
import socket
import queue
import threading
import time
import base64
import sounddevice as sd
import numpy


class connect(object):
    def __init__(self):
        self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)#创建套接字
        self.s.connect(("127.0.0.1", 7879))
        self.logflag = False  #登录状态
        self.signflag = False #注册状态
        self.senderror = False #发送状态
        self.recmsg = [] #存储消息的列表
        self.recfile = [] #存储文件的列表
        self.recvoice = [] #存储语音的列表

    def receive(self):
        while True:
            message = eval(self.s.recv(111111024))#接收消息
            print(message)
            #消息的处理
            if message["type"] == "loginBack":  #接收到登录的反馈
                self.loginstas = message
                if message["info"] == "loginSucc":
                    self.userlist = message["userlist"]
                self.logflag = True
            if message["type"] == "rgtrBack":   #接收到注册的反馈
                print("got signback message")
                self.signstas = message
                self.signflag = True
            elif (message["type"] == "msg" and message["mtype"] == "msgerr"):   #消息发送失败
                self.senderror = True
            elif (message["type"] == "msg" and message["mtype"] != "msgerr"):   #消息发送成功的反馈
                temp = []
                temp.append(message["sname"])
                temp.append(message["ssname"])
                arrtime = time.localtime(int(message["time"]))
                rtime = time.strftime("%Y-%m-%d %H:%M:%S", arrtime)
                temp.append(rtime)
                temp.append(message["msg"])
                self.recmsg.append (temp) #存储消息
                if login.pr.chatuser == message["sname"]: #更新对话框
                    login.pr.grprecvText.append(message["ssname"]+" "+rtime+" :")
                    if (str(type(message["msg"])) == "<class 'str'>"):
                        login.pr.grprecvText.append(message["msg"])
                    elif (str(type(message["msg"])) == "<class 'int'>"):
                        img = QtGui.QTextImageFormat()
                        login.pr.grprecvText.insertPlainText("\n")
                        path = "./img/" + str(message["msg"]) + ".png"
                        tcursor = login.pr.grprecvText.textCursor()
                        img = QtGui.QTextImageFormat()
                        img.setName(path)
                        img.setHeight(30)
                        img.setWidth(30)
                        tcursor.insertImage(img)
                if (message["type"] == "msg" and message["mtype"] == "file"): #存储文件
                    ftemp = []
                    ftemp.append(message["sname"])
                    ftemp.append(message["ssname"])
                    ftemp.append(rtime)
                    ftemp.append(message["file"])
                    ftemp.append(message["fname"])
                    self.recfile.append(ftemp)
                if (message["type"] == "msg" and message["mtype"] == "voice"): #存储语音
                    vtemp = []
                    vtemp.append(message["sname"])
                    vtemp.append(message["ssname"])
                    lt = rtime+' '+message["len"]+"s"
                    vtemp.append(lt)
                    vtemp.append(message["voice"])
                    self.recvoice.append(vtemp)
            elif (message["type"] == "msg" and message["mtype"] == "msg"):
                pass

    def run(self):
        fun = threading.Thread(target=self.receive) #打开接收并处理消息的线程
        fun.start()


class loginwindow(QtWidgets.QDialog):
    def __init__(self):
        super(loginwindow, self).__init__()
        self.q = queue.Queue()
        self.setupUi()

    def setupUi(self):
        self.setObjectName("login")
        self.resize(430, 300)
        self.setStyleSheet("#login{border-image:url(./img/login.png);}")

        self.loginButton = QtWidgets.QPushButton(self) #登录按钮
        self.loginButton.setGeometry(QtCore.QRect(80, 180+50, 93, 28))
        self.loginButton.setObjectName("loginButton")
        self.loginButton.setStyleSheet('border:2px groove gray;border-radius:10px;padding:2px 4px;')
        self.loginButton.clicked.connect(self.loginButtonClick)

        self.signupButton = QtWidgets.QPushButton(self) #注册按钮
        self.signupButton.setGeometry(QtCore.QRect(230, 180+50, 93, 28))
        self.signupButton.setObjectName("signupButton")
        self.signupButton.setStyleSheet('border:2px groove gray;border-radius:10px;padding:2px 4px;')
        self.signupButton.clicked.connect(self.signupButtonClick)

        self.usernamehint = QtWidgets.QLabel(self) #用户名的提示
        self.usernamehint.setGeometry(QtCore.QRect(80+20, 80+60, 72, 16))
        self.usernamehint.setObjectName("label")

        self.pwhint = QtWidgets.QLabel(self) #密码提示
        self.pwhint.setGeometry(QtCore.QRect(80+20, 120+60, 72, 21))
        self.pwhint.setObjectName("label_2")

        self.usernameinput = QtWidgets.QLineEdit(self) #用户名输入框
        self.usernameinput.setGeometry(QtCore.QRect(180+20, 80+60, 113, 21))
        self.usernameinput.setObjectName("inputbox")

        self.pwinput = QtWidgets.QLineEdit(self) #密码输入框
        self.pwinput.setGeometry(QtCore.QRect(180+20, 120+60, 113, 21))
        self.pwinput.setObjectName("inputbox2")
        self.pwinput.setEchoMode(self.pwinput.Password)


        self.retranslateUi() #设置窗口名称函数
        QtCore.QMetaObject.connectSlotsByName(self)

    def retranslateUi(self):
        _translate = QtCore.QCoreApplication.translate
        self.setWindowTitle(_translate("login", "login"))
        self.loginButton.setText(_translate("login", "Login"))
        self.signupButton.setText(_translate("login", "Signup"))
        self.usernamehint.setText(_translate("login", "Username："))
        self.pwhint.setText(_translate("login", "Password："))

    def loginButtonClick(self):
        '''
        点击登录按钮触发事件
        '''
        username = self.usernameinput.text()
        pw = self.pwinput.text()
        if len(username) == 0 or len(pw) == 0:
            self.loginhint = loginhint ()
            self.loginhint.loginError.setText("Please iuput your username and password !")
            self.loginhint.show()
        else:
            con.s.send(str({"type": "login",
                                "name": username,
                                "password": pw,}
                                ).encode())

            while True :
                if  con.logflag :
                    break
            #根据各种情况进行处理
            if (con.loginstas["info"] == "loginSucc"):
                self.pr = chatWindow(username, con.userlist)
                self.pr.show()
                self.close()
            elif (con.loginstas["info"] == "username does not exist"):
                self.loginhint = loginhint()
                self.loginhint.loginError.setText("username does not exist")
                self.loginhint.show()
            elif (con.loginstas["info"] == "password error"):
                self.loginhint = loginhint()
                self.loginhint.loginError.setText("password error")
                self.loginhint.show()
            elif (con.loginstas["info"] == "login already"):
                self.loginhint = loginhint()
                self.loginhint.loginError.setText("login already")
                self.loginhint.show()
            con.logflag = False

    def signupButtonClick(self):
        self.signup = signup()
        self.signup.show()

class loginhint(QtWidgets.QWidget): #提示的类
    def __init__(self):
        super(loginhint, self).__init__()
        self.loginhintwin()

    def loginhintwin (self):
        self.setObjectName("loginhint")
        self.resize(375, 150)

        self.setWindowTitle("Error")
        self.loginError = QtWidgets.QLineEdit(self)         #登录信息提示框
        self.loginError.setGeometry(QtCore.QRect(0, 60, 375, 28))
        self.loginError.setStyleSheet("border:none;")
        self.loginError.setAlignment(QtCore.Qt.AlignCenter)
        self.loginError.setReadOnly(True)

class signup(QtWidgets.QDialog):
    def __init__(self):
        super(signup, self).__init__()
        self.signupwin()

    def signupwin(self):
        self.setObjectName("signup")
        self.resize(400, 300)
        self.setStyleSheet("#signup{border-image:url(./img/register.png);}")

        self.usernamehint = QtWidgets.QLabel(self) #用户名提示信息
        self.usernamehint.setGeometry(QtCore.QRect(80, 80, 72, 16))
        self.usernamehint.setObjectName("label")

        self.pwhint1 = QtWidgets.QLabel(self) #密码提示信息
        self.pwhint1.setGeometry(QtCore.QRect(80, 120, 72, 21))
        self.pwhint1.setObjectName("label_2")

        self.pwhint2 = QtWidgets.QLabel(self) #再次输入密码提示信息
        self.pwhint2.setGeometry(QtCore.QRect(80, 160, 72, 21))
        self.pwhint2.setObjectName("label_3")

        self.usernameinput = QtWidgets.QLineEdit(self) #用户名输入框
        self.usernameinput.setGeometry(QtCore.QRect(180, 80, 113, 21))
        self.usernameinput.setObjectName("inputbox")

        self.pwinput1 = QtWidgets.QLineEdit(self) #密码输入框
        self.pwinput1.setGeometry(QtCore.QRect(180, 120, 113, 21))
        self.pwinput1.setObjectName("inputbox2")
        self.pwinput1.setEchoMode(self.pwinput1.Password)

        self.pwinput2 = QtWidgets.QLineEdit(self) #密码确认输入框
        self.pwinput2.setGeometry(QtCore.QRect(180, 160, 113, 21))
        self.pwinput2.setObjectName("inputbox3")
        self.pwinput2.setEchoMode(self.pwinput2.Password)

        self.signupButton = QtWidgets.QPushButton(self) #注册按钮
        self.signupButton.setGeometry(QtCore.QRect(150, 200, 93, 28))
        self.signupButton.setObjectName("signupButton")
        self.signupButton.setStyleSheet('border:2px groove gray;border-radius:10px;padding:2px 4px;')
        self.signupButton.clicked.connect(self.signup)

        self.retranslateUi()
        QtCore.QMetaObject.connectSlotsByName(self)

    def signup(self):
        """
        注册触发函数
        """
        username = self.usernameinput.text()
        pw1 = self.pwinput1.text()
        pw2 = self.pwinput2.text()
        #进行本地的判断
        if pw1 != pw2 :
            self.loginhint = loginhint()
            self.loginhint.loginError.setText("Please input same passwords !")
            self.loginhint.show()
        elif len(username) == 0 or len(pw1) == 0 or len(pw2)==0 :
            self.loginhint = loginhint()
            self.loginhint.loginError.setText("Username or password cannot be null !")
            self.loginhint.show()
        else :
            con.s.send(str({"type": "signup",
                                "name": username,
                                "password": pw1,}).encode())
            print("send sucessfully!")
            print(con.signflag)
            while True :
                if  con.signflag :
                    break
            print("sign got message")
            print(con.signstas["info"])
            if con.signstas["info"] == "rgtrSucc":
                self.r = loginhint()
                self.r.loginError.setText("success !")
                self.r.show()
            elif con.signstas["info"] == "rgtrFail":
                self.r = loginhint()
                self.r.loginError.setText("username exist!")
                self.r.show()
            con.signflag = False

    def retranslateUi(self):
        _translate = QtCore.QCoreApplication.translate
        self.setWindowTitle(_translate("signup", "Singup"))
        self.usernamehint.setText(_translate("signup", "Username："))
        self.pwhint1.setText(_translate("signup", "Password："))
        self.pwhint2.setText(_translate("signup", "Password："))
        self.signupButton.setText(_translate("signup", "Singup"))


class chatWindow(QtWidgets.QDialog):
    def __init__(self, name, l):
        self.Username = name
        self.friendlist = l
        self.chatuser = name
        self.friendlist.append("group")
        self.friendlist.remove(self.Username)
        super(chatWindow, self).__init__()
        self.setupUi()
        try:
            os.mkdir(self.Username)  # 创建对应的文件夹
        except FileExistsError:
            pass

    def setupUi(self):

        self.setObjectName("ChatRoom")
        self.setStyleSheet("#ChatRoom{border-image:url(./img/Mychat2.png);}")
        self.resize(1005, 463)

        self.grprecvText = QtWidgets.QTextEdit(self)        #消息框
        self.grprecvText.setGeometry(QtCore.QRect(220, 30, 780, 270))
        self.grprecvText.setObjectName("textRecv")
        self.grprecvText.setAlignment(QtCore.Qt.AlignTop)
        self.grprecvText.setReadOnly(True)
        self.grprecvText.hide()

        self.user = QtWidgets.QLabel(self)
        self.user.setGeometry(QtCore.QRect(230, 0, 50, 25))


        self.sendText = QtWidgets.QTextEdit(self)           #发送消息的编辑框
        self.sendText.setGeometry(QtCore.QRect(220, 335, 780, 85)) #
        self.sendText.setObjectName("textSend")
        self.sendText.setAlignment(QtCore.Qt.AlignTop)
        self.sendText.hide()
        # self.sendText.keyPressEvent()
        self.destsend = 'all'

        self.sendtxtButton = QtWidgets.QPushButton(self)    #发送消息的按钮
        self.sendtxtButton.setGeometry(QtCore.QRect(765, 425, 65, 27))
        self.sendtxtButton.setObjectName("txtsendButton")
        self.sendtxtButton.setStyleSheet('border:2px groove gray;border-radius:10px;padding:2px 4px;')
        self.sendtxtButton.clicked.connect(self.sendclicked)
        self.sendtxtButton.hide()


        num = 0
        self.scwidget = QtWidgets.QWidget()
        self.scwidget.setGeometry(QtCore.QRect(5, 25, 210, len(self.friendlist)*62+5))
        self.scwidget.setMinimumSize(210,0)
        self.scwidget.setObjectName("hhh")
        self.scwidget.setStyleSheet("#hhh{border-image:url(./img/Mychat3.png);}")
        self.listButton = []
        #根据从服务器获取的信息来显示好友列表
        for i in self.friendlist:
            self.listButton.append(QtWidgets.QPushButton(self.scwidget))
            self.listButton[num].setGeometry(QtCore.QRect(0, 62*num, 190, 62))
            self.listButton[num].setText(i)
            self.listButton[num].setStyleSheet('border:2px groove gray;border-radius:10px;padding:2px 4px;')
            self.tt = i
            self.listButton[num].clicked.connect(self.cuser(i))
            num = num+1
        self.scroll = QtWidgets.QScrollArea(self)
        self.scroll.setWidget(self.scwidget)
        self.scroll.setMinimumSize(210,450)
        self.scroll.setObjectName("hhhh")
        self.scroll.setStyleSheet("#hhhh{border-image:url(./img/Mychat3.png);}")
        self.scroll.setHorizontalScrollBarPolicy(QtCore.Qt.ScrollBarAlwaysOff)


        self.fileButton = QtWidgets.QPushButton(self)       #发送文件的按钮
        self.fileButton.setGeometry(QtCore.QRect(220, 300, 35, 35))
        self.fileButton.clicked.connect(self.fileButtoncliked)
        self.fileButton.setStyleSheet("border-image:url(./img/file.png);")
        self.fileButton.hide()

        self.secinput = QtWidgets.QWidget() #输入录制时长的窗口
        self.secinput.resize(400,200)
        self.inputhin = QtWidgets.QLabel(self.secinput)

        self.inputhin.setGeometry(QtCore.QRect(50, 35, 300, 20))  #输入提示信息
        self.inputhin.setObjectName("hin")
        self.inputhin.setText("Please input the recording seconds ")

        self.sinput = QtWidgets.QLineEdit(self.secinput) #时长输入框
        self.sinput.setGeometry(QtCore.QRect(100, 75, 200, 20))
        self.sinput.setObjectName("inputbox")

        self.enterbutton = QtWidgets.QPushButton(self.secinput) #确认按钮
        self.enterbutton.setGeometry(QtCore.QRect(150, 125, 65, 35))
        self.enterbutton.setText('Enter')
        self.enterbutton.setStyleSheet('border:2px groove gray;border-radius:10px;padding:2px 4px;')
        self.enterbutton.clicked.connect(self.getsec)


        self.voiceButton = QtWidgets.QPushButton(self)      #发送声音的按钮
        self.voiceButton.setGeometry(QtCore.QRect(255, 300, 35, 35))
        self.voiceButton.clicked.connect(self.voiceButtoncliked)
        self.voiceButton.setStyleSheet("border-image:url(./img/voice.png);")
        self.voiceButton.hide()

        self.playButton = QtWidgets.QPushButton(self)  # 播放音频消息的按钮
        self.playButton.setGeometry(QtCore.QRect(360, 300, 35, 35))
        self.playButton.clicked.connect(self.playButtoncliked)
        self.playButton.setStyleSheet("border-image:url(./img/playy.png);")
        self.playButton.hide()


        self.downloadButton = QtWidgets.QPushButton(self) #下载文件的按钮
        self.downloadButton.setGeometry((QtCore.QRect(325,300,35,35)))
        self.downloadButton.clicked.connect(self.downloadButtoncliked)
        self.downloadButton.setStyleSheet("border-image:url(./img/download.png);")
        self.downloadButton.hide()

        self.emojiButton = QtWidgets.QPushButton(self)      # 发送表情的按钮
        self.emojiButton.setGeometry(QtCore.QRect(290, 300, 35, 35))
        self.emojiButton.clicked.connect(self.emojiButtoncliked)
        self.emojiButton.setStyleSheet("border-image:url(./img/emoji.png);")
        self.emojiButton.hide()

        self.fileselect = QtWidgets.QFileDialog(self)       #文件选择界面
        self.fileselect.setGeometry(QtCore.QRect(248, 341, 500, 62))

        self.downloadlist = QtWidgets.QListWidget(self) #下载文件选择界面
        self.downloadlist.setGeometry(QtCore.QRect(290, 100, 125, 200))
        self.downloadlist.doubleClicked.connect(self.downloadlistDoubleClicked)
        self.downloadlist.hide()

        self.voicelist = QtWidgets.QListWidget(self) #播放语音消息选择界面
        self.voicelist.setGeometry(QtCore.QRect(290, 100, 250, 200))
        self.voicelist.doubleClicked.connect(self.voicelistDoubleClicked)
        self.voicelist.hide()

        self.emoji = QtWidgets.QTableWidget(self)           #表情列表
        self.emoji.setGeometry(QtCore.QRect(270, 255, 120, 40))
        self.emoji.verticalHeader().setVisible(False)       # 隐藏垂直表头
        self.emoji.horizontalHeader().setVisible(False)     # 隐藏水平表头
        self.emoji.setHorizontalScrollBarPolicy(QtCore.Qt.ScrollBarAlwaysOff)   # 隐藏垂直滚动条
        self.emoji.setVerticalScrollBarPolicy(QtCore.Qt.ScrollBarAlwaysOff)     # 隐藏水平滚动条
        self.emoji.setColumnCount(3)
        self.emoji.setRowCount(1)
        label = []
        for i in range(3):
            icon = QtWidgets.QLabel()
            #icon.setMargin(4)
            s = './img/'+str(i)+'.png'
            print(s)
            mp = QtGui.QPixmap(s)
            mp.scaled(25, 25, QtCore.Qt.KeepAspectRatio)
            icon.setPixmap(mp)
            self.emoji.setCellWidget(0 , i , icon)
            self.emoji.setColumnWidth(i, 40)          # 设置列的宽度
            self.emoji.setRowHeight(0, 40) # 设置行的高度
        self.emoji.cellClicked.connect(self.emojicliked)
        self.emoji.hide()

        self.retranslateUi()
        QtCore.QMetaObject.connectSlotsByName(self)

    def cuser(self, t):
        def calluser():
            """
            好友按钮触发函数
            """
            self.user.setText(str(t))
            self.chatuser = t
            self.sendText.show()
            self.sendtxtButton.show()
            self.fileButton.show()
            self.voiceButton.show()
            self.emojiButton.show()
            self.downloadButton.show()
            self.playButton.show()
            self.grprecvText.clear()
            #根据选择的好友改变消息框的信息
            for item in con.recmsg:
                if (item[0] == t):
                    self.grprecvText.append(item[1]+" "+item[2]+" :")
                    if(str(type(item[3]) ) == "<class 'str'>"):
                        self.grprecvText.append(item[3])
                    elif(str(type(item[3])) == "<class 'int'>"):
                        img = QtGui.QTextImageFormat()
                        self.grprecvText.insertPlainText("\n")
                        path = "./img/"+str(item[3]) + ".png"
                        tcursor = self.grprecvText.textCursor()
                        img = QtGui.QTextImageFormat()
                        img.setName(path)
                        img.setHeight(30)
                        img.setWidth(30)
                        tcursor.insertImage(img)
            self.grprecvText.show()
        return calluser

    def fileButtoncliked(self):
        """
        文件选择函数
        """
        fileinfo = self.fileselect.getOpenFileName(self, 'OpenFile')
        print(fileinfo)
        filepath, filetype = os.path.splitext(fileinfo[0])
        filename = filepath.split("/")[-1]+filetype
        print(filename,filetype)
        if fileinfo[0] != '':
            with open(fileinfo[0], mode='rb') as f:
                r = f.read()
                f.close()
            file_r = base64.encodebytes(r).decode("utf-8")
            a = str({"type": "msg",
                     "mtype": "file",
                     "dname": self.chatuser,
                     "sname": self.Username,
                     "time": time.time(),
                     "msg": file_r,
                     "fname":filename}).encode('utf-8')
            con.s.send(a)
        time.sleep(0.05)
        if con.senderror:
            self.r = loginhint()
            self.r.loginError.setText("This user is not online !")
            self.r.show()
            con.senderror = False

    def voiceButtoncliked(self):
        self.secinput.show()

    def getsec(self):
        """
        确认录制时长函数
        """
        #判断输入是否合法
        if  self.sinput.text().isdigit():
            sec = int(self.sinput.text())
            if (sec > 15 or sec < 0):
                self.k = loginhint()
                self.k.loginError.setText("Time must be from 0 to 15s !")
                self.k.show()
            else:
                fs = 40000
                myrecording = sd.rec(sec * fs, samplerate=fs, channels=1, blocking=True)
                tt = myrecording.tobytes()
                a = str({"type": "msg",
                         "mtype": "voice",
                         "dname": self.chatuser,
                         "sname": self.Username,
                         "time": time.time(),
                         "len": self.sinput.text(),
                         "msg": tt}).encode()
                self.secinput.close()
                con.s.send(a)
                time.sleep(0.05)
                if con.senderror:
                    self.r = loginhint()
                    self.r.loginError.setText("This user is not online !")
                    self.r.show()
                    con.senderror = False
        else :
            self.k = loginhint()
            self.k.loginError.setText("Time must be from 0 to 15s !")
            self.k.show()
        self.sinput.clear()

    def  emojiButtoncliked (self):
        self.emoji.show()

    def downloadButtoncliked(self):
        """
        点击文件下载按钮触发此事件
        """
        self.downloadlist.clear()
        ltemp = []
        for item in con.recfile:
            if item[0] == self.chatuser:
                ltemp.append(item[4])
        self.downloadlist.addItems(ltemp)
        self.downloadlist.show()

    def playButtoncliked(self):
        """
        点击播放语音消息按钮触发此事件
        """
        self.voicelist.clear()
        ltemp = []
        for item in con.recvoice:
            if item[0] == self.chatuser:
                ltemp.append(item[2])
        self.voicelist.addItems(ltemp)
        self.voicelist.show()

    def downloadlistDoubleClicked(self):
        """
        文件下载
        """
        name = self.downloadlist.currentItem().text()
        print(name)
        for item in con.recfile:
            if (item[0] == self.chatuser and name == item[4]):
                h = item[3]
                print(h)
                path = "./" + self.Username + "/" + item[4]
                with open(path, "wb") as f:
                    f.write(base64.b64decode(h))
                    f.close()
        self.downloadlist.hide()

    def voicelistDoubleClicked(self):
        """
        语音消息播放
        :return:
        """
        name = self.voicelist.currentItem().text()
        print(name)
        for item in con.recvoice:
            if (item[0] == self.chatuser and name == item[2]):
                h = item[3]
                x = numpy.frombuffer(h, dtype=numpy.float32)
                sd.play(x, 40000, blocking=True)
        self.voicelist.hide()


    def emojicliked(self,row , col):
        """
        发送表情
        :param row:
        :param col:
        :return:
        """
        a = str({"type": "msg",
                 "mtype": "emoji",
                 "dname": self.chatuser,
                 "sname": self.Username,
                 "time": time.time(),
                 "msg": col}).encode()
        con.s.send(a)
        self.emoji.hide()
        time.sleep(0.05)
        if con.senderror:
            self.r = loginhint()
            self.r.loginError.setText("This user is not online !")
            self.r.show()
            con.senderror = False
        pass

    def sendclicked(self):
        """
        发送文字消息
        :return:
        """
        text = self.sendText.toPlainText()
        if (len(text)):
            a =  str({"type": "msg",
                        "mtype": "msg",
                        "dname": self.chatuser,
                        "sname": self.Username,
                        "time": time.time(),
                        "msg": text}).encode()
            con.s.send(a)
        time.sleep(0.05)
        if con.senderror :
            self.r = loginhint()
            self.r.loginError.setText("This user is not online !")
            self.r.show()
            con.senderror = False
        self.sendText.clear()

    def retranslateUi(self):
        _translate = QtCore.QCoreApplication.translate
        self.setWindowTitle(_translate("MyChat", self.Username))
        self.sendtxtButton.setText(_translate("txtsendButton", "Send"))

    def __del__(self):
        con.s.close()




if __name__ == "__main__":

    app = QtWidgets.QApplication(sys.argv)
    login = loginwindow()
    login.show()
    con = connect()
    con.run()
    sys.exit(app.exec_())