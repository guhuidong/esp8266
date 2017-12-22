import SocketServer
from SocketServer import StreamRequestHandler as SRH
import socket 
import MySQLdb
host = '192.168.100.110'
port = 9999
addr = (host,port)
class Servers(SRH):
    def handle(self):
        conn = MySQLdb.connect(host='localhost',port = 3306,user='root',passwd='telecom660786',db ='smart_home',)
        insertstr1=format('insert into smart_control (mac) values ("%s") on duplicate key update type=0,status=1')
        insertstr2=format('insert into smart_control (mac) values ("%s") on duplicate key update type=1,status=1')
        insertstr3=format('insert into smart_control (mac) values ("%s") on duplicate key update type=0')
        selecttstr=format('select status from smart_control where mac="%s"')
        print ('got connection from ' , self.client_address)
        self.mac=''
        self.request.settimeout(5)
        while True:
            try:
                self.data = self.request.recv(1024)
                if self.data==None or len(self.data)==0:
                    aa=insertstr2 % (self.mac)
                    cur=conn.cursor()
                    cur.execute(aa)
                    conn.commit()
                    cur.close()
                    print('connection close')
                    break;
                if self.data[:3]=='add':
                    self.mac=self.data[3:20]
                    aa=insertstr1 % (self.data[3:20])
                    cur=conn.cursor()
                    cur.execute(aa)
                    conn.commit()
                    cur.close()
                if self.data[:3]=='hrt':
                    self.mac=self.data[3:20]
                    aa=insertstr3 % (self.data[3:20])
                    cur=conn.cursor()
                    cur.execute(aa)
                    conn.commit()
                    cur.close()
                    aa=selecttstr % (self.data[3:20])
                    cur=conn.cursor()
                    count=cur.execute(aa)
                    result=cur.fetchone()
                    sts=result[0]
                    cur.close()
                    if sts==1:
                        self.connection.sendall('%s\n' % 'LIGHTONE')
                    if sts==2:
                        self.connection.sendall('%s\n' % 'LIGHTTWO')
                    if sts==3:
                        self.connection.sendall('%s\n' % 'LIGHTTHREE')
                    if sts==4:
                        self.connection.sendall('%s\n' % 'LIGHTFOUR')
                    if sts==0:
                        self.connection.sendall('%s\n' % 'LIGHTOFF');
            except socket.timeout,e:
                print (e)
                print ('connection is close....')
                aa=insertstr2 % (self.mac)
                cur=conn.cursor()
                cur.execute(aa)
                conn.commit()
                cur.close()
                break
        self.connection.close()
if __name__ == '__main__':
    print ('server is running....')
    SocketServer.TCPServer.allow_reuse_address=True
    server = SocketServer.ThreadingTCPServer(addr,Servers)
    server.socket.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1)
    server.serve_forever()
