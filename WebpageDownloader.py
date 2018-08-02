#!/usr/bin/python3
#coding:utf-8

from math import ceil
import random
from urllib import request
import os, time, re
import pickle
#urllib2 has been replaced by urllib.request in Python3

def write2file(obj,filename):
        with open(filename,'wb') as fout:
            pickle.dump(obj,fout)

def loadfromfile(filename):
    if not os.path.exists(filename):
        raise ValueError('%s does not exists' %filename)
    with open(filename,'rb') as fin:
        return pickle.load(fin)
    return None
    
class Bitmap:
    def __init__(self,numbits):
        if numbits<=0:
            raise Exception("Invalid parameter");
        self.numbits=numbits#number of bits for bitmap
        self.buf=bytearray(ceil(self.numbits/8))
        
    def turn_on_bit(self,pos):
        bytepos=pos//8
        bitpos=pos%8
        self.buf[bytepos]|=(0x01<<bitpos)
        
    def turn_off_bit(self,pos):
        bytepos=pos//8
        bitpos=pos%8
        self.buf[bytepos]&=(~(0x01<<bitpos))
        
    def get_bit(self,pos):
        bytepos=pos//8
        bitpos=pos%8
        return (self.buf[bytepos]&(0x01<<bitpos))>>bitpos
    
    def clear(self):
        for i in range(len(self.buf)):
            self.buf[i]=0x00
            
    def print_values(self):
        for i in range(self.numbits):
            bytepos=i//8
            bitpos=i%8
            print('%d'%((self.buf[bytepos]&(0x01<<bitpos))>>bitpos),end=' ')
        print()

class Bloomfilter:
    def __init__(self,numbits,numhash=8):
        if numbits<=0 or numhash<=0:
            raise Exception("Invalid parameters")
        self.numbits=numbits #number of bits for its bitmap
        self.numhash=numhash
        self.bitmap=Bitmap(self.numbits)
        self.hashtable=[0 for x in range(self.numhash)]
        
    def exists(self,elem):
        self.__hash_function_k(elem)
        res=1
        for k in range(self.numhash):
            res&=self.bitmap.get_bit(self.hashtable[k])
        return res
        
    def insert(self,elem):
        self.__hash_function_k(elem)
        for k in range(self.numhash):
            self.bitmap.turn_on_bit(self.hashtable[k])
            
    def clear(self):
        self.bitmap.clear()
    
    def __hash_function(self,elem):
        val=5381
        if type(elem)==int:
            belem=bytes(elem)
        elif type(elem)==str:
            belem=bytes(elem,'utf-8')
        else:
            raise Exception('Unsupported datatype')
        for b in belem:
            val+=((val<<5)+val+b)
        return val
    
    def __hash_function_k(self,elem):
        for k in range(self.numhash):
            val=self.__hash_function(elem)
            random.seed(val+k+47)
            self.hashtable[k]=random.randint(0,self.numbits-1)

#在Python中检测网页编码
class WebpageDownloader:
    def __init__(self,root_url,max_num=1e4,dir_name='./html',bloomfilter_sz=0,hash_sz=10):
        self.max_num=max_num #maximum number of web pages to download
        if not dir_name.endswith('/'):
            dir_name+='/'
        self.dir_name=dir_name
        if bloomfilter_sz<=0:
            bloomfilter_sz=30*max_num
        self.bloomfilter=Bloomfilter(bloomfilter_sz,hash_sz)
        self.counter=0 #number of urls processed successfully
        self.header={'User-Agent':'Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/29.0.1547.66 Safari/537.36'}
        self.logfile="webdownloader.log"
        self.mapfile="webdownloader.map"
        self.fp_log=open(self.logfile,'wt',encoding='utf-8')
        self.fp_map=open(self.mapfile,'wt',encoding='utf-8')
        self.docid=0
        
    def start(self,root_url):
        #create the path if it does not exist
        if not os.path.exists(self.dir_name):
            print('creating directory:'+self.dir_name)
            self.fp_log.write('creating directory:%s\n' %self.dir_name)
            #mkdirs can create non-existing parent directory，
            #whereas mkdir can't do it
            os.makedirs(self.dir_name)

        buffer=[root_url]
        top=0 #index of the url to be processed
        while(self.counter<self.max_num and top<len(buffer)):
            url=buffer[top]
            urlpathname='' #make a filename for the webpage
            try:#all the characters are encoded in ASCII,may need to tranlate it
                url.encode('ascii')
                urlpathname=url
            except:#non-ascii character(Chinese etc. is included in url)
                pos=url.find(r':')
                url=url[:pos+1]+request.pathname2url(url[pos+1:]) 
                urlpathname=request.url2pathname(url)
            
            if self.bloomfilter.exists(url):#duplicated url
                print('Duplicate  url:%s' %urlpathname)
                self.fp_log.write('Duplicate  url:%s\n' %urlpathname)
                top+=1
                continue
            print('Processing url:%s' %urlpathname)
            self.fp_log.write('Processing url:%s\n' %urlpathname)
            content=self.getHTML(url,self.header)
            if not content: #faild to download the infomation of the url
                print("Faild to get url:%s" %urlpathname)
                self.fp_log.write('Failed to get url:%s!\n' %urlpathname)
                top+=1
                continue
            self.bloomfilter.insert(url) #insert the url in bloomfilter
            #self.urlset.add(url)
            if self.max_num>len(buffer):#number of webpages processed not enough
                buffer+=self.extract_urls(url,content) #extract hyper links

            filename=self.dir_name+str(self.counter)+'.html'
            #content=self.content_filter(content)#clear the useless data
            
