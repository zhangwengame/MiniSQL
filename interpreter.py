# coding=utf-8
import sys
import re
def select_clause(s):
    if "from" not in s:
        print "No table are seleceted!"
    elif "where" in s:
        m = re.match(r"select(.*)from(.*)where(.*)$", s)
        if m:
            s1=m.group(1).replace(" ","")
            s2=m.group(2).replace(" ","")
            s3=m.group(3).replace(" ","")
            s1=s1.replace(",",".");
            s3=s3.replace("and","&");
            s3=s3.replace("or","|");
            s="21"+s1+","+s2+","+s3
            return s
    else:
        m = re.match(r"select(.*)from(.*)$", s)
        s1=m.group(1).replace(" ","")
        s2=m.group(2).replace(" ","")
        s1=s1.replace(",",".");
        s="20"+s1+","+s2
        return s
sql=""
sql=raw_input(">>")
while True:
    s=raw_input()
    if '$' in s:    #$为结束字符
        break
    sql+=" "
    sql+=s
print sql
if "select" in sql:
    sql=select_clause(sql)
print sql
    
        
            
