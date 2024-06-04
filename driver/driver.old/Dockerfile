FROM osrf/ros:foxy-desktop

RUN <<EOF
apt update
apt install python3 -y
apt install mosquitto mosquitto-clients -y
apt-get install -y python3 python3-pip
pip3 install paho-mqtt
EOF

ENV ROS_DOMAIN_ID=1

