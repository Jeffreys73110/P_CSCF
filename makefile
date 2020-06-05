b64 := true	# comment this line as 32 bit
# -m32 bits or -m64 bits
ifdef b64
	BITS := -m64
	OS_BIT := 64
else
	BITS := -m32
	OS_BIT := 32
endif

# if make debug=1 than debug mode
ifndef debug
	DEBUG := -D NDEBUG
endif

# variables
DIR_BUILD := build
DIR_SRC := src
DIR_SEC_AUT := $(DIR_SRC)/sec
DIR_MOD := $(DIR_SRC)/module
LINK_TARGET := p_cscf.out
# libs
LIB_PATH := $(DIR_SRC)/module/wincmpt/lib $(DIR_SRC)/module/netcpi/lib
LIBS := netcpi$(OS_BIT) wincmpt$(OS_BIT)
SYS_LIBS := -lpthread

# library path -L(path)
LIB_L := $(foreach n,$(LIB_PATH),-L$(n))
LIB_l := $(foreach n,$(LIBS),-l$(n))

# SYMBOL_CCM := $(subst ..,$(FBACK),$(subst /,$(SLASH),$(CCM)))	// TODO: study subst
# OBJS := $(patsubst %.c,%.c.o,$(patsubst %.cpp,%.o,$(SYMBOL_CCM)))	// TODO: study patsubst
OBJS := 
OBJS += ${DIR_BUILD}/DTrace.o
OBJS += ${DIR_BUILD}/Functions.o
OBJS += ${DIR_BUILD}/sctp_client.o
# OBJS += ${DIR_BUILD}/MapProcess.o
# OBJS += ${DIR_BUILD}/QueueManager.o
# OBJS += ${DIR_BUILD}/ServerDlg.o
# OBJS += ${DIR_BUILD}/Jfunction.o
OBJS += ${DIR_BUILD}/MD5.o
OBJS += ${DIR_BUILD}/Authentication.o
OBJS += ${DIR_BUILD}/crypt.o
# OBJS += ${DIR_BUILD}/timer.o
# OBJS += ${DIR_BUILD}/DlgQuery.o
# OBJS += ${DIR_BUILD}/Security.o
# OBJS += ${DIR_BUILD}/User.o
OBJS += ${DIR_BUILD}/SIP_Server.o
OBJS += ${DIR_BUILD}/net_interface.o
OBJS += ${DIR_BUILD}/server.o


# UDP_OBJ :=
# UDP_OBJ += ${DIR_BUILD}/DTrace.o
# UDP_OBJ += ${DIR_BUILD}/Functions.o
# UDP_OBJ += ${DIR_BUILD}/net_interface.o
# UDP_OBJ += ${DIR_BUILD}/Sip_Response.o 
# UDP_OBJ += ${DIR_BUILD}/rtp.o 
# UDP_OBJ += ${DIR_BUILD}/UDP.o 
# UDP_OBJ += ${DIR_BUILD}/sctp_client.o 
# # UDP_OBJ += ${AMRWB_LIBOBJ2}




all : clean init $(OBJS)
	g++ $(BITS) -Wl,--gc-sections  -Wl,-Map,$(LINK_TARGET).map -o $(LINK_TARGET) $(OBJS) $(SYS_LIBS) $(LIB_l) $(LIB_L)
	@echo All done

# g++ -m64 -Wl,--gc-sections  -Wl,-Map,s1ap_server.map -o s1ap_server 
# 	build/DTrace.o build/Functions.o build/MapProcess.o build/QueueManager.o build/ServerDlg.o build/s1ap_decode.o build/s1ap_encode.o build/s1ap_common.o 
# 	build/nas_decode.o build/nas_encode.o build/s1ap.o build/spgw.o build/mme.o build/f1.o build/Jfunction.o build/timer.o build/sha256.o build/snow_3g.o build/DlgQuery.o 
# 	build/test.o -lpthread -lnetcpi64 -lwincmpt64 -Lsrc/module/wincmpt/lib -Lsrc/module/netcpi/lib

