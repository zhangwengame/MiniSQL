# coding=utf-8
import sys
import re
def interpreter():
    def select_clause(s):
        if "from" not in s:
            print "No table are seleceted!"
        elif "where" in s:
            m = re.match(r"select(.*)from(.*)where(.*);", s)
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
            m = re.match(r"select(.*)from(.*);", s)
            s1=m.group(1).replace(" ","")
            s2=m.group(2).replace(" ","")
            s1=s1.replace(",",".");
            s="20"+s1+","+s2
            return s

    def create_clause(s):
        if "database" in s:
            m = re.match(r"create.*database(.*);", s)
            s1=m.group(1).replace(" ","")
            s="00"+s1
        elif "table" in s:
            s1=s[:s.find('(')]
            s2=s[s.find('(')+1:]
            m=re.match(r"create.*table(.*)", s1)
            s1=m.group(1).replace(" ","")
            s2=s2.replace(" ","")
            print "s1",s1
            print "s2",s2
            s="01"+s1+','
            s3=""
            if "primarykey" in s2:
                m=re.match(r"(.*),primarykey\((.*)\);$", s2)
                s2=m.group(1)
                s3=m.group(2)
                s3=s3.replace(")","")
            else:
                m=re.match(r"(.*)\);$", s2)
                s2=m.group(1)
            print s2
            s2=s2.split(',')
            for t in s2:
                if len(s3)>0 and s3 in t:
                    t='!'+t
                if "unique" in t:
                    t=t.replace("unique","")
                    t='@'+t;
                if "int" in t:
                    t=t.replace("int",'i');
                elif "float" in t:
                    t=t.replace("float",'f');
                elif "char" in t:
                    t=t.replace("char",'c');
                s=s+t+'.'
        elif "index" in s:
            m = re.match(r"create.*index(.*)on(.*)\((.*)\);", s)
            s1=m.group(1).replace(" ","")
            s2=m.group(2).replace(" ","")
            s3=m.group(3).replace(" ","")
            s="02"+s1+'.'+s2+'.'+s3
        return s;

    sql=""
    sql=raw_input(">>")
    while True:
        if ';' in sql:    #$为结束字符
            break
        s=raw_input()
        sql+=" "
        sql+=s
    if "select" in sql:
        sql=select_clause(sql)
    elif "create" in sql:
        sql=create_clause(sql)
    #sql=sql.replace("$","")
    #return sql
    print sql

interpreter()