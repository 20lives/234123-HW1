#TODO: replace ID with your own IDS, for example: 123456789_123456789
SUBMITTERS := 316327857_316327857
COMPILER := g++
COMPILER_FLAGS := --std=c++11
SRCS := BuiltInCommand.cpp  Commands.cpp  ExternalCommand.cpp  FgJob.cpp  JobsList.cpp  PipeCommand.cpp  RedirectionCommand.cpp  signals.cpp  SmallShell.cpp  smash.cpp  Utilities.cpp
OBJS=$(subst .cpp,.o,$(SRCS))
HDRS := BuiltInCommand.h  Commands.h  ExternalCommand.h  FgJob.h  JobsList.h  PipeCommand.h  RedirectionCommand.h  signals.h  SmallShell.h  Utilities.h
TESTS_INPUTS := $(wildcard test_input*.txt)
TESTS_OUTPUTS := $(subst input,output,$(TESTS_INPUTS))
SMASH_BIN := smash

test: $(TESTS_OUTPUTS)

$(TESTS_OUTPUTS): $(SMASH_BIN)
$(TESTS_OUTPUTS): test_output%.txt: test_input%.txt test_expected_output%.txt
	./$(SMASH_BIN) < $(word 1, $^) > $@
	diff $@ $(word 2, $^)
	echo $(word 1, $^) ++PASSED++

$(SMASH_BIN): $(OBJS)
	$(COMPILER) $(COMPILER_FLAGS) $^ -o $@

$(OBJS): %.o: %.cpp
	$(COMPILER) $(COMPILER_FLAGS) -c $^

zip: $(SRCS) $(HDRS)
	zip $(SUBMITTERS).zip $^ submitters.txt Makefile

clean:
	rm -rf $(SMASH_BIN) $(OBJS) $(TESTS_OUTPUTS) 
	rm -rf $(SUBMITTERS).zip
