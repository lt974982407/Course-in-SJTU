import requests
from bs4 import BeautifulSoup
import csv
from selenium import webdriver

#获取电影详情url的函数
def getdetailurl(url, cookie):
    #模拟浏览器，并用cookie实现登录
    headers = {'cookie':cookie,'User-Agent':'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/90.0.4430.85 Safari/537.36 Edg/90.0.818.49'}
    #获取html文件
    response = requests.get(url,headers = headers)
    #使用beautiful soup解析html文件
    soup = BeautifulSoup(response.text,"html.parser")
    #所有的电影标签
    items = soup.findAll("div",class_ = "hd")
    res = []#存储结果的列表
    for item in items:
        urls = item.a.get('href')#获得电影的url
        #print(urls)
        temp = getdetail(urls,cookie)#获取电影的详细信息
        res.append(temp)
    for i in res:
        print(i)
    return res

def getdetail(url,cookie):
    # 模拟浏览器，并用cookie实现登录
    headers = {'cookie': cookie,'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/90.0.4430.85 Safari/537.36 Edg/90.0.818.49'}
    response = requests.get(url, headers=headers)
    soup = BeautifulSoup(response.text, "html.parser")
    tmp = []
    #获取电影的名称
    name = soup.find("span",property = "v:itemreviewed")
    tmp.append(name.string)
    #获取电影的导演、编剧、主演
    creator = soup.find("div", id = "info").findAll("span",class_ = "attrs")
    for i in creator :
        ts = ''
        t = i.findAll("a")
        for s in t :
            ts = ts + s.string + "/"
        ts=ts[ : -1]
        tmp.append(ts)
    #获取电影的简介
    discription = soup.find("div",id = "link-report").find("span",property = "v:summary").text
    tmp.append(discription)
    #获得电影的打分
    score = soup.find("strong",class_ = "ll rating_num").text
    tmp.append(score)
    #获得电影的获奖情况
    awards = soup.findAll("ul" , class_ = "award")
    award_string = ''
    for award in awards:
        award_string = award_string + award.li.a.text
        award_string += ' '
        award_string = award_string + award.findAll("li")[1].text
        award_string += '/'
    award_string = award_string[ : -1]
    tmp.append(award_string)
    return tmp

def main():
    cookiestr = login ()#调用登录的函数
    res = []#存储结果
    while (True):
        #输入需要爬取的页数
        s = input("请输入需要获取的页数（1-10）：")
        if (s not in ['1', '2', '3', '4', '5', '6', '7', '8', '9', '10']):#输入不合法
            print('输入不合法！')
        else :
            num = int(s) - 1
            for i in range(num + 1):
                url = "https://movie.douban.com/top250?start=" + str(i * 25) + "&filter="#具体的url
                restmp = getdetailurl(url,cookiestr)
                for item in restmp :
                    res.append(item)
            break
    with open("res.csv", "w", newline="", encoding='utf-8-sig') as datacsv:
        # dialect为打开csv文件的方式，默认是excel，delimiter="\t"参数指写入的时候的分隔符
        csvwriter = csv.writer(datacsv, dialect=("excel"))
        # 把数据写入csv文件
        csvwriter.writerow(["影片名称", "导演", "编剧", "主演", "简介", "评分", "获奖情况"])
        for i in res:
            csvwriter.writerow(i)

def login ():
    driver = webdriver.Chrome('E:\chromedriver_win32\chromedriver.exe')
    driver.get('https://www.douban.com/')#打开豆瓣的主页
    iframe = driver.find_element_by_tag_name("iframe")#找到iframe
    driver.switch_to.frame(iframe)
    #填入手机号
    username = input("请输入手机号：")
    driver.find_element_by_name('phone').send_keys(username)
    #点击获取验证码
    driver.find_element_by_xpath("//div[@class = 'account-form-field-code']/a").click()
    #输入验证码
    password = input("请输入验证码：")
    driver.find_element_by_id('code').send_keys(password)
    #登录
    driver.find_element_by_xpath("//div[@class = 'account-form-field-submit ']/a").click()
    #获取cookie
    cookies = driver.get_cookies()
    print(cookies)
    #转换cookie的格式
    cookie = [item["name"] + "=" + item["value"] for item in cookies]
    cookiestr = '; '.join(item for item in cookie)
    return cookiestr

if __name__ == '__main__':
    main()