exo1 : src/pipe.cpp
	g++ -o pipe src/pipe.cpp -I . -Wall -std=c++1y 

exo2 : src/main.cpp 
	g++ -g -o exo2 $^  src/Stack.h -I . -Wall -std=c++1y

exo3 : src/serveur.cpp src/client.cpp 
	g++ -g -o serveur src/serveur.cpp src/chat_common.h -I . -Wall -std=c++1y -lpthread
	g++ -g -o client src/client.cpp src/chat_common.h -I . -Wall -std=c++1y -lpthread