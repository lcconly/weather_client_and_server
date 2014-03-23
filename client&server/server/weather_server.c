/*************************************************************************
	> File Name: weather_server.c
	> Author: liucheng
	> Mail: 1035377294@qq.com 
	> Created Time: Tue 04 Mar 2014 06:58:05 PM CST
 ************************************************************************/
//基本框架参照书152页
//#include<stdio.h>
//#include<stdlib.h>
//#include<sys/types.h>
//#include<sys/socket.h>
//#include<netinet/in.h>
//#include<string.h>
#include"weather_data_struct.h"
#include<time.h>

//#define MAXLINE 4096
#define SERV_PORT 6666
#define LISTENQ 8

/*判断城市是否可以查询*/
int is_city_can_check_weather(char *city){
    char *cityCurrent[34]={"nanjing","shanghai","beijing","tianjin","hefei","aomen",
                          "chongqing","fuzhou","lanzhou","guangzhou","nanning","guiyang",
                         "haikou","shijiazhuang","zhengzhou","haerbin","wuhan","changsha",
                           "changchun","nanchang","shenyang","huhehaote","yinchuan","xining",
                          "jinan","taiyuan","xian","chengdu","taibei","kunming","lasa",
                           "xianggang","wulumuqi","hangzhou"};
	int i=0;
    for(i=0;i<34;i++){
        if(strcmp(city,cityCurrent[i])==0)
            return 1;
    }
    return 0;
}

int main(int argc,char **argv){
    int listenfd,connfd,n;
    pid_t childpid;
    socklen_t chilen;
    char city[30];
	time_t b_time;
	struct tm *tim;
	b_time=time(NULL);
	tim=localtime(&b_time);
    //char buf[MAXLINE];
    struct sockaddr_in cliaddr,servaddr;
    struct client_data client;
    struct server_data server;
    if((listenfd=socket(AF_INET,SOCK_STREAM,0))<0){
        perror("Problem in creating the socket");
        exit(2);
    }//创建套接子

    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    servaddr.sin_port=htons(SERV_PORT);//服务器的信息配置

    bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr));

    listen(listenfd,LISTENQ);//监听
    printf("%s\n","Server running...waiting for connections.");
    while(1){
        chilen=sizeof(cliaddr);
        //有新连接，分配线程
        connfd=accept(listenfd,(struct sockaddr*)&cliaddr,&chilen);
        printf("%s\n","Received request...");
        
        if((childpid=fork())==0){
            printf("%s\n","Child created for dealing with client request");
            close(listenfd);//关闭主套接字
            //memset(&server,0,sizeof(server));
            //memset(&client,0,sizeof(client));
            while((n=recv(connfd,&client,sizeof(client),0))>0){
                printf("%s","String received from and resent to the client:");
                puts((char *)&client);
                if(client.type[0]==0x01){//收到的为城市是否存在的申请
                    //printf("Type1!!!\n");
                    strcpy(city,client.city);
                    if(is_city_can_check_weather(city)==1){
                        printf("city %s is being\n",city);
						server.type[0]=0x03;//存在
                        strcpy(server.city,city);
                    }
                    else {
						printf("city %s is not being\n",city);
                        server.type[0]=0x04;//不存在
                        strcpy(server.city,city);
                    }
                }
                else if(client.type[0]==0x02&&client.type[1]==0x01){//功能1，3
                    printf("function 1&&3 in city %s!!!\n",city);
                    strcpy(city,client.city);
                    if(client.date>0x05){//一般五天后的无法查询
                        server.type[0]=0x02;
                        server.type[1]=0x41;
                        strcpy(server.city,city);
                        server.year=htons((tim->tm_year)+1900);
                        server.month=(unsigned char)(tim->tm_mon+1);
                        server.day=(unsigned char)(tim->tm_mday);
                        server.date=client.date;
                    }    
                    else {
                        server.type[0]=0x01;//查询一天或者某一天的天气
                        server.type[1]=0x41;
                        strcpy(server.city,city);
                        server.year=htons((tim->tm_year)+1900);
                        //printf("year:%d\n",server.year);
                        server.month=(unsigned char)(tim->tm_mon+1);
                        server.day=(unsigned char)(tim->tm_mday);
                        server.date=client.date;
                        server.weather_id1=(unsigned char)(rand()%5);
                        server.temp1=(unsigned char)(rand()%40);
                        server.weather_id2=0x00;
                        server.temp2=0x00;
                        server.weather_id3=0x00;
                        server.temp3=0x00;
                    }
                }
                else if(client.type[0]==0x02&&client.type[1]==0x02){//功能2 的查询
                    printf("function 2 in city %s!!!\n",city);
                    strcpy(city,client.city);
                    server.type[0]=0x01;                        
                    server.type[1]=0x42;
                    strcpy(server.city,city);
                    server.year=htons((tim->tm_year)+1900);
                    server.month=(unsigned char)(tim->tm_mon+1);
                    server.day=(unsigned char)(tim->tm_mday);
                    server.date=client.date;
                    server.weather_id1=(unsigned char)(rand()%5);
                    server.temp1=(unsigned char)(rand()%40);
                    server.weather_id2=(unsigned char)(rand()%5);
                    server.temp2=(unsigned char)(rand()%40);
                    server.weather_id3=(unsigned char)(rand()%5);
                    server.temp3=(unsigned char)(rand()%40);
                }
                send(connfd,&server,sizeof(server),0);
                memset(&server,0,sizeof(server));
                memset(&client,0,sizeof(client));
            }
            if(n<0)
                printf("%s\n","Read error");
            exit(0);
        }
        close(connfd);
    }
}
