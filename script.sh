#!/bin/bash
mkdir POCO
mkdir POCO_SERVER
mv ./poco_server.cpp ./POCO_SERVER
cd ./POCO
wget https://pocoproject.org/releases/poco-1.13.0/poco-1.13.0-all.tar.gz
tar xvf *.tar.gz
sudo apt-get install libpoco-dev
sudo apt-get install openssl libssl-dev
sudo apt-get install libiodbc2 libiodbc2-dev
sudo apt-get install libmysqlclient-dev
cd ./poco-1.13.0-all
sudo ./configure --no-sqlparser --no-tests
sudo make
sudo make install
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib
echo 'export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib' >> ~/.bashrc
cd ../../POCO_SERVER
g++ ./poco_server.cpp -L/usr/local/lib -lPocoFoundation -lPocoNet -lpthread -o EXE
sudo rm -r ../POCO
