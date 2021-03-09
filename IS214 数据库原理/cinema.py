import psycopg2
import datetime
#import getpass

conn = psycopg2.connect(database="cinema", user="user", password="password", host="127.0.0.1", port="5432")
cursor = conn.cursor()


def main():
    print("Welcome, if you're a customer, please type 1, while if you're the manager, please type 2")
    identity = int(input('>'))

    while identity != 1 and identity != 2:
        print("You have input an illegal number, please input again.")
        identity = int(input('>'))

    if identity == 1:
        print("Do you already have an account? If you already have an account, please input 1, else please input 2 for registration")
        num = input('>')
        while num != '1' and num != '2':
            print("Sorry, you have input an illegal number, please input again.")
            num = input('>')

        if num == '1':
            login(1)
        else:
            register()


    if identity == 2:
        login(2)


def login(i):
    print("Please input your id ")
    id = input('>')
    strsql = "SELECT password from users where id='"+str(id)+"'"
    cursor.execute(strsql)
    row = cursor.fetchall()
    ps = str(row[0][0])
    #if row[0][0] == None:
    #    print("You haven't register yet, please register.")
    #    register()
    print("Please input your password")
    password = input('>')
    password=password.ljust(10, ' ')

    while password != ps:
        print("Sorry, your password is invalid. Please input again.")
        password = input('>')

    print("You have login successfully.")
    username = id
    if i == 1:
        user(username)
    if i == 2:
        manager()

def register():
    print("Please input your id :")
    id = input('>')
    print("Please input your id and password:")
    password = input('>')
    strsql="INSERT INTO users values('"
    strsql+=str(id)+"','"+str(password)+"')"
    cursor.execute(strsql)
    conn.commit()
    print("You have registered successfully.")

    user(id)

def buy(username):
    sqlstr = "SELECT fn, rno from ticket where id='"+str(username)+"'"
    cursor.execute(sqlstr)
    rows = cursor.fetchall()
    print("\n")
    print("user id:"+username)
    print("FILM NAME:      ROOM:")
    for row in rows:
        print(row[0]+"      "+row[1])
    print("\n")

def user(username):
    option = 'c'
    print("Welcome to our cinema!")
    while option != 'q':
        print("Please choose what you what to do from options below(type q to exit):")
        print("a. Film introduction")
        print("b. Timetable")
        print("c. buy ticket")
        print("d. return ticket")
        print("e. show tickets you have bought")

        option = input('>')

        if option == 'q':
            print("Goodbye")
            return
        elif option == 'a':
            cursor.execute("SELECT fn, time, pr from film")
            rows = cursor.fetchall()
            print("\n")
            print("Film Name:          Time Length:    Ticket Price:")
            for row in rows:
                print(str(row[0])+"          "+str(row[1])+"             "+str(row[2]))
            print("\n")

        elif option == 'b':
            cursor.execute("SELECT fn, rno, rest, st, ft from timetable")
            rows = cursor.fetchall()
            print("\n")
            print("Film Name:      Room Number:    Rest of tickets:      Start Time:          Finish Time:")
            for row in rows:
                r = str(row[2])
                if int(r) < 100:
                    r = "0"+str(row[2])
                print(str(row[0])+"      "+str(row[1])+"               "+str(r)+"                  "+str(row[3])+"              "+str(row[4])+"\n")


        elif option == 'c':
            print("Which film would you like to watch?")
            fn = input("file name>")
            #sql_fn = "SELECT fn from film where fn='" + str(fn) + "'"
            #cursor.execute(sql_fn)
            print("At which room?")
            rno = input("room number>")
            sqlstr = "SELECT rest from timetable where fn='" + str(fn) + "'" + "and rno='" + str(rno) + "'"
            cursor.execute(sqlstr)
            r = cursor.fetchall()
            rest = r[0][0]
            if rest == 0:
                print("Sorry, the tickets have been sold out")
            else:
                rest -= 1
                sqlstr = "UPDATE timetable set rest=" + str(rest) + " where fn='" + str(fn) + "' and rno='" + str(
                    rno) + "'"
                cursor.execute(sqlstr)
                sqlstring="insert into ticket values ('"+username+"','"+fn+"','"+rno+"')"
                cursor.execute(sqlstring)
                conn.commit()
                print("Buy ticket successfully\n")

            """donot know if he has buy a ticket, cannot judge if the film exists"""
        elif option == 'd':
            print("Please input the film name")
            fn = input("file name>")
            # sql_fn = "SELECT fn from film where fn='" + str(fn) + "'"
            # cursor.execute(sql_fn)
            print("At which room?")
            rno = input("room number>")
            sqlstr = "SELECT rest from timetable where fn='" + str(fn) + "'" + "and rno='" + str(rno) + "'"
            cursor.execute(sqlstr)
            r = cursor.fetchall()
            rest = r[0][0]
            rest += 1
            sqlstr = "UPDATE timetable set rest=" + str(rest) + " where fn='" + str(fn) + "' and rno='" + str(rno) + "'"
            cursor.execute(sqlstr)
            sqlstring = "delete from ticket where id='" + username + "'and fn='" + fn + "'and rno='" + rno + "'"
            cursor.execute(sqlstring)
            conn.commit()
            print("Return ticket successfully\n")

        elif option == 'e':
            buy(username)

        else:
            print("Sorry, you have input an illegal instruction, please input try again.")



