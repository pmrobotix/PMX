#!/bin/bash
sudo systemctl stop brickman
sudo ./Driver-UnitTest_EV3 >/dev/tty0
sudo systemctl start brickman
