FROM debian:buster

RUN apt-get update && apt-get install -y \
    gcc g++ make cmake libwxgtk3.0-dev libwxbase3.0-dev \
    wx3.0-headers wx-common libxml2-utils python \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /opt/kovel

CMD ["bash", "build_wx.sh"]