#             if os.path.exists(filename):#the file already exists
#                 top+=1
#                 self.counter+=1
#                 continue
            try:
                open(filename,'wt',encoding='utf-8').write(content)
            except IOError as err:
                print(err)
            else:
                self.fp_map.write('%d %s\n' %(self.counter,urlpathname))
                self.counter+=1
            top+=1
            
        if top>=len(buffer):
            print('No more urls to get!')
            self.fp_log.write('No more urls to get!\n')
        print("Download %d web pages done!\n" %(self.counter))
        self.fp_log.write("Download %d web pages done!\n" %(self.counter))
        self.fp_log.close()
        self.fp_map.close()
        
    #get the html for specific url
    def getHTML(self,url,header_str):
        self.header['Referer']=url
        req=request.Request(url,headers=self.header)
        retry=0
        while retry<3:
            #make sure all the characters in url are encoded with ASCII
            #Otherwise, exception is raised in rquest.urlopen(url)
            try:
                req=request.urlopen(req)
            except request.URLError as err:
                print(err)
                retry+=1
                print('Trying to reload '+url)
                self.fp_log.write('Trying to reload %s\n' %url)
                time.sleep(1)
                continue
            try:
                content=req.read().decode('utf-8','ignore')
            except Exception as err:
                print(err)
                self.fp_log.write('%s\n' %err)
            else:
                return content
        else:
            print('Unconnectable:'+url)
            self.fp_log.write('Unconnectable:%s\n' %url)
            return None

    #remove the useless information from html
    def content_filter(self,src):
        pos=src.find(r'<script')
        res=src[:pos]
        regexp=re.compile(r'</script>(.*?)<script',re.DOTALL)
        content_list=regexp.findall(src)
        for x in content_list:
            res+=x
            
        src=res
        pos=src.find(r'<')
        res=src[:pos]
        regexp=re.compile(r'>(.*?)<',re.DOTALL)
        content_list=regexp.findall(src)
        for x in content_list:
            res+=x
        return res

    #extract urls from the html
    def extract_urls(self,father_url,content):
        regexp=re.compile(r'href="(.*?)"',re.DOTALL)
        urls=regexp.findall(content)#find all hyper link
        #remove the urls for several sources
        invalid_suffix=('.css','.js','.jpg','.png','.gif','.ico')
        valid_idx=[]
        for idx in range(len(urls)):
            start_len=-1
            if urls[idx].startswith(r'http://'):
                start_len=7
            elif urls[idx].startswith(r'https://'):
                start_len=8
            if not start_len in (7,8):
                pos=-1;
                if urls[idx].startswith(r'/'):
                    pos=father_url.find(r'/',7)
                    urls[idx]=father_url[:pos]+urls[idx]
                elif urls[idx].startswith(r'../'):
                    pos=father_url.rfind(r'/')
                    pos=father_url.rfind(r'/',pos)
                    urls[idx]=father_url[:pos]+urls[idx][2:]
                elif urls[idx].startswith(r'./'):
                    pos=father_url.rfind(r'/')
                    urls[idx]=father_url[:pos]+urls[idx][1:]
                else:
                    pos=father_url.rfind(r'/')
                    urls[idx]=father_url[:pos+1]+urls[idx]
            res=map(lambda x:urls[idx].endswith(x),invalid_suffix)
            if True in res:
                continue
            #if urls[idx] in self.urlset:#duplicate url
            if self.bloomfilter.exists(urls[idx]):
                print('Duplicate  url:%s' %request.url2pathname(urls[idx]))
                self.fp_log.write('Duplicate  url:%s\n' %request.url2pathname(urls[idx]))
                continue
            valid_idx.append(idx)
        return [urls[i] for i in valid_idx];
    
if __name__=='__main__':
    #root_url='http://www.csdn.net/'
    #root_url='http://dataunion.org/'
    #root_url='http://programmer.csdn.net/'
    root_url=input('Input the root url: ')
    max_num=int(input('Input the maximum number of webpages you want: '))
    #max_num=500
    downloader=WebpageDownloader(root_url,max_num)
    downloader.start(root_url)
