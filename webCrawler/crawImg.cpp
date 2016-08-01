#include<iostream>
#include<fstream>
#include<string>
#include<string.h>
#include<queue>
#include<netdb.h>
#include<sys/socket.h>
#include<unistd.h>
#include<stdlib.h>
#include<malloc.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unordered_set>
using namespace std;
#define DEFULT_PAGE_BUF_SIZE 1048576

queue<string> Urlqueue;
unordered_set<string> visitedUrl;
unordered_set<string> visitedImg;
int g_ImgCnt=0;
bool ParseURL(const string &url,string &host,string &resource)
{
    const char *pos=strstr(url.c_str(),"http://");
    if(pos==NULL)
        pos=url.c_str();
    else
        pos +=strlen("http://");
    //if(strstr(pos,"/")==0)
      //  return false;
    char *pHost=new char[100]();
    char *pResource=new char[1000]();
    sscanf(pos,"%[^/]%s",pHost,pResource);
    host=pHost;
    resource=pResource;
    if(resource.size()==0)
       resource +="/";
    delete[] pHost;
    delete[] pResource;
    return true;
}
bool GetHttpResponse(const string &Url,char *&Response,int &BytesRead)
{
    string host,resource;
    if(!ParseURL(Url,host,resource)){
        cout<<"Can not parse the url"<<endl;
        return false;
    }
    struct hostent *hp=gethostbyname(host.c_str());
    if(hp==NULL){
        cout<<"Can not find host address"<<endl;
        return false;
    }
    int sock_fd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(sock_fd==-1){
        cout<<"Can not create sock."<<endl;
        return false;
    }
    struct sockaddr_in sa;
    bzero(&sa,sizeof(sa));
    sa.sin_family=AF_INET;
    sa.sin_port=htons(80);
    memcpy(&sa.sin_addr,hp->h_addr,4);
    if(connect(sock_fd,(struct sockaddr *)&sa,sizeof(sa))==-1){
        cout<<"Can not connect:"<<Url<<endl;
        close(sock_fd);
        return false;
    }
   string request="GET "+resource+" HTTP/1.1\r\nHost:"+host+"\r\nConnection: close\r\n\r\n";//注意格式
   cout<<request<<endl;
   //string request="GET "+resource+" HTTP/1.1\r\n\r\n";
    if(-1==send(sock_fd,request.c_str(),request.size(),0)){
        cout<<"send error"<<endl;
        close(sock_fd);
        return false;
    }
    int ContentLength=DEFULT_PAGE_BUF_SIZE;
    //char *PageBuf=new char[ContentLength]();
    char *PageBuf=(char *)malloc(ContentLength);
    memset(PageBuf,0,ContentLength);
    BytesRead=0;
    int ret=1;
    cout<<"Read:";
    while(ret>0){
        ret=recv(sock_fd,PageBuf+BytesRead,ContentLength-BytesRead,0);
        if(ret==-1){
            cout<<"recv error"<<endl;
            return false;
        }
        if(ret>0)
            BytesRead +=ret;
        if(ContentLength-BytesRead<1000){
            cout<<"\nReallo memory"<<endl;
            ContentLength*=2;
            PageBuf=(char *)realloc(PageBuf,ContentLength);
        }
        cout<<ret<<"  ";
    }
    cout<<endl;
    PageBuf[BytesRead]='\0';
    Response=PageBuf;
    close(sock_fd);
    return true;
}
string ToFileName(const string &url)
{
    string fileName;
    fileName.resize(url.size());
    int k=0;
    for(int i=0;i<(int)url.size();i++){
        char ch=url[i];
        if(ch!='\\'&&ch!='/'&&ch!='*'&&ch!='?'&&ch!='"'&&ch!='<'&&ch!='>'&&ch!='|')
            fileName[k++]=ch;
    }
    return fileName.substr(0,k)+".txt";
}

