CC		= gcc
CFLAGS	= -Wall -g
SLIBS	= -lwiringPi

CORE_DIR	= core
CORE_OBJS	=	$(CORE_DIR)/flowmeter.o \
				$(CORE_DIR)/reset_screen.o \
				$(CORE_DIR)/draw.o

BMP_DIR		= libbmp
BMP_OBJS	= $(BMP_DIR)/libbmp.o

UTILS_DIR	= utils
UTILS_OBJS	=	$(UTILS_DIR)/utilities.o \
				$(UTILS_DIR)/device_count.o \
				$(UTILS_DIR)/add_device.o \
				$(UTILS_DIR)/create_device.o \
				$(UTILS_DIR)/valid_device_type.o \
				$(UTILS_DIR)/print_type.o \
				$(UTILS_DIR)/pin_available.o

STARTUP_DIR		= startup
STARTUP_OBJS	=	$(STARTUP_DIR)/screen_setup.o \
					$(STARTUP_DIR)/set_pins.o \
					$(STARTUP_DIR)/set_images.o

MISC_DIR		= misc
MISC_OBJS		= $(MISC_DIR)/logger.o

OBJS		=	ecu.o $(CORE_OBJS) \
				$(BMP_OBJS) \
				$(UTILS_OBJS) \
				$(STARTUP_OBJS) \
				$(MISC_OBJS)

ecu: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(SLIBS)

clean:
	rm -f *.o \
		$(CORE_DIR)/*.o \
		$(BMP_DIR)/*.o \
		$(UTILS_DIR)/*.o \
		$(STARTUP_DIR)/*.o \
		$(MISC_DIR)/*.o