ims : clean init $(UDP_OBJ)
	g++ $(BITS) -Wl,--gc-sections -Wl,-Map,ims.map -o ims.out $(UDP_OBJ) $(SYS_LIBS) $(LIB_l) $(LIB_L)
	@echo All done

init :
	mkdir -p build
$(DIR_BUILD)/%.o : $(DIR_SRC)/%.cpp $(DIR_SRC)/%.h
	g++ $(BITS) -c $< -o $@
$(DIR_BUILD)/%.o : $(DIR_SRC)/SIP_Server/%.cpp $(DIR_SRC)/SIP_Server/%.h
	g++ $(BITS) -c $< -o $@
# $(DIR_BUILD)/%.o : $(DIR_SRC)/User.cpp
# 	g++ $(BITS) -c $< -o $@
# $(DIR_BUILD)/%.o : $(DIR_SEC_AUT)/%.cpp $(DIR_SEC_AUT)/%.h
# 	g++ $(BITS) -c $< -o $@
# $(DIR_BUILD)/%.o : $(DIR_MOD)/%.cpp $(DIR_MOD)/%.h
# 	g++ $(BITS) -c $< -o $@
# $(DIR_BUILD)/%.o : $(DIR_MOD)/DlgServer/%.cpp $(DIR_MOD)/DlgServer/%.h
# 	g++ $(BITS) -c $< -o $@
$(DIR_BUILD)/%.o : $(DIR_MOD)/DTrace/%.cpp $(DIR_MOD)/DTrace/%.h
	g++ $(BITS) -c $< -o $@ $(DEBUG)
$(DIR_BUILD)/%.o : $(DIR_MOD)/Functions/%.cpp $(DIR_MOD)/Functions/%.h
	g++ $(BITS) -c $< -o $@
# $(DIR_BUILD)/%.o : $(DIR_MOD)/MapProcess/%.cpp $(DIR_MOD)/MapProcess/%.h
# 	g++ $(BITS) -c $< -o $@
$(DIR_BUILD)/%.o : $(DIR_MOD)/MD5/%.cpp $(DIR_MOD)/MD5/%.h
	g++ $(BITS) -c $< -o $@
$(DIR_BUILD)/%.o : $(DIR_MOD)/crypt/%.cpp $(DIR_MOD)/crypt/%.h
	g++ $(BITS) -c $< -o $@
$(DIR_BUILD)/%.o : $(DIR_MOD)/net_interface/%.cpp $(DIR_MOD)/net_interface/%.h
	g++ $(BITS) -c $< -o $@
$(DIR_BUILD)/%.o : $(DIR_MOD)/sctp_client/%.cpp $(DIR_MOD)/sctp_client/%.h
	g++ $(BITS) -c $< -o $@

# $(DIR_BUILD)/UDP.o : $(DIR_SRC)/IMS_UDP_S/UDP.cpp
# 	g++ $(BITS) -c $(DIR_SRC)/IMS_UDP_S/UDP.cpp -o $(DIR_BUILD)/UDP.o
# $(DIR_BUILD)/UDP.o : $(DIR_SRC)/IMS_UDP_S/UDP.cpp
# 	g++ $(BITS) -c $< -o $@
# $(DIR_BUILD)/%.o : $(DIR_SRC)/IMS_UDP_S/%.cpp $(DIR_SRC)/IMS_UDP_S/%.h
# 	g++ $(BITS) -c $< -o $@
# $(DIR_BUILD)/%.o : $(DIR_SRC)/IMS_UDP_S/Sip_Response/%.cpp $(DIR_SRC)/IMS_UDP_S/Sip_Response/%.h
# 	g++ $(BITS) -c $< -o $@

.phony: clean
clean:
	-rm $(DIR_BUILD)/*.o $(LINK_TARGET)
