SUBDIR = ./module/log ./module/tinyxml ./module/TCP ./module/oracle ./app/module_oracle ./app/module_tcp 
all:
	
	for subdir in $(SUBDIR); do \
		echo "===> Making  in $$subdir..."; \
		make -C $$subdir;\
	done;


clean:
	for subdir in $(SUBDIR); do \
		echo "===> Making  in $$subdir..."; \
		make clean -C $$subdir;\
	done;
