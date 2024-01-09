## Guide
# Run set up enviroment
```
./setup-env.sh
```
This will clone luajit-2.0

# Config connection
Change the listen port and backend port (remember that ReactJS and NodeJS servers are backend, this socket connection will run on listen port)

# Run
```
cd src
./rsp {path to the .lua config file}
```
You need to run "rsp" twice because you need to connect to both ReactJS and NodeJS serve

# Reference
https://www.gilesthomas.com/2013/08/writing-a-reverse-proxyloadbalancer-from-the-ground-up-in-c-part-1
