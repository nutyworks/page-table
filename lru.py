#!/usr/bin/python3

from page import Page

max_table = int(input())
pages = map(lambda x: Page(int(x)),input().split(" "))
table = []
time = 0

def is_page_in_table(num):
    for t in table:
        if t.num == num:
            return True
    return False


def find_in_table(num):
    for t in table:
        if t.num == num:
            return t 
    return None 


def print_table():
    tableclone = table[:]
    tableclone.sort(key=lambda x: x.table_num)

#   print(list(map(lambda x: f"{x.table_num} {x.num}", tableclone)))
    print(list(map(lambda x: x.num, tableclone)))


for t, page in enumerate(pages):
    table.sort(key=lambda x: x.last_used)
    if is_page_in_table(page.num):
        find_in_table(page.num).last_used = t
        print_table()
        continue
    if len(table) >= max_table:
        elem = table.pop(0)
        page.table_num = elem.table_num 

    if page.table_num == -1:
        page.table_num = len(table)
    
    page.last_used = t
    table.append(page)

    print_table()
