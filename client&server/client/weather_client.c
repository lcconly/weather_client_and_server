/*************************************************************************
	> File Name: weather_client.c
	> Author: liucheng
	> Mail: 1035377294@qq.com 
	> Created Time: Mon 03 Mar 2014 04:40:28 AM PST
 ************************************************************************/
//基本框架参见书150页
#include"weather_data_struct.h"

/*主窗口显示*/
void show_main_window(){
    printf("Welcome to NJUCS Weather Forecast Demo Program!\n");
    printf("Please input City Name in Chinese pinyin<e.g. nanjing or beijing>\n");
    printf("<c>cls,<#>exit\n");
}

/*显示选项窗口显示*/
void show_choose_window(){
    system("clear");
    printf("Please enter the given number to query\n");
    printf("1.today\n");
    printf("2.three day from today\n");
    printf("3.custom day by yourself\n");
    printf("<r>back,<c>cls,<#>exit\n");
    printf("===================================================\n");
}

/*根据weather_id获取相应的天气描述*/
void show_weather_type(int weather_id){
    switch(weather_id){
        case 0x00:printf(" shower;");break;
        case 0x01:printf(" clear;");break;
        case 0x02:printf(" cloudy;");break;
        case 0x03:printf(" rain;");break;
        case 0x04:printf(" fog;");break;
        default :printf("fault!");break;
    }
}

/*选项一的天气状态显示*/
void show_weather_if_choose_1(struct server_data server){
    printf("City:%s Today is:%d/%02d/%02d Weather information is as follows:\n",server.city,ntohs(server.year),server.month,server.day);
    printf("Today's weather is:"); 
    show_weather_type(server.weather_id1);
    printf(" Temp:%02d\n",server.temp1);
}

/*选项二的天气状态显示*/
void show_weather_if_choose_2(struct server_data server){
    printf("City:%s Today is:%d/%02d/%02d Weather information is as follows:\n",server.city,ntohs(server.year),server.month,server.day);
    printf("The 1th day's weather is:"); 
    show_weather_type(server.weather_id1);
    printf(" Temp:%02d\n",server.temp1);//第一天

    printf("The 2th day's weather is:"); 
    show_weather_type(server.weather_id2);
    printf(" Temp:%02d\n",server.temp2);//第二天
   
    printf("The 3th day's weather is:"); 
    show_weather_type(server.weather_id3);
    printf(" Temp:%02d\n",server.temp3);//第三天

}

/*选项三的天气状态显示*/
void show_weather_if_choose_3(struct server_data server){
    if(server.type[0]==0x02)
		printf("Sorry, no given day's weather infomation for city %s!\n", server.city);
    else if(server.type[0]==0x01){
		printf("City:%s Today is:%d/%02d/%02d Weather information is as follows:\n",server.city,ntohs(server.year),server.month,server.day);
        printf("The %dth day's weather is:",server.date); 
		show_weather_type(server.weather_id1);
		printf(" Temp:%02d\n",server.temp1);
    }
}

/*选择窗口的逻辑代码*/
int function_choice(int sockfd,struct client_data client,struct server_data server,char *city){
    //printf("memset\n");
    memset(&client,0,sizeof(client));
    memset(&server,0,sizeof(server));
    strcpy(client.city,city);
	client.type[0]=0x01;
    send(sockfd,&client,sizeof(client),0);
    //printf("before recv\n");
    if(recv(sockfd,&server,sizeof(server),0)<0){
        printf("Data Receive Error!!!\n");
        return 0;
    }
    //printf("after recv\n");
    if(server.type[0]==0x04){
        //printf("type\n");
        printf("Sorry, Server does not have weather information for city %s!\n", client.city);
        return 0;
    }
    else if(server.type[0]==0x03){
        char func='c';
		char functemp[30];
        while(1){
            //printf("while\n");
			memset(&functemp,0,sizeof(functemp));
            //memset(&functemp,0,sizeof(functemp));
			if(func=='r'){
                system("clear");
                return 0;
            }
            switch(func){
                case 'c':show_choose_window();break;
                case '#':return 1;
                case '1':{
                    client.type[0]=0x02;
                    client.type[1]=0x01;
                    client.date=0x01;
                    send(sockfd,&client,sizeof(client),0);
                    if(recv(sockfd,&server,sizeof(server),0)<0){
                        printf("Data Receive Error!!!\n");
                        return 0;
                    }
					if(server.type[0]==0x01&&server.type[1]==0x41)
						show_weather_if_choose_1(server);
                    break;
                }
                case '2':{
                    client.type[0]=0x02;
                    client.type[1]=0x02;
                    client.date=0x03;
                    send(sockfd,&client,sizeof(client),0);
                    if(recv(sockfd,&server,sizeof(server),0)<0){
                        printf("Data Receive Error!!!\n");
                        return 0;
                    }
					if(server.type[0]==0x01&&server.type[1]==0x42)
						show_weather_if_choose_2(server);
                    break;
                }
                case '3':{
                    int num=0;
                    char temp[2];
					//scanf("%d",&num);
                    while(num>10 || num<1) {
                        printf("Please enter the day number(below 10, e.g. 1 means today):");
                        scanf("%2s%*[^\n]", &temp);
						num=atoi(temp);
                    }
                    client.type[0]=0x02;
                    client.type[1]=0x01;
                    client.date=(char)num;
                    send(sockfd,&client,sizeof(client),0);
                    if(recv(sockfd,&server,sizeof(server),0)<0){
                        printf("Data Receive Error!!!\n");
                        return 0;
                    }
                    show_weather_if_choose_3(server);
                    break;
                }
                default:printf("input error!\n");break;
            }
			//scanf("%s",&functemp);
			//if(functemp[1]=='\0'){
			scanf("%30s",functemp);
			if(functemp[1]=='\0')
				func=functemp[0];
			//printf("%c",func);
			//}
			else
				func='q';
        }
    }
    return 0;
}

int main(int argc,char const *argv[]){
    int sockfd;
    struct client_data client;
    struct server_data server;
    struct sockaddr_in serveraddr;
    char city[30];
	if(argc!=2){
		perror("Usage:make run IP address of the server!!\n");
		exit(0);	
	}
    //memset(&city,0,sizeof(city));
    memset(&serveraddr,0,sizeof(serveraddr));
    serveraddr.sin_port=htons(6666);
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_addr.s_addr=inet_addr(argv[1]);
    
    if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0){
        printf("Socket Create Error!!!\n");
        return 0;
    }//创建socket
    
    if(connect(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr))==-1){
        printf("Server connect Error!!!\n");
        return 0;
    }//链接服务器
    system("clear");
    while(1){
        memset(&city,0,sizeof(city));
        //system("clear");
        show_main_window();
        //printf("main\n");
        scanf("%30s%*[^\n]",city);
        if(city[0]=='c'&&city[1]=='\0'){
            system("clear");
			memset(&city,0,sizeof(city));
		}
        else if(city[0]=='#'&&city[1]=='\0'){
            system("exit");
            return 1;
        }    
        else{
            //printf("function\n");
            if(function_choice(sockfd,client,server,city)==1)
                return 1;//功能显示
            //else printf("func\n");
        }
    }
    return 1;
}

