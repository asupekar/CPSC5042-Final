all: compile_server compile_client run_server
compile_server: server.cpp
	g++ server.cpp -o server
compile_client: client.cpp
	g++ client.cpp -o client
run_server: server
	./server