SUBDIR = buffer concurrent exception internal io net util

all-recursive clean-recursive depend-recursive install-recursive :
	@target=`echo $@ | sed s/-recursive//`; \
	list='$(SUBDIR)'; \
	for subdir in $$list; do \
		echo "===> Making $$target in $$subdir..."; \
		(cd $$subdir && $(MAKE) $$target) \
	done;
 
all:	all-recursive

clean:	clean-recursive

depend: depend-recursive

install: install-recursive copy-file