#preprocess
ifeq ($(PROJECT_SRC_PATH),)
	PROJECT_SRC_PATH = $(shell pwd)/..
endif

include $(PROJECT_SRC_PATH)/makefile.incl

BUILD = testmwstring #testserializable testbinfile

all:
	for i in $(BUILD); do \
		$(CXX) $(CXXFLAGS) $(MWMATH_MACRO) $(INC_MWSTL) $(INC_MWMATH) $(INC_MWGLOBAL) $(INC_LOG4CPLUS) -c $(addsuffix .cpp, $$i) -o $(addsuffix .o, $$i); \
		$(CXX) $(addsuffix .o, $$i) $(STATIC_LIB_MWMATH) $(STATIC_LIB_MWSTL) $(STATIC_LIB_MWGLOBAL) $(STATIC_LIB_LOG4CPLUS) $(LIB_RT) -o $$i; \
	done

clean:
	$(CLEAN)

rebuild:
	$(REBUILD)
