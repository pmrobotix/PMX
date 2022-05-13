#! /bin/bash

#Se connecter une première fois avec ssh robot@[IP] pour enregistrer le mot de passe de la connexion ssh.
#EV3 CHO
IPUSB="10.42.0.199" #USB LINUX
#IPUSB="169.254.230.22" #USB LINUX
#IPWIFI="192.168.2.102" #WIFI anciennce clef wifi
#IPHOME="192.168.0.219" #WIFI HOME CHO INDEED2G anciennce clef wifi
IPWIFI="192.168.2.104"
IPHOME="192.168.0.123" #WIFI HOME CHO INDEED2G
#IPHOME="192.168.0.220" #WIFI HOME CHO INDEED
#IP5GHZ="192.168.3.102" #WIFI1 PMX5G 
IP5GHZ="192.168.4.104" #WIFI2 PMX5G 
#IP5GHZ="192.168.4.101" #WIFI2 PMX2G 



all()
{
	scp "../Bot_LegoMindstorm_EV3/Release/Bot_LegoMindstorm_EV3" "../Driver-EV3-UnitTest/Debug/Driver-EV3-UnitTest" robot@$IP:/home/robot/pmx
}
arm()
{
	ssh robot@$IP "killall Bot_LegoMindstorm_EV3";
	scp "../Bot_LegoMindstorm_EV3/Release/Bot_LegoMindstorm_EV3" "../src/Bot-LegoEV3/config_LegoEV3Robot.txt" robot@$IP:/home/robot/pmx
}
common()
{
	scp "../Common-UnitTest_EV3/Debug/Common-UnitTest_EV3" robot@$IP:/home/robot/pmx
}
driver()
{
	scp "../Driver-UnitTest_EV3/Debug/Driver-UnitTest_EV3" robot@$IP:/home/robot/pmx
}

driver-arm-8.3()
{
	scp "../Driver-UnitTest_EV3/Debug-gcc-arm-linux-gnueabihf-8.3/Driver-UnitTest_EV3" robot@$IP:/home/robot/pmx
}
arm-2014.05-29()
{
	ssh robot@$IP "killall Bot_LegoMindstorm_EV3";
	scp "../Bot_LegoMindstorm_EV3/Release-arm-2014.05-29/Bot_LegoMindstorm_EV3" "../src/Bot-LegoEV3/config_LegoEV3Robot.txt" robot@$IP:/home/robot/pmx
}
driver-arm-2014.05-29()
{
	scp "../Driver-UnitTest_EV3/Debug-arm-2014.05-29/Driver-UnitTest_EV3" robot@$IP:/home/robot/pmx
}
common-arm-2014.05-29()
{
	scp "../Common-UnitTest_EV3/Debug-arm-2014.05-29/Common-UnitTest_EV3" robot@$IP:/home/robot/pmx
}

tests()
{
	scp "../../ev3dev-lang-cpp/build/tests/api_tests" "../../ev3dev-lang-cpp/build/demos/button-test"  "../../ev3dev-lang-cpp/build/demos/ev3dev-lang-demo" "../../ev3dev-lang-cpp/build/demos/ev3dev-lang-test" "../../ev3dev-lang-cpp/build/demos/sound-test" robot@$IP:/home/robot/pmx	
}

recupsvg()
{
		#faire un repertoire de logs pour mutualiser la recup des logs csv et svg    
        scp "robot@$IP:/home/robot/pmx/*.svg" /home/pmx/recup 
}
recupcsv()
{
		#faire un repertoire de logs pour mutualiser la recup des logs csv et svg
        scp "robot@$IP:/home/robot/pmx/*.csv" /home/pmx/recup    
}

connect()
{
	echo $IP
	ssh -t robot@$IP "cd /home/robot/pmx ; exec \$SHELL -l"
}

case "$1" in
	5g)
		IP=$IP5GHZ
	;;
	usb)
		IP=$IPUSB
	;;
	wifi)
		IP=$IPWIFI
	;;
	
	home)
		IP=$IPHOME
	;;
	
	*)
		IP=$IPUSB
	;;
esac

case "$2" in
	view-doc)
		view_doc;;
	connect)
		connect;;
	all)
		all;;
	driver)
		driver;;
	common)
		common;;
	arm)
		arm;;
	arm-2014.05-29)
		arm-2014.05-29;;
	driver-arm-2014.05-29)
		driver-arm-2014.05-29;;
	common-arm-2014.05-29)
		common-arm-2014.05-29;;
	driver-arm-8.3)
		driver-arm-8.3;;
	svg)
		recupsvg;;
	csv)
		recupcsv;;
	tests)
		tests;;

	*)
		echo "Usage: $0 {5g|usb|wifi|home} {connect|all|arm|driver|common|arm-2014.05-29|driver-arm-8.3|csv|svg|tests}" >&2
		exit 1
	;;
esac

exit 0
