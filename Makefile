# The name of the plugin.
PLUGIN = PrintFunctionNames

# LLVM paths. Note: you probably need to update these.
LLVM_DIR = /usr/local/lib/llvm
LLVM_BUILD_DIR = /usr/local/lib/build
CLANG_DIR = $(LLVM_DIR)/tools/clang
CLANG = $(LLVM_BUILD_DIR)/bin/clang

# Compiler flags.
CXXFLAGS  = -I$(LLVM_DIR)/include -I$(CLANG_DIR)/include
CXXFLAGS += -I$(LLVM_BUILD_DIR)/include -I$(LLVM_BUILD_DIR)/tools/clang/include
CXXFLAGS += -D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS -Wno-long-long
CXXFLAGS += -fPIC -fvisibility-inlines-hidden
CXXFLAGS += -fno-exceptions -fno-rtti -std=c++11
CXXFLAGS += -Wall

# Linker flags.
LDFLAGS = -shared -Wl,-undefined,dynamic_lookup

$(PLUGIN).so : $(PLUGIN).o
	$(CXX) $(LDFLAGS) -o $(PLUGIN).so $(PLUGIN).o

$(PLUGIN).o : $(PLUGIN).cpp
	$(CXX) $(CXXFLAGS) -c $(PLUGIN).cpp -o $(PLUGIN).o

check : $(PLUGIN).so
	$(CLANG) -c -Xclang -load -Xclang ./$(PLUGIN).so \
                -Xclang -add-plugin -Xclang print-fns test.c

clean :
	rm -fv $(PLUGIN).o $(PLUGIN).so test.o
