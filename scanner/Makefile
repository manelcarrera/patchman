src = $(wildcard ./common/cpp/*.cpp ./linux/cpp/*.cpp)
obj = $(src:.c=.o)
CXX = g++
INCDIRS := -I./common/h -I./linux/h

#CXXFLAGS := -std=c++11 -Wall -Wextra
#CXXFLAGS := -std=c++11 -fpermissive -Wwrite-strings

LDFLAGS = -lsqlite3 -lcurl -lcrypto -lssl

scanner: $(obj)
	$(CXX) $(INCDIRS) -o $@ $^ $(LDFLAGS)

#.PHONY: clean
#clean:
#	rm -f $(obj) myprog