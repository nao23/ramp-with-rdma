vpath %.cpp ./src/server:./src/client:./src/net:./src/data
vpath %.cpp ./src/service:./src/service/handler:./src/service/executor
vpath %.cpp ./src/config:./src/util:./src/lib

CXXFLAGS = -Wall -O2 -std=c++11 -g
INCLUDE =  -I ./include/server -I ./include/client -I ./include/net \
		-I ./include/data -I ./include/service -I ./include/service/handler \
		-I ./include/service/executor -I ./include/config -I ./include/util
LDFLAGS = -ltbb -lrdmacm -libverbs

SERVER_SRC =    server_main.cpp Server.cpp ServerThread.cpp \
		HostAndPort.cpp TCPServerSocket.cpp TCPSocket.cpp RDMACMServerSocket.cpp RDMACMSocket.cpp \
		SendRecvServerSocket.cpp SendRecvSocket.cpp \
		RDMAWriteServerSocket.cpp RDMAWriteSocket.cpp \
		RDMAWriteImmSocket.cpp RDMAWriteImmServerSocket.cpp \
		Timestamp.cpp Item.cpp Table.cpp CommittedItemsCache.cpp \
		Transaction.cpp RAMPFastExecutor.cpp \
		LockBasedExecutor.cpp NoCCExecutor.cpp ACRAMPFastExecutor.cpp \
		MessageHeader.cpp Mutex.cpp LockManager.cpp \
		Buffer.cpp Config.cpp

CLIENT_SRC =    client_main.cpp Client.cpp ClientThread.cpp \
		HostAndPort.cpp ConnectionPool.cpp \
		TCPSocket.cpp RDMACMSocket.cpp SendRecvSocket.cpp RDMAWriteSocket.cpp RDMAWriteImmSocket.cpp \
		Timestamp.cpp Item.cpp \
		Transaction.cpp TrxQueue.cpp \
		RAMPFastHandler.cpp \
		LockBasedHandler.cpp NoCCHandler.cpp ACRAMPFastHandler.cpp MessageHeader.cpp Mutex.cpp \
		Buffer.cpp Config.cpp

RAMPLIB_SRC =   com_yahoo_ycsb_db_JNIRAMPClient.cpp \
		HostAndPort.cpp ConnectionPool.cpp \
		TCPSocket.cpp RDMACMSocket.cpp SendRecvSocket.cpp \
		RDMAWriteSocket.cpp RDMAWriteImmSocket.cpp \
                Timestamp.cpp Item.cpp \
                RAMPFastHandler.cpp \
		LockBasedHandler.cpp NoCCHandler.cpp ACRAMPFastHandler.cpp \
		Transaction.cpp MessageHeader.cpp Mutex.cpp \
	        Buffer.cpp Config.cpp

OBJDIR = ./obj
LIB_OBJDIR = ./lib_obj

SERVER_OBJ = $(addprefix $(OBJDIR)/, $(SERVER_SRC:.cpp=.o))
CLIENT_OBJ = $(addprefix $(OBJDIR)/, $(CLIENT_SRC:.cpp=.o))
RAMPLIB_OBJ = $(addprefix $(LIB_OBJDIR)/, $(RAMPLIB_SRC:.cpp=.o))

SERVER = server
CLIENT = client
RAMPLIB = libramp.so

all: $(SERVER) $(CLIENT) $(RAMPLIB)

$(SERVER): $(SERVER_OBJ)
	$(CXX) $^ -o $@ $(LDFLAGS)

$(CLIENT): $(CLIENT_OBJ)
	$(CXX) $^ -o $@ $(LDFLAGS)

$(RAMPLIB): $(RAMPLIB_OBJ)
	$(CXX) $^ -o $@ $(LDFLAGS) -shared

$(OBJDIR)/%.o: %.cpp
	@[ -d $(OBJDIR) ] || mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

$(LIB_OBJDIR)/%.o: %.cpp
	@[ -d $(LIB_OBJDIR) ] || mkdir -p $(LIB_OBJDIR)
	$(CXX) $(CXXFLAGS) -fPIC $(INCLUDE) -I ./include/lib -I $(JAVA_HOME)/include \
		-I $(JAVA_HOME)/include/linux/ -c $< -o $@

.PHONY: clean
clean:
	rm -rf *~ $(OBJDIR) $(LIB_OBJDIR) $(SERVER) $(CLIENT) $(RAMPLIB)