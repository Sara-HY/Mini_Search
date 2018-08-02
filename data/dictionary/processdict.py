#!/usr/bin/python3

if __name__=="__main__":
    col=int(input("Input the index of column you want to save(start with 0):"))
    filename=input("Input filename of source:")
    outfile=input("Input filename of target:");
    fin=open(filename,'r')
    fout=open(outfile,'w')
    for line in fin:
        buf=line.split()
        data=buf[col]
        fout.write('%s\n' %data)
    fin.close();
    fout.close();
	
