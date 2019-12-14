 # Title: Socket Programming For Weather & Stocks
 # Team Members: Vimal Sekhar & Irfan Poljak
 # Summary: Socket programming is a way of connecting two nodes on a particular network to communicate with one another. As one node  (socket) listens to a particular port, the other node (socket) reaches out to the other to form a connection. The server forms the listener socket as opposed to the client which reaches out to the server. Typically, such programs are used to communicate between various different processes running on different systems. The essential purpose is to create a client-server environment. Regarding the client program, when it is started, it will provide a command line interface to the user and will ask whether it is weather or stock request. Then it will make a connection to the server (based off the port number) and send the request in a pre-defined format which the server understands. The client will then wait for the response and when the response comes from the server, it displays the response message in the terminal window. It will then ask/wait for the next command/request from the user and repeat the same process (until the user asks to quit the program). So, the client can run on the same computer where multiple terminals can be started for the clients to simulate multiple client communication. Or, if another computer is on the same network that can access the server IP, then we can run the program in the same way. This is essentially the idea of socket-programming (our project).

                          
                 ----------------- Socket Programming Project Instructions:----------- 

            1). Clone/Download the entire repository:
                  - This will include both folders (Client and Server).

            2). To run the program:
                  - Step 1: Open 2 different terminal / exe windows.
                  - Step 2: On the first terminal window, you will run the SERVER. On the command line, do the following:
                  
                          - Step 2a: Run " cmake . " on the command line
                          - Step 2b: Run " cmake --build ." on the command line
                          - Step 2c: Run " ./runserver W/S [<port_number>] "
                          
                                    Example:  " ./runserver W 1030  "
                                    
                                                where W can be changed with S.
                                                where 1030 is optional (port 1027 is default).
                                                
                 - Step 3: On the second terminal window, you will run the CLIENT. On the command line, do the following:
                 
                          - Step 3a: Run " cmake . " on the command line
                          - Step 3b: Run " cmake --build ." on the command line
                          - Step 3c: Run " ./runclient <client-name> W/S [<port_number>] "
                          
                                    Example:  " ./runclient george W 1030 "
                                                
                                                where george is the client-name.
                                                where W can be changed with S. 
                                                where 1030 is optional (port 1027 is default).
                
                  NOTE:
                       Basically,the W/S are options for starting the WEATHER SERVER or STOCK SERVER
                          - Make sure to RUN SERVER FIRST, then RUN CLIENT (since server listens for connections). 
                          - Please enter "W" if you would like to start the WEATHER SERVER --> "./runserver W"
                          - Otherwise, enter "S"   -->   " ./runserver S "
                          - The final parameter is the port number (which is optional). If you do not specify
                            a port number, it will use the default port number as 1027. 
                   
                




