SERVER := server
CLIENT := client
BUILD_DIR := ./build
SRC_DIRS := ./src
INC_DIRS := ../../include

# Find all the C and C++ files we want to compile
# Note the single quotes around the * expressions. 
# The shell will incorrectly expand these otherwise, but we want to send the * directly to the find command.
SERVER_SRCS := $(shell find $(SRC_DIRS) -name '*_server.c')
CLIENT_SRCS := $(shell find $(SRC_DIRS) -name '*_client.c')
INC_SRCS := $(shell find $(INC_DIRS) -name '*.c')
SRCS = SERVER_SRCS + CLIENT_SRCS + INC_SRCS 
SERVER_SRCS += $(INC_SRCS)
CLIENT_SRCS += $(INC_SRCS)

# Prepends BUILD_DIR and appends .o to every src file
# As an example, ./your_dir/hello.cpp turns into ./build/./your_dir/hello.cpp.o
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
SERVER_OBJS := $(SERVER_SRCS:%=$(BUILD_DIR)/%.o)
CLIENT_OBJS := $(CLIENT_SRCS:%=$(BUILD_DIR)/%.o)

# String substitution (suffix version without %).
# As an example, ./build/hello.cpp.o turns into ./build/hello.cpp.d
DEPS := $(OBJS:.o=.d)

# Every folder in ./src will need to be passed to GCC so that it can find header files
INC_DIRS += $(shell find $(SRC_DIRS) -type d)

# Add a prefix to INC_DIRS. So moduleA would become -ImoduleA. GCC understands this -I flag
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# The -MMD and -MP flags together generate Makefiles for us!
# These files will have .d instead of .o as the output.
CPPFLAGS := $(INC_FLAGS) -MMD -MP
LDFLAGS := -lpthread

# The final build step.
all: $(BUILD_DIR)/$(SERVER) $(BUILD_DIR)/$(CLIENT)
	rm -rf $(BUILD_DIR)/include
	mv $(BUILD_DIR)/../../include $(BUILD_DIR)

$(BUILD_DIR)/$(SERVER): $(SERVER_OBJS)
	$(CXX) $(SERVER_OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/$(CLIENT): $(CLIENT_OBJS)
	$(CXX) $(CLIENT_OBJS) -o $@ $(LDFLAGS)

# Build step for C source
$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Build step for C++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)

# Include the .d makefiles. The - at the front suppresses the errors of missing
# Makefiles. Initially, all the .d files will be missing, and we don't want those
# errors to show up.
-include $(DEPS)