# QUARCS_stellarium-web-engine

<img src="https://github.com/QHYCCD-QUARCS/QUARCS_stellarium-web-engine/assets/158538628/7ad24990-f259-4396-bb94-e56f6f94abc3" width="900">

## Features

- Planned task table.
- Autofocus function.
- Improve the configuration page of the device.
- State recovery after client disconnection and reconnection.
- Southern polar axis.
- Target search in star maps.
- Obtain status feedback (debug information) from the server.
- Some improvements in interaction logic.

## Ubuntu 24.04.1 LTS SERVER

<img align="left" src="https://www.raspberrypi.com/app/uploads/2020/06/raspberrry_pi_logo.png" width="48">

These instructions are written for **Ubuntu 24.04.1 LTS Server (64-bit) ARM** for **Raspberry Pi Models** Please make sure you have already installed the **QUARCS_QT-SeverProgram** followed by **QUARCS_PHD2** or your install will not work correctly.

---

## 1. Install EMSDK
**Install EMSDK Prerequisites**
Run the following command to install prerequisites for EMSDK:

    sudo apt-get install emscripten xz-utils python3 python3-venv clang scons libcurl4-openssl-dev -y
    systemctl daemon-reload

**Install EMSDK**  
To install the EMSDK:

    cd ~    
    git clone --branch 3.1.64 https://github.com/emscripten-core/emsdk.git 
    cd emsdk 
    ./emsdk install 3.1.64
    ./emsdk activate 3.1.64
    source ./emsdk_env.sh 

**Configure EMSDK Enviormental Varibles**  

    echo 'source "$HOME/emsdk/emsdk_env.sh"' >> "$HOME/.bashrc" 
    source "$HOME/.bashrc" 

**Verification**  
To confirm EMSDK installation, check the installed version:

	emcc --version 

You should see **emcc (Emscripten gcc/clang-like replacement + linker emulating GNU ld) 3.1.64 (...)** displayed in the terminal.

---

## 2. Install SG4P
To install SG4P and the needed library files, execute the following commands:

    cd ~ 
    git clone --branch RPi4_5 https://github.com/joeytroy/sgp4.git 
    cd sgp4 
    mkdir build 
    cd build 
    cmake -DCMAKE_INSTALL_PREFIX=/usr/local .. 
    make -j$(nproc) 
    sudo make install 

**Verification**  
To confirm SG4P installation, check the for the proper libraries version:

    ls /usr/local/lib | grep libsgp4 
    ls /usr/local/include/libsgp4 

You should see **libsgp4.a** and **libsgp4s.so**, along with header files like **Tle.h** and **Observer.h**.

---

## 3. Install Stellarium Web
To install the Stellarium Web software:

    cd ~ 
    git clone --branch RPi4_5 https://github.com/joeytroy/QUARCS_stellarium-web-engine 
    cd QUARCS_stellarium-web-engine 
    export EMSCRIPTEN_TOOL_PATH=~/emsdk/upstream/emscripten 
    scons 

## 4. Install nodejs, npm
### Delete old packages to avoid any impact on the future 

    sudo apt-get remove nodejs npm -y

### Install the version manager for nvm and nodejs

    curl -o- https://raw.githubusercontent.com/nvm-sh/nvm/v0.39.2/install.sh | bash 
    source ~/.bashrc 
    nvm install 14 

### Check if nvm has been successfully installed

    node -v
    npm -v

You should see Node.js **v14.x.x** and nmp **6.x.x**

## 5. Install Docker
### Install Docker Prerequisites:

    sudo apt-get install curl apt-transport-https ca-certificates software-properties-common -y 
    curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add - 
    sudo add-apt-repository "deb [arch=arm64] https://download.docker.com/linux/ubuntu $(lsb_release -cs) stable" 
    
        
### Install Docker:

    sudo apt update && sudo apt install docker-ce -y 

### Verify Docker installation:

    sudo systemctl status docker 

You can press **ctrl+c** to close out of the docker prompt

### Start Docker (if needed):

    sudo systemctl start docker 

## 6. Build and Run the Project
### Navigate to the Frontend Directory 

    cd ~/QUARCS_stellarium-web-engine/apps/web-frontend
    sudo su

### Build Docker Images
If done correctly you should be running as **root@** user

    make clean
    make setup

### Start the Development Server

    make dev

## 7. Start Quarcs Stellarium Web Engine

To start the project, run in the apps/web-frontend directory:

    cd ~/QUARCS_stellarium-web-engine/apps/web-frontend
    sudo su
    sh kill_run.sh

## Contributing

In order for your contribution to Stellarium Web Engine to be accepted, you have to sign the
[QUARCS_Stellarium Web Contributor License Agreement (CLA)](doc/cla/sign-cla.md).

Welcome to join the QUARUS Discod discussion group for online discussions (<https://discord.gg/uHTPfJ5uuV>)
