# CPSC5042-Final
Seattle University, CPSC 5042 final project. Spring quarter 2020


## How to run

Compiled on cs1.seattleu.edu. Type `make` to compile then start the server. 
Type `./client 127.0.0.1 12124` to run the client. 

## To do
- [x] Send username/password from client
- [x] Read username/password from client and do basic string validation
- [ ] Figure out user data structure (currently, unordered hash map)
- [ ] Figure out user storage to disk
- [ ] Clean up RPC parsing (currently, lots of casting string to char array pointer)
- [ ] Parse the client response (currently, just printing and retunring a success)
- [ ] Modularize the code so it's not all running in main
- [ ] Clean up includes
- [ ] Separate out header file for additional function definitions