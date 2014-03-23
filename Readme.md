A.单独的天气查询系统的客户端实现：
	主要功能：
		『1』查询某些城市当天的天气
		『2』查询某些城市三天内的天气
		『3』用户自定义查询某些城市的特定日期的天气
		『4』可重复查询
	运行方法：
		『1』运行平台linux_32；
		『2』利用终端进入程序代码所在的目录，执行make clean，然后make run即可直接运行。

B.天气查询系统的客户端和服务器实现：
	主要功能：
		『1』查询某些城市当天的天气
		『2』查询某些城市三天内的天气
		『3』用户自定义查询某些城市的特定日期的天气
		『4』可重复查询
	服务器：基于查询系统的功能实现，能够多线程运行。	
	运行方法：
		客户端：
			『1』运行平台linux_32；
			『2』利用终端进入程序代码所在的目录，执行make clean，然后make，然后./weather_client 主机的ip地址 ，即可直接运行。
		服务器：
			『1』运行平台linux_32；
			『2』利用终端进入程序代码所在的目录，执行make clean，然后make run即可直接运行。