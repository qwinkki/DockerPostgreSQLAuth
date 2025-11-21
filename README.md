# DockerPostgreSQLAuth

How to run:
First of all you need to download libpqxx to the project and connect libraries!
1. Open Docker
2. Open PgAdmin 4
3. Create database in PgAdmin: AuthTest
4. start cmd and go to project(cd)
5. write "docker-compose up -d"
6. start the project and use it

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
    - library directories -> lib
    - dependences -> "pqxx.lib;libpq.lib;libcrypto.lib;libssl.lib;lz4.lib;zlib.lib;ws2_32.lib;"


!!!!!IN GENERAL USE 20+ COMPILATION



