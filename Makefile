CXX = g++
CXXFLAGS = -Wall -std=c++98
CXXDEBUGFLAGS = -ggdb -fsanitize=address -fsanitize=undefined
CXXFILES = wav_generator.cpp
TARGETS =  debug release
EXTENSION = .wav
.PHONY: all fresh clean clean_wav

all: $(TARGETS)

release:
	$(info Building out files $@)
	$(CXX) $(CXXFLAGS) $(CXXFILES) -o $@.out
	$(info Built $@)

debug:
	$(info Building $@)
	$(CXX) $(CXXFLAGS) $(CXXDEBUGFLAGS) $(CXXFILES) -o $@.out
	$(info Built $@)

clean:
	$(info Cleaning)
	rm -f $(TARGETS)
	rm -f $(HANDIN)
	$(info Cleaned)

clean_wav:
	$(info Remove all $(EXTENSION) files)
	find . -type f -name *$(EXTENSION) -exec rm -f  {} \;

fresh: clean all
