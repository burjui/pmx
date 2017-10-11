MGR = billmgr
PLUGIN = pmx

CFLAGS += -I/usr/local/mgr5/include/billmgr
CXXFLAGS += -I/usr/local/mgr5/include/billmgr

WRAPPER += pmx
pmx_SOURCES = pmx.cpp
pmx_FOLDER = processing
pmx_LDADD = -lmgr -lmgrdb
pmx_DLIBS = processingmodule processingdomain

BASE ?= /usr/local/mgr5
include $(BASE)/src/isp.mk
