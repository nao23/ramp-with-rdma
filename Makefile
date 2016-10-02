SERVER = server
CLIENT = client

INC_DIRS = $(shell find ./include/common ./include/server ./include/client -type d)

SERVER_SRCS = $(shell find ./src/common ./src/server -name *.cpp)
CLIENT_SRCS = $(shell find ./src/common ./src/client -name *.cpp)

SERVER_OBJS = $(addsuffix .o,$(basename $(SERVER_SRCS)))
CLIENT_OBJS = $(addsuffix .o,$(basename $(CLIENT_SRCS)))

INC_FLAGS = $(addprefix -I , $(INC_DIRS))
CXXFLAGS = -Wall -O2 -std=c++11 $(INC_FLAGS)
LDFLAGS = -ltbb -lrdmacm -libverbs

all: $(SERVER) $(CLIENT)

$(SERVER): $(SERVER_OBJS)
	$(CXX) $^ -o $@ $(LDFLAGS)

$(CLIENT): $(CLIENT_OBJS)
	$(CXX) $^ -o $@ $(LDFLAGS)

.PHONY: clean
clean:
	rm -rf *~ $(SERVER) $(CLIENT) $(shell find src -name *.o)