from bs4 import BeautifulSoup
import requests
import sys
import re


def last_page(term, soup):
    page = []
    for link in soup.find_all(href=re.compile('^/search\?q='+term+'\&page=')):
        page.append(link.get('href'))
    large = 1
    for x in page:
        x = re.sub('/search\?q='+term+'\&page=','', x)
        if int(x) > large:
            large = int(x)
    return large

def search(soup, site, extension):
    list =[]
    for link in soup.findAll('a', attrs={'href': re.compile("^"+site+extension)}):
            list.append(link.get('href'))
    return list

def bbc(term):
    num = 1
    bbc_search = 'https://www.bbc.co.uk/search?q='+term+'&page='
    bbc_uk = 'https://www.bbc.co.uk/'
    pro = 'programmes/'
    news = 'news/'
    list = []
    
    #links will contain every hyperlink resulting from search
    links = []
    
    while True:
        r = requests.get(bbc_search+str(num))
        
        if r.status_code == 200:
            soup = BeautifulSoup(r.content, 'html.parser')
            if num == 1:
                last = last_page(term, soup)
            list.append(search(soup, bbc_uk, pro))
            list.append(search(soup, bbc_uk, news))
            x = 0
       
        if num < last:
            num += 1
        else:
            break
    
    while x < len(list):
        for y in list[x]:
            links.append(y)
        x += 1
    
    for i in links:
        print(i)

def main():
    if len(sys.argv) == 2:
       term = sys.argv[1]
    else:
        term = 'tesla'
    bbc(term)


if __name__ == '__main__':
    main()
