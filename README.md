# Final-Project-COMP310-Fall-2019

                          
                          
                          How to run our Socket Programming Project:

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
                   
                




