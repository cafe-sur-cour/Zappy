FROM jenkins/jenkins:lts-jdk17

# Désactiver l'assistant de configuration initial
ENV JAVA_OPTS="-Djenkins.install.runSetupWizard=false"

# Copier la liste des plugins dans le répertoire de référence de Jenkins
COPY plugins.txt /usr/share/jenkins/ref/plugins.txt

# Installer les plugins Jenkins
RUN jenkins-plugin-cli --plugin-file /usr/share/jenkins/ref/plugins.txt

# Passer en utilisateur root
USER root

# Créer des emplacements pour les scripts de vérification de style
RUN mkdir -p /usr/local/bin

# Installer les dépendances système
RUN apt-get update && apt-get install -y \
    git \
    gcc \
    g++ \
    make \
    cmake \
    python3 \
    python3-pip \
    gcovr \
    libcriterion-dev \
    libgtest-dev \
    libgl1-mesa-dev \
    libx11-dev \
    libxcursor-dev \
    libxrandr-dev \
    libxi-dev \
    libxinerama-dev \
    libxxf86vm-dev \
    libwayland-dev \
    libxkbcommon-dev \
    wayland-protocols \
    xvfb \
    libasound2-dev \
    libopenal-dev \
    libopengl-dev \
    liballegro5-dev \
    valgrind && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# Installer les dépendances Python pour les vérificateurs de style
RUN pip3 install pylint flake8 pycodestyle cpplint

# Cloner et compiler raylib avec -fPIC et sans audio
RUN git clone https://github.com/raysan5/raylib.git /tmp/raylib && \
    cd /tmp/raylib && mkdir build && cd build && \
    cmake -DSHARED=ON -DCMAKE_POSITION_INDEPENDENT_CODE=ON -DSUPPORT_FILEFORMAT_WAV=OFF -DSUPPORT_AUDIO=OFF .. && \
    make -j$(nproc) && \
    make install && \
    rm -rf /tmp/raylib

# Revenir à l'utilisateur jenkins
USER jenkins
