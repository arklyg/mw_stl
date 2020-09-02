#preprocess
ifeq ($(PROJECT_SRC_PATH),)
	PROJECT_SRC_PATH = $(shell pwd)/..
endif

include $(PROJECT_SRC_PATH)/makefile.incl

.PHONEY : all clean rebuild install test

TARGET = libmwstl.a
LIB_INCS = $(wildcard *.h)

all:$(TARGET)

$(TARGET):$(MAIN_OBJS)
	$(AR) $@ $^

%.o:%.cc
	$(CXX) $(CXXFLAGS) $(INC_MWSTL) $(MWSTL_MACRO) $(INC_MWGLOBAL) $(INC_LOG4CPLUS) -c $< -o $@

clean:
	$(RM) $(TARGET)
	$(RM) $(MAIN_OBJS)

rebuild:
	$(MAKE) clean
	$(MAKE)

install:
	$(CP) $(TARGET) $(DIR_MWSTL_LIB)
	$(CP) $(LIB_INCS) $(DIR_MWSTL_INC)
