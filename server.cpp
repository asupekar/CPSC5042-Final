#include <cstdio>

// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <iostream>
#include <unordered_map>
#include <fstream>
using namespace std;

// declarations
typedef unordered_map<string, string> useful_map;
useful_map readUsers();
char* connectRPC(char *, char *, useful_map);
useful_map charArrToHashMap(char *);

//#define PORT 8080
#define PORT 12124
int main(int argc, char const *argv[])
{
	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024] = { 0 };

	cout << "Server startup" << endl;
	useful_map userList = readUsers();

	// Creating socket file descriptor 
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	printf("Got Socket\n");
	// Forcefully attaching socket to the port
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
		&opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	// Forcefully attaching socket to the port
	printf("About to bind\n");
	if (bind(server_fd, (struct sockaddr *)&address,
		sizeof(address)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	printf("Waiting\n");
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
		(socklen_t*)&addrlen)) < 0)
	{
		perror("accept");
		exit(EXIT_FAILURE);
	}
	printf("Accepted Connected\n");

	valread = read(new_socket, buffer, 1024);
	printf("%s\n", buffer);
    
	// What RPC is it
	// Parse out char array
	// Call the correct RPC Function
    // if rpc = connect
	// quick and dirty way
	useful_map rpcCallRcvd = charArrToHashMap(buffer);
	char * output = new char[100];
	string rpc = rpcCallRcvd["rpc"];
	if (rpc.compare("connect") == 0) {
		//pass connecting info
		string un = rpcCallRcvd["username"];
		string pwd = rpcCallRcvd["password"];
		char * username = new char[un.length()];
		strcpy(username, un.c_str());
		char * password = new char[pwd.length()]; 
		strcpy(password, pwd.c_str());
		output = connectRPC(username, password, userList);
	}
	// if rpc = disconnect

	send(new_socket, output, strlen(output), 0);

	return 0;
}


//ADD FUNCTIONS
char* connectRPC(char *userName, char *password, useful_map ul) {
    // prepare return
	char* output = new char[30];
	string lookupUn = string(userName);
	// username validation
	try {
		ul.at(lookupUn);
		// validate passwords
		string pwd = ul[lookupUn];
		if (pwd.compare(string(password)) == 0) {
			//success
			strcpy(output, "status=1;error=Success;");
		} else {
			//error
			strcpy(output, "status=-1;error=BadPassword;");
		}
	} catch (...) {
		//error
		strcpy(output, "status=-1;error=BadUsername;");
	}

	return output;
}

// including a function to read/write user list to disk
// simple but users will persist between server launches
// load at server start
useful_map readUsers() {
	//commented out csv portion because it was reading the \n 
	// figure out a nicer data storage option
	// sqlite??
	/*//initialize
	useful_map users;
	//open the csv file with users info
	ifstream openFile("users.csv");
	//loop over and read to a hashmap
	if(openFile.is_open()) {
        while(true) {
            string key, value;
            if (!getline(openFile, key, ',')) break;
            //read value
            getline(openFile, value, '\n');
			//cout << key << ", " << value << endl;
            users[key] = value;
        }
		openFile.close();
    }*/
	useful_map users ({{"Naomi","p@ssword"},{"Ruifeng","p@ssword"},{"Aishwarya","p@ssword"}});
	return users;
}

// just adding for easy parsing
// but not practical since it casts back and forth with strings
useful_map charArrToHashMap(char * charArr) {
	const char delimiter[2] = ";";
	const char delimiter2[2] = "=";
	char* leftStr;
	char* rightStr;
	useful_map RPCCall;

	leftStr = strtok_r(charArr, delimiter2, &charArr);
	rightStr = strtok_r(charArr, delimiter, &charArr);
	RPCCall[string(leftStr)] = string(rightStr);
	int i = 0;
	while(leftStr != NULL && rightStr != NULL) {
		if (i % 2 == 0) {
			leftStr = strtok_r(charArr, delimiter2, &charArr);
		} else {
			rightStr = strtok_r(charArr, delimiter, &charArr);
			RPCCall[string(leftStr)] = string(rightStr);
		}
		i++;
	}

	return RPCCall;
}