def manager():
    option = 'd'
    print("Welcome.")
    while option != 'q':
        print("Please choose what you want to do from options below(type q to exit):")
        print("a. Film Introduction")
        print("b. TimeTable")
        print("c. Change TimeTable")
        print("d. Change Film")
        print('e. Change room')
        print('f. Business volume')

        option = input('>')

        if option == 'q':
            print("Goodbye")
            return

        elif option == 'a':
            cursor.execute("SELECT fn, time, pr from film")
            rows = cursor.fetchall()
            print("\n")
            print("Film Name:          Time Length:    Ticket Price:")
            for row in rows:
                print(str(row[0]) + "          " + str(row[1]) + "             " + str(row[2]))
            print("\n")

        elif option == 'b':
            cursor.execute("SELECT fn, rno, rest, st, ft from timetable")
            rows = cursor.fetchall()
            print("\n")
            print("Film Name:      Room Number:    Rest of tickets:      Start Time:          Finish Time:")
            for row in rows:
                r = str(row[2])
                if int(r) < 100:
                    r = "0" + str(row[2])
                print(str(row[0]) + "      " + str(row[1]) + "               " + str(r) + "                  " + str(row[3])+"              "+str(row[4])+"\n")

        elif option == 'c':
            print("please input what you want to do with timetable(add, delete, alter)")
            action = input('>')

            while action != 'add' and action != 'delete' and action  != 'alter':
                print("Sorry, you have input an illegal instruction, please input again.")
                action = input('>')

            if action == 'add':
                if action == 'add':
                    sqlstr_select = "SELECT num from room where rno="
                    sqlstr = "INSERT INTO timetable(fn, rno, rest, st, ft)"
                    print("Please input the film name")
                    fn = input("file name>")
                    sql = "SELECT fn from film where fn="
                    sql += str(fn)
                    # try:
                    #   cursor.execute(sql)
                    # except:
                    #   print("The film does not exist.")
                    #  break
                    print("Please input the room number")
                    rno = input("room number>")
                    sqlstr_select += "'" + str(rno) + "'"
                    cursor.execute(sqlstr_select)
                    row = cursor.fetchall()
                    rest = row[0][0]
                    print("Please input the start time")
                    st = input("start time>")

                    if st[1] == ':':
                        st = ''.join(['0', st])
                    hour = int(st[0]) * 10 + int(st[1])
                    minute = int(st[3]) * 10 + int(st[4])
                    second = int(st[6]) * 10 + int(st[7])

                    stt = datetime.time(hour, minute, second)

                    sqlstr_select = "SELECT time from film where fn='" + str(fn) + "'"
                    cursor.execute(sqlstr_select)
                    time = cursor.fetchall()
                    t = int(time[0][0])

                    h = int(t / 60)
                    m = int(t % 60)
                    s = int(t - m - 60 * h)

                    hour += h
                    minute += m
                    second += s

                    if second < 10:
                        ss = "0" + str(second)
                    else:
                        ss = str(second)
                    if minute < 10:
                        mm = "0" + str(minute)
                    else:
                        mm = str(minute)

                    ft = str(hour) + ":" + str(mm) + ":" + str(ss)

                    ftt = datetime.time(hour, second, minute)

                    sql_time = "SELECT st, ft from timetable where rno='"+str(rno)+"'"
                    cursor.execute(sql_time)
                    t = cursor.fetchall()

                    for tt in t:
                        if (stt < tt[0] and ftt < tt[1] and ftt > tt[0]) or (
                                stt > tt[0] and stt < tt[1] and ftt > tt[1]) or (stt > tt[0] and ftt < tt[1]) or (
                                stt < tt[0] and ftt > tt[1]):
                            print("illegal time")
                            return

                    sqlstr += "VALUES('" + str(fn) + "', '" + str(rno) + "'," + str(rest) + ", '" + str(
                        st) + "', '" + str(ft) + "')"
                    cursor.execute(sqlstr)
                    conn.commit()
            elif action == 'delete':
                sqlstr = "DELETE FROM timetable WHERE "
                print("Please input the film name")
                fn = input("film name>")
                print("Please input the room number")
                rno = input("room number>")
                print("Please input the start time")
                st = input("start time>")
                sqlstr += "fn='" + str(fn) + "'and rno='" + str(rno) + "'and st='" + str(st) +  "'"
                cursor.execute(sqlstr)
                conn.commit()
            elif action == 'alter':
                print("Please input which film do you want to change")
                fn = input('film name>')
                print("at which room")
                rno = input('room number>')
                print("please input the new start time")
                st = input('start time>')

                if st[1] == ':':
                    st = ''.join(['0', st])
                hour = int(st[0]) * 10 + int(st[1])
                minute = int(st[3]) * 10 + int(st[4])
                second = int(st[6]) * 10 + int(st[7])
                stt = datetime.time(hour, minute, second)

                sqlstr = "SELECT time from film where fn='" + str(fn) + "'"
                cursor.execute(sqlstr)
                time = cursor.fetchall()
                t = time[0][0]

                h = int(t / 60)
                m = int(t % 60)
                s = int(t - 60 * h - m)

                hour = int(h + hour)
                minute = int(minute + m)
                second = int(second + s)

                ftt = datetime.time(hour, minute, second)

                sqlstr = "SELECT st, ft from timetable where rno='" + str(rno) + "'"
                cursor.execute(sqlstr)
                ttt = cursor.fetchall()

                for tt in ttt:
                    if (stt < tt[0] and ftt < tt[1] and ftt > tt[0]) or (
                            stt > tt[0] and stt < tt[1] and ftt > tt[1]) or (stt > tt[0] and ftt < tt[1]) or (
                            stt < tt[0] and ftt > tt[1]):
                        print("illegal time")
                        return

                sqlstr = "UPDATE timetable set st='" + str(stt) + "', ft='" + str(ftt) + "' where fn='" + str(
                    fn) + "' and rno='" + str(rno) + "'"
                cursor.execute(sqlstr)
                conn.commit()

        elif option=='d':
            print("please input what you want to do with Film(add, delete, alter)")
            action = input('>')
            while action != 'add' and action != 'delete' and action  != 'alter':
                print("Sorry, you have input an illegal instruction, please input again.")
                action=input('>')

            if action=='add':
                sqlstr="INSERT INTO Film(fn,time,pr) "
                print('Please input the name of the film')
                fn=input('>')
                print('How long does the film last?')
                time=input('>')
                print('Plese input the price of the film')
                pr=input('>')
                sqlstr+="VALUES ('"+str(fn)+"',"+str(time)+","+str(pr)+")"
                #print(sqlstr)
                cursor.execute(sqlstr)
                conn.commit()
            if action=='delete':
                sqlstr="DELETE FROM Film WHERE fn='"
                print("Please input the name of the film")
                fn=input('>')
                sqlstr+=str(fn)+"'"
                cursor.execute(sqlstr)
                conn.commit()
            if action=='alter':
                print('Please input what do you want to change(film time or film price)')
                choice=input('>')
                while choice !='film time' and choice !='film price':
                    input("Sorry, you have input an illegal instruction, please input again.")
                    choice=input('>')
                if choice =='film time':
                    print('Please input the name of the film you want to change:')
                    fn=input('>')
                    print('Please input the new time')
                    time=input(">")
                    sqlstr="UPDATE Film SET time ="
                    sqlstr+=str(time)+" where fn='"+str(fn)+"'"
                    cursor.execute(sqlstr)
                elif choice=='film price' :
                    print('Please input the name of the film you want to change:')
                    fn = input('>')
                    print('Please input the new price')
                    pr = input(">")
                    sqlstr = "UPDATE Film SET pr="
                    sqlstr += str(pr) + " where fn='" + str(fn) + "'"
                    cursor.execute(sqlstr)
                conn.commit()

        elif option =='f':
            strsql="select timetable.fn,num,rest,film.pr,room.rno from timetable,film,room where film.fn=timetable.fn and timetable.rno=room.rno "
            cursor.execute(strsql)
            rows = cursor.fetchall()
            money=0
            for row in rows:
                money+=float(row[3][1:])*(row[1]-row[2])
            print(money)

        elif option =='e':
            print("please input what you want to do with Room(add, delete, alter)")
            action= input('>')
            while action != 'add' and action != 'delete' and action  != 'alter':
                print("Sorry, you have input an illegal instruction, please input again.")
                action=input('>')

            if action == 'add':
                sqlstr = "INSERT INTO Room(rno,num) VALUES ('"
                print('Please input the NO of the room')
                rno=input('>')
                print('Please input the seat number of the room')
                num=input('>')
                sqlstr+=str(rno)+"',"+str(num)+")"
                cursor.execute(sqlstr)
            if action =='delete':
                sqlstr="DELETE FROM Room WHERE rno='"
                print("Please input the NO of the room")
                rno=input('>')
                sqlstr+=str(rno)+"'"
                cursor.execute(sqlstr)
            if action =='alter':
                    print('Please input the NO of the film you want to change:')
                    rno = input('>')
                    print('Please input the new number of seats')
                    num = input(">")
                    sqlstr = "UPDATE Room SET num ="
                    sqlstr += str(num) + " where rno='" + str(rno) + "'"
                    cursor.execute(sqlstr)
            conn.commit()

        else:
            print("Sorry, you have input an illegal instruction, please input try again.")


if __name__ == '__main__':
    main()
    conn.close()