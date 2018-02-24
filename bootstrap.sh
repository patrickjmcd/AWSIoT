sudo apt-get update
sudo apt-get install -y gcc build-essential cmake git
git clone https://github.com/kyle-github/libplctag.git
cd libplctag
mkdir -p build
cd build
cmake ..
make
make install
echo "LD_LIBRARY_PATH=/usr/local/lib" | sudo tee --append /etc/environment
export LD_LIBRARY_PATH=/usr/local/lib
cd /vagrant/src
g++ testPJM.cpp -lplctag -lpthread -o test
echo "------------------------------------"
echo "Waiting 5 seconds to try the test..."
echo "------------------------------------"
sleep 5
./test -p
