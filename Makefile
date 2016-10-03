SERVER = server
CLIENT = client
LIB = libramp.so

OBJDIR = ./obj
INC_DIRS = $(shell find ./include/common ./include/server ./include/client -type d)

SERVER_SRCS = $(shell find ./src/common ./src/server -name *.cpp)
CLIENT_SRCS = $(shell find ./src/common ./src/client -name *.cpp)
LIB_SRCS    = $(shell find ./src/common ./src/client/net ./src/client/service/handler ./src/lib -name *.cpp)

SERVER_OBJS = $(addprefix $(OBJDIR)/server/, $(notdir $(SERVER_SRCS:.cpp=.o)))
CLIENT_OBJS = $(addprefix $(OBJDIR)/client/, $(notdir $(CLIENT_SRCS:.cpp=.o)))
LIB_OBJS    = $(addprefix $(OBJDIR)/lib/, $(notdir $(LIB_SRCS:.cpp=.o)))

INC_FLAGS = $(addprefix -I , $(INC_DIRS))
CXXFLAGS = -Wall -O2 -std=c++11 $(INC_FLAGS)
LDFLAGS = -ltbb -lrdmacm -libverbs


all: $(SERVER) $(CLIENT) $(LIB)

$(SERVER): $(SERVER_OBJS)
	$(CXX) $^ -o $@ $(LDFLAGS)

$(CLIENT): $(CLIENT_OBJS)
	$(CXX) $^ -o $@ $(LDFLAGS)

$(LIB): $(LIB_OBJS)
	$(CXX) $^ -o $@ $(LDFLAGS) -shared

$(OBJDIR)/server/%.o: 
	@[ -d $(OBJDIR)/server ] || mkdir -p $(OBJDIR)/server
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $(filter %/$(notdir $(basename $@)).cpp, $(SERVER_SRCS)) -o $@

$(OBJDIR)/client/%.o: 
	@[ -d $(OBJDIR)/client ] || mkdir -p $(OBJDIR)/client
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $(filter %/$(notdir $(basename $@)).cpp, $(CLIENT_SRCS)) -o $@

$(OBJDIR)/lib/%.o: 
	@[ -d $(OBJDIR)/lib ] || mkdir -p $(OBJDIR)/lib
	$(CXX) $(CXXFLAGS) -fPIC $(INCLUDE) -I ./include/lib -I $(JAVA_HOME)/include -I $(JAVA_HOME)/include/linux \
	-c $(filter %/$(notdir $(basename $@)).cpp, $(LIB_SRCS)) -o $@


.PHONY: clean
clean:
	rm -rf *~ $(SERVER) $(CLIENT) $(LIB) $(OBJDIR)