void HTMLParse(string &htmlResponse,vector<string> &imgurls,const string &host)
{
    const char *p=htmlResponse.c_str();
    char *tag="href=\"";
    const char *pos=strstr(p,tag);
    ofstream ofile("url.txt",ios::app);
    while(pos){
        pos+=strlen(tag);
        const char *nextQ=strstr(pos,"\"");//find "
        if(nextQ){
            char *url=new char[nextQ-pos+1];
            sscanf(pos,"%[^\"]",url);
            char *httptag="http://";//如果不带http则加上主机名
            string surl;
            const char *postemp=strstr(url,httptag);
            if(postemp==NULL)
                surl=host;
             surl+=url;
            if(visitedUrl.find(surl)==visitedUrl.end()){
                visitedUrl.insert(surl);
                ofile<<surl<<endl;
                cout<<surl<<endl;
                Urlqueue.push(surl);
            }
            pos=strstr(pos,tag);
            delete[] url;
        }
    }
    ofile<<endl<<endl;
    ofile.close();

    tag="<img ";
    const char*att1="src=\"";
    const char *att2="lazy-src=\"";
    const char *pos0=strstr(p,tag);
    while(pos0){
        pos0+=strlen(tag);
        const char*pos2=strstr(pos0,att2);
        if(!pos2||pos2>strstr(pos0,">"))
            pos=strstr(pos0,att1)+strlen(att1);
        else
            pos=pos2+strlen(att2);
        const char *nextQ=strstr(pos,"\"");
        if(nextQ){
            const char *url=new char[nextQ-pos+1]();
            sscanf(pos,"%[^\"]",url);
            cout<<url<<endl;
            string imgUrl=url;
            if(visitedImg.find(imgUrl)==visitedImg.end()){
                visitedImg.insert(imgUrl);
                imgurls.push_back(imgUrl);
            }
            pos0=strstr(pos0,tag);
            delete[] url;
        }
    }
    cout<<"end of Parse this html"<<endl;
}
void DownLoadImg(vector<string>&imgurls,const string &url)
{
    string foldname=ToFileName(url);
    foldname="./img/"+foldname;
    mkdir(foldname.c_str(),0777);
    char *image;
    int byteRead;
    for(int i=0;i<imgurls.size();i++){
        string str=imgurls[i];
        int pos=str.find_last_of(".");
        if(pos==string::npos)
            continue;
        else{
            string ext=str.substr(pos+1,str.size()-pos-1);
            if(ext!="bmp"&&ext!="jpg"&&ext!="jpeg"&&ext!="gif"&&ext!="png")
                continue;
        }
        if(GetHttpResponse(imgurls[i],image,byteRead)){
            const char *p=image;
            const char *pos=strstr(p,"\r\n\r\n")+strlen("\r\n\r\n");
            int index=imgurls[i].find_last_of("/");
            if(index!=string::npos){
                string imgname=imgurls[i].substr(index,imgurls[i].size());
                ofstream ofile(foldname+imgname,ios::binary);
                if(!ofile.is_open())
                    continue;
                cout<<g_ImgCnt++<<foldname+imgname<<endl;
                ofile.write(pos,byteRead-(pos-p));
                ofile.close();
            }
            free(image);
        }
    }
}
void BFSfun(const string &Url)
{
    char *Response;
    int Bytes;
    if(!GetHttpResponse(Url,Response,Bytes)){
        cout<<"This url is wrong! ignore."<<endl;
        return;
    }
    string HttpResponse=Response;
    free(Response);
    string filename=ToFileName(Url);
    ofstream ofile("./html/"+filename);
    if(ofile.is_open()){
        ofile<<HttpResponse<<endl;
        ofile.close();
    }
    vector<string> imgurls;
    string host,resource;
    ParseURL(Url,host,resource);
    HTMLParse(HttpResponse,imgurls,host);
    DownLoadImg(imgurls,Url);
}

int main()
{
    mkdir("./img",0777);
    mkdir("./html",0777);
    string Urlstart="http://www.wmpic.me/tupian";
    Urlqueue.push(Urlstart);
    while(!Urlqueue.empty())
    {
        string Urltemp=Urlqueue.front();
        cout<<Urltemp<<endl;
        BFSfun(Urltemp);
        Urlqueue.pop();
    }
    return 0;
}
