CC = g++ -std=c++11
CFLAGS = -lpthread -lmysqlclient -lcrypto -I./include/

TARGET  = ./app
OBJ_DIR = ./obj
SRC_DIR = ./source
SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, %.o, $(SRC))

VPATH = $(SRC_DIR)
vpath %.o $(OBJ_DIR)

all: $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CC) -o $@ $(addprefix $(OBJ_DIR)/, $(OBJECTS)) $(CFLAGS)

%.o : %.cpp
	$(CC) -c $< -o $(OBJ_DIR)/$@

.PHONY : clean
clean:
	rm -rf $(TARGET) $(OBJ_DIR)/*.o ./*.o
