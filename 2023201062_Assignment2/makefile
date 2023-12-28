# target (when the command is executed this target is updated): dependencies (when dependencies changes run the command mentioned below)
# 	action (run this command)

output: main.o display.o cwd.o echo.o ls.o cd.o process.o pinfo.o search.o
	g++ main.o display.o cwd.o echo.o ls.o cd.o process.o pinfo.o search.o -o output

main.o: main.cpp display.h cwd.h
	g++ -c main.cpp

display.o: display.cpp display.h echo.o ls.o cd.o cwd.o process.o pinfo.o search.o
	g++ -c display.cpp

echo.o: echo.cpp echo.h
	g++ -c echo.cpp

ls.o: ls.cpp ls.h cwd.o
	g++ -c ls.cpp

cwd.o: cwd.cpp cwd.h
	g++ -c cwd.cpp

cd.o: cd.cpp cd.h
	g++ -c cd.cpp

process.o: process.cpp process.h
	g++ -c process.cpp

pinfo.o: pinfo.cpp pinfo.h
	g++ -c pinfo.cpp

search.o: search.cpp search.h
	g++ -c search.cpp

clean:
	rm *.o