# DockerPostgreSQLAuth

How to run:
First of all you need to download libpqxx to the project and connect libraries!
1. Open Docker
2. start cmd and go to project(cd)
3. write "docker-compose up -d"
4. start the project and use it

In general, this project only register new users with their passwords, then when user is loggining new table is creating with his login, there you can write anything you want;  
Also you can create a lot of users and tables for them;

connect pqxxlib's
HOW TO CONNECT:
1. start cmd and got to the path of the project
2. git clone https://github.com/microsoft/vcpkg.git

3. cd vcpkg && bootstrap-vcpkg.bat

4. vcpkg install libpqxx:x64-windows

5.  in project properties:
    - include  directories -> include
      ><img width="796" height="410" alt="image" src="https://github.com/user-attachments/assets/aeba6ffc-09bc-4b28-b311-141016675345" />

    - library directories -> lib
      ><img width="795" height="422" alt="image" src="https://github.com/user-attachments/assets/a19ba629-0564-4e17-8099-74905fa86c0c" />

    - dependences -> "pqxx.lib;libpq.lib;libcrypto.lib;libssl.lib;lz4.lib;zlib.lib;ws2_32.lib"
      ><img width="800" height="380" alt="image" src="https://github.com/user-attachments/assets/c3ee135e-801a-49a3-8a92-d30e6c8295f4" />



!!!!!IN GENERAL USE 20+ COMPILATION
> <img width="799" height="498" alt="image" src="https://github.com/user-attachments/assets/bdb80e67-f4d5-4e4f-8eee-652e023bca92" />




