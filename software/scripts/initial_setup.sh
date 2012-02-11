# Updates
echo -e "Updating Modules\n"
opkg update
echo -e "Done\n\n"

# Deps
echo -e "Installing Dependencies\n"
opkg install ncurses python-dev python-modules python-setuptools wireless-tools zd1211-firmware
echo -e "Done\n\n"

# Vim
cd ~/
echo -e "set nu\nset ts=4\nsyntax on\n" > .vimrc 

# Tmp Setup
mkdir .init_tmp
cd .init_tmp

# Mercurial
echo -e "Setting Up Mercurial\n"
wget http://mercurial.selenic.com/release/mercurial-2.1-rc.tar.gz
tar xfvz mercurial-2.1-rc.tar.gz
cd mercurial-2.1-rc
python setup.py --pure install
cd ../
echo -e "Done\n\n"

# Hostapd
echo -e "Setting Up hostapd\n"
wget http://www.infradead.org/~tgr/libnl/files/libnl-1.1.tar.gz
tar xvzf libnl-1.1.tar.gz
cd libnl-1.1
patch -p1 < ../patches/libnl-netlink-local.patch
./configure
make
make install
cd ../
echo /usr/local/lib >> /etc/ld.so.conf
ldconfig
wget http://w1.fi/releases/hostapd-0.7.3.tar.gz
tar zxvf hostapd-0.7.3.tar.gz
cd hostapd-0.7.3/hostapd
cp defconfig .config 
patch -p0 < ../../patches/hostapd-config.patch
make
mkdir /usr/local/bin
make install
cd ../
echo -e "Done\n\n"

# Cleanup
echo -e "Cleaning up\n"
cd ../
rm -rf .init_tmp
echo -e "Done\n\n"

echo -e "Exiting\n\n"
