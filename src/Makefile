TARGET		= aurora
OBJS		= main.o lib/common/callback.o lib/glib2d/glib2d.o lib/aurora/aurora.o lib/libcsv/libcsv.o

INCDIR		=
CFLAGS		= -G0 -Wall -O2
CXXFLAGS	= $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS		= $(CFLAGS)

LIBDIR		=
LDFLAGS		=
LIBS		= -ljpeg -lpng -lz -lpspgum -lpspgu -lpsprtc -lm -lpspvram

BUILD_PRX = 1

EXTRA_TARGETS	= EBOOT.PBP
PSP_EBOOT_TITLE	= aurora

PSP_FW_VERSION = 660

PPSSPPSDL = /Applications/PPSSPPSDL.app/Contents/MacOS/PPSSPPSDL

PSPSDK	= $(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak

ifeq ($(BUILD_PRX),1)
    COPY_PRX = @cp $(TARGET).prx ../bin/$(TARGET).prx
    RM_PRX = @rm -f ../bin/$(TARGET).prx
else
    COPY_PRX = @echo "No .prx to copy\n"
    RM_PRX = @echo "No .prx to remove\n"
endif

normal:
	@make
	@rm -f ../bin/$(TARGET)/$(EXTRA_TARGETS)
	@$(RM_PRX)
	@cp EBOOT.PBP ../bin/$(TARGET)/$(EXTRA_TARGETS)
	@$(COPY_PRX)
	@make clean

run:
	@$(PPSSPPSDL) ../bin/$(TARGET)/$(EXTRA_TARGETS) 2>&1 >/dev/null | grep --line-buffered -e '-->'
	#@$(PPSSPPSDL) ../bin/$(TARGET)/$(EXTRA_TARGETS)
