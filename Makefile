KSRCDIR=/lib/modules/`uname -r`/source
KBINDIR=/lib/modules/`uname -r`/build
INSTALL_PATH=drivers/usb/misc
PWD = `pwd`
TVDIR=$(PWD)/.tmp_versions
K_TVDIR=$(KSRCDIR)/.tmp_versions
K_VFILES=$(wildcard $(K_TVDIR)/*.mod)

KMODSRC := yurex.c

KMODOBJ := $(KMODSRC:%.c=%.o)
KMOD_KO := $(KMODOBJ:%.o=%.ko)

ifneq ($(KERNELRELEASE),)

obj-m	:= $(KMODOBJ)

else

kmodules:
ifneq ($(strip $(KMOD_KO)), )
	@mkdir -p $(TVDIR)
ifneq (,$(K_VFILES))
	@ln -snf $(K_VFILES) $(TVDIR)
endif
	make -C $(KSRCDIR) O=$(KBINDIR) M=$(PWD)
endif

install :
	cp $(KMOD_KO) /lib/modules/`uname -r`/kernel/$(INSTALL_PATH)/
	/sbin/depmod `uname -r`

clean :
	rm -f *.ko *.o .?*.cmd ?*.mod.c core Module.* modules.*
	rm -rf $(TVDIR)

distclean: clean

endif
