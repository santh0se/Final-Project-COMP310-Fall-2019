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
                
                  NOTE:
                       Basically,the W/S are options for starting the WEATHER SERVER or STOCK SERVER. 
                          - Please enter "W" if you would like to start the WEATHER SERVER --> "./runserver W"
                          - Otherwise, enter "S"   -->   " ./runserver S "
                          - The final parameter is the port number (which is optional). If you do not specify
                            a port number, it will use the default port number as 1027. 
                   
                




