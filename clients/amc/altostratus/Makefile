# ============================================================================
#
#       java dev tools 
#	This Makefile will make AltoStratus.jar
#	The intended directory structure is:
#		images/
#		intermediates/
#		Makefile
#		run_altostratus
#		src/
#			amc/
#			ObserveM/
#			:
#		xerces/
#
# ============================================================================
# 
# Primitives to tell make about java
# DO NOT EDIT
# 

# set a makefile variable to be the list of directories where we might have installed java classes
#CLASS_DIRS      = $(shell find $(JAVA_CLASS_DIR) -type d -printf %p\: )

#VPATH =  $(CLASS_DIRS):$(SRC_DIR)
VPATH =  $(SRC_DIR)

.SUFFIXES: .java .class .o .c .y .l .s 

.java.class:
	echo $? >> $(JC_ARGS_FILE)

.class.h:
	$(JAVAH) $(JFLAGS) $*

.class.c:
	$(JAVAH) $(JFLAGS) -stubs $*

JC_ARGS_FILE    = .jc_depend
#JAVA_HOME		= /home/java/bin
JAVA_HOME		= /usr/bin
JAVAC           = $(JAVA_HOME)/javac
JAVAH           = $(JAVA_HOME)/javah
JAVADOC			= $(JAVA_HOME)/javadoc
JAVA_CLASS_DIR  = ./intermediates
JAVA_CLASS_PATH	= $(JAVA_CLASS_DIR):./xerces/xerces.jar
JAVA_MERGE		= ./xerces/xerces.jar


# JFLAGS_OPTIONAL	= -deprecation		# Turn this on to see Deprecated warnings
JFLAGS          = -classpath $(JAVA_CLASS_PATH):$(CLASSPATH) -d $(JAVA_CLASS_DIR) $(JFLAGS_OPTIONAL)
DEBUG_FLAGS	= -g
#
JAR_FILE = AltoStratus.jar
JAR 	= jar
# JAR_FLAGS = -cvfm
JAR_FLAGS = -cfm
JAR_MANIFEST	= AltoStratus.mf

UNZIP 		= jar
UNZIP_FLAGS	= -xf

SRC_DIR		= ./src
INSTALL_DIR 	= ./build

HTML_FILE	= altostratus.html

IMAGE_SRC_DIR 	= images
IMAGE_SRC_FILES	= $(IMAGE_SRC_DIR)/*.gif
IMAGE_DEST_DIR 	= $(JAVA_CLASS_DIR)/$(IMAGE_SRC_DIR)

UNPACKED_TEST	= .unpacked

#
# ============================================================================
# DEFS
#

# don't include appleshare files
J_SRC           = $(shell find $(SRC_DIR) -name "*.java" | grep -v AppleDouble )
J_OBJS          = $(shell find $(SRC_DIR) -name "*.java" | grep -v AppleDouble | awk -F. '{print "."$$1$$2".class"}')
CLEAN_OBJS      = $(shell find $(JAVA_CLASS_DIR) -name "*.class" )
TEMP_OBJS	= $(shell find . -name "*~") 
#
# ============================================================================
# TARGETS

all: $(JAR_FILE)

# ============================================================================

$(JAVA_CLASS_DIR)/$(UNPACKED_TEST): $(JAVA_CLASS_DIR)
	cd $(JAVA_CLASS_DIR); touch $(UNPACKED_TEST)

# !!! this copies files all the time, there should probably be a rule
unpack: $(JAVA_CLASS_DIR) $(JAVA_CLASS_DIR)/$(UNPACKED_TEST)
	cd $(JAVA_CLASS_DIR); $(UNZIP) $(UNZIP_FLAGS) ../$(JAVA_MERGE)
	mkdir -p $(IMAGE_DEST_DIR)
	cp $(IMAGE_SRC_FILES) $(IMAGE_DEST_DIR)

$(JAR_FILE): classes
	@- echo 
	cd $(JAVA_CLASS_DIR); rm -f $(JAR_FILE)
	cd $(JAVA_CLASS_DIR); $(JAR) $(JAR_FLAGS) $(JAR_FILE) ../$(JAR_MANIFEST) *

classes: $(JAVA_CLASS_DIR) JavaPrep $(J_OBJS) unpack
	@- echo
	@- echo "making class files"
	$(JAVAC) $(JFLAGS) $(shell cat $(JC_ARGS_FILE)) 

debug_classes: $(JAVA_CLASS_DIR) JavaPrep $(J_OBJS) unpack
	@- echo
	@- echo "making class files"
	$(JAVAC) $(JFLAGS) $(DEBUG_FLAGS) $(shell cat $(JC_ARGS_FILE)) 

debug_jarfile: debug_classes
	@- echo 
	cd $(JAVA_CLASS_DIR); rm -f $(JAR_FILE)
	cd $(JAVA_CLASS_DIR); $(JAR) $(JAR_FLAGS) $(JAR_FILE) *

$(INSTALL_DIR): 
	@- echo "making install directory" ;
	@- mkdir $(INSTALL_DIR); 

$(JAVA_CLASS_DIR): 
	@- echo "making java class directory" ;
	@- mkdir $(JAVA_CLASS_DIR); 


JavaPrep: 
	@- echo 
	@- echo "nuking $(JC_ARGS_FILE)"
	@- rm -f $(JC_ARGS_FILE)
	@- touch $(JC_ARGS_FILE)

install-jar: $(JAR_FILE)
	cp $(JAVA_CLASS_DIR)/$(JAR_FILE) $(INSTALL_DIR)
	cp $(HTML_FILE) $(INSTALL_DIR)

debug-jar: debug_jarfile
	cp $(JAVA_CLASS_DIR)/$(JAR_FILE) $(INSTALL_DIR)
	cp $(HTML_FILE) $(INSTALL_DIR)

clean:
	@- echo "removing objects and arguments"
	- rm -f $(CLEAN_OBJS) $(JAVA_CLASS_DIR)/$(JAVA_CLASS_DIR)/$(JAR_FILE) $(JC_ARGS_FILE) $(TEMP_OBJS) core *~
	- rm -rf $(IMAGE_DEST_DIR)

really-clean: clean
	@- echo "removing classes directory"
	- rm -rf $(JAVA_CLASS_DIR)


# ============================================================================
# EOF
# ============================================================================
