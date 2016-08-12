export PLATFORM=_GZ_PLATFORM_
all:
	@echo "+++++++++++++++////// PLATFORM: ${PLATFORM} //////+++++++++++++"
	make -f MakefileBuild
	@echo ""
	rm -rf tdsbin tdsbin.tar.gz
	mkdir -pv tdsbin tdsbin/dabigate tdsbin/forwardserver tdsbin/trapapp
	
	cp -rf app/script tdsbin/
	
	cp -f app/dabigate/dabigate tdsbin/dabigate
	cp -f app/dabigate/dabigateConfig.xml tdsbin/dabigate
	
	cp -f app/forwardserver/fwdserver tdsbin/forwardserver
	cp -f app/forwardserver/fwdserverConfig.xml tdsbin/forwardserver
	
	cp -f app/trapapp/trapapp tdsbin/trapapp
	cp -f app/trapapp/trapappConfig.xml tdsbin/trapapp
	
	sleep 1
	tar -zcvf tdsbin.tar.gz tdsbin/
	
clean:
	rm -rf tdsbin tdsbin.tar.gz
	make -f MakefileBuild clean