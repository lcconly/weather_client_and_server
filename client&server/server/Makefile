weather_server:weather_server.o
	gcc -o weather_server weather_server.o

weather_client.o:weather_server.c weather_data_struct.h
	gcc -c weather_server.c

.PHONY: run clean

run:weather_server
	./weather_server

clean:
	rm weather_server.o weather_server
