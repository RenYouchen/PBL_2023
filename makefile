all: main 

build:
	arduino-cli compile

upload:
	arduino-cli upload

monitor:
	arduino-cli monitor -p /dev/ttyACM0 -c baudrate=115200

main:
	arduino-cli compile -u
