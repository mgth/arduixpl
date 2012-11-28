# location of the arduino install folder
ARDUINO_DIR = F:/DropBox/Dropbox/arduino-1.0.1

# name of this project (used for the output files)
TARGET = xPL_Lighting

# a list of the local project folders containing source
# files relative to this makefile
PROJECT_DIRS =	.				\
				mylib 

# libraries to include in this project,
# remove any you're not using
LIBRARIES = EEPROM				\
			Ethernet			\
			Ethernet/utility 	\
			Firmata				\
			LiquidCrystal		\
			SD					\
			SD/utility			\
			Servo				\
			SoftwareSerial		\
			SPI					\
			Stepper				\
			Wire				\
			Wire/utility		\
			xPL				\

# location of any other header files to include


# target folder location
OUTPUT_DIR = obj

# compiler flags for c and cpp files
CFLAGS = -c -g -Os -Wall -ffunction-sections -fdata-sections -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=100
CPPFLAGS = -fno-exceptions $(CFLAGS)

# linker flags
LINK_FLAGS = -Os -Wl,--gc-sections -mmcu=atmega328p -L$(OUTPUT_DIR) 

##### most projects shouldn't need to touch anything below this line #####

# location of various tools
BIN = $(ARDUINO_DIR)/hardware/tools/avr/bin
CORE_DIR = $(ARDUINO_DIR)/hardware/arduino/cores/arduino
VAR_DIR = $(ARDUINO_DIR)/hardware/arduino/variants/standard
LIBS_DIR = $(ARDUINO_DIR)/libraries
GCC = $(BIN)/avr-gcc
AR = $(BIN)/avr-ar
COPY = $(BIN)/avr-objcopy

# set up all the include folders
INCLUDES =	-I$(CORE_DIR) -I$(VAR_DIR)										\
			$(foreach lib,$(LIBRARIES), -I$(ARDUINO_DIR)/libraries/$(lib))	\
			$(foreach inc,$(PROJECT_DIRS), -I$(inc)/)						\
			$(foreach inc,$(USER_INCLUDES),-I$(inc))

# create a list of all the obj files to create for the core library
CORE_OBJS = $(addprefix $(OUTPUT_DIR)/, $(addsuffix .o, $(notdir $(wildcard $(CORE_DIR)/*.cpp) $(wildcard $(CORE_DIR)/*.c) )))
CORE_LIB = $(OUTPUT_DIR)/core.a

# create a list of all the obj files to create for the library folders
LIB_OBJS = $(foreach lib, $(LIBRARIES), $(addprefix $(OUTPUT_DIR)/$(lib)/, $(addsuffix .o, $(notdir $(wildcard $(LIBS_DIR)/$(lib)/*.cpp) $(wildcard $(LIBS_DIR)/$(lib)/*.c)))))

# create a list of all the obj files to create for the project folders
PROJECT_OBJS = $(foreach folder, $(PROJECT_DIRS), $(addprefix $(OUTPUT_DIR)/$(folder)/, $(addsuffix .o, $(notdir $(wildcard $(folder)/*.cpp) $(wildcard $(folder)/*.c)))))

# create a list of dependency files (one for each project obj file)
DEPENDENCIES = $(PROJECT_OBJS:.o=.d)

# main build target
.PHONY: all
all: $(OUTPUT_DIR)/$(TARGET).hex
	@echo Finished building $<
	@echo
	
# rule to build the hex file
$(OUTPUT_DIR)/$(TARGET).hex: $(OUTPUT_DIR)/$(TARGET).elf
	@mkdir -p $(dir $@)
	@echo "Copying $@"
	@$(COPY) -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0 $(OUTPUT_DIR)/$(TARGET).elf $(OUTPUT_DIR)/$(TARGET).eep 
	@$(COPY) -O ihex -R .eeprom $(OUTPUT_DIR)/$(TARGET).elf $(OUTPUT_DIR)/$(TARGET).hex 

# rule to build the elf file
$(OUTPUT_DIR)/$(TARGET).elf: $(CORE_LIB) $(LIB_OBJS) $(PROJECT_OBJS)
	@echo "Linking $@"
	@$(GCC) $(LINK_FLAGS) -o $(OUTPUT_DIR)/$(TARGET).elf $(LIB_OBJS) $(PROJECT_OBJS) $(CORE_LIB) -lm 

# rule to build the core.a library file	
$(CORE_LIB): $(CORE_OBJS)
	@for core_file in $(CORE_OBJS) ; do 		\
		echo Storing $$core_file ;				\
		$(AR) rcs $(CORE_LIB) $$core_file ;		\
	done
	
#include the dependencies, if they exist	
-include $(DEPENDENCIES)

# rule to build the project .cpp files
$(OUTPUT_DIR)/%.cpp.o: %.cpp
	@mkdir -p $(dir $@)
	@echo "Compiling $<"
	@$(GCC) $(CPPFLAGS) -I$(CORE_DIR) -I$(VAR_DIR) $(INCLUDES) -MMD $< -o $@
	
# rule to build the project .c files
$(OUTPUT_DIR)/%.c.o: %.c
	@mkdir -p $(dir $@)
	@echo "Compiling $<"
	@$(GCC) $(CFLAGS) -I$(CORE_DIR) -I$(VAR_DIR) $(INCLUDES) -MMD $< -o $@

# rule to build the core .cpp files
$(OUTPUT_DIR)/%.cpp.o: $(CORE_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@echo "Compiling $<"
	@$(GCC) $(CPPFLAGS) -I$(CORE_DIR) -I$(VAR_DIR) $(INCLUDES) $< -o $@
	
# rule to build the core .c files
$(OUTPUT_DIR)/%.c.o: $(CORE_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "Compiling $<"
	@$(GCC) $(CFLAGS) -I$(CORE_DIR) -I$(VAR_DIR) $(INCLUDES) $< -o $@
	
# rule to build the library source files
.SECONDEXPANSION:
LIB_SRC = $(LIBS_DIR)/$(basename $(subst $(OUTPUT_DIR)/,,$@))
$(LIB_OBJS): $$(LIB_SRC)
	@mkdir -p $(dir $@)
	@echo "Compiling $<"
	@$(GCC) $(CPPFLAGS) -I$(CORE_DIR) -I$(VAR_DIR) $(INCLUDES) $< -o $@

clean:
	@echo Cleaning...
	@-rm -f $(OUTPUT_DIR)/$(TARGET).eep
	@-rm -f $(OUTPUT_DIR)/$(TARGET).hex
	@-rm -f $(OUTPUT_DIR)/$(TARGET).elf
	@-rm -f $(CORE_LIB) $(CORE_OBJS) $(LIB_OBJS) $(PROJECT_OBJS) $(DEPENDENCIES)

rebuild: clean all
