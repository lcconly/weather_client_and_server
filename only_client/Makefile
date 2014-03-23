weather_client:weather_client.o
	gcc -o weather_client weather_client.o

weather_client.o:weather_client.c weather_data_struct.h
	gcc -c weather_client.c

.PHONY: run clean
 
run:weather_client
	./weather_client

clean:
	rm weather_client.o weather_client
