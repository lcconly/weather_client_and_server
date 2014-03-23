/*************************************************************************
	> File Name: weather_data_struct.h
	> Author: liucheng
	> Mail: 1035377294@qq.com 
	> Created Time: Mon 03 Mar 2014 04:20:48 AM PST
 ************************************************************************/

#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<string.h>

struct client_data{
    unsigned char type[2];
    unsigned char city[30];
    unsigned char date;
};

struct server_data{
    unsigned char type[2];
    unsigned char city[30];
    unsigned short year;
    unsigned char month;
    unsigned char day;
    unsigned char date;
    unsigned char weather_id1;
    unsigned char temp1;
    unsigned char weather_id2;
    unsigned char temp2;
    unsigned char weather_id3;
    unsigned char temp3;
    unsigned char data[94];
};

