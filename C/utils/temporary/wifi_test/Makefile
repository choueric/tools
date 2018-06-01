wifitest:main.c wpa_ctrl.c wpa_ctrl.h wpa_cli.c wpa_cli.h comm.h
	arm-none-linux-gnueabi-gcc -Wall main.c wpa_ctrl.c wpa_cli.c wpa_ctrl.h comm.h wpa_cli.h -o wifitest
	cp wifitest /media/sf_tftp/

clean:
	rm -f wifitest
