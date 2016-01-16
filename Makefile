all: count

#%: %.cpp
#	g++ -std=c++11 $< -o $@

#%: %.c
#	gcc $< -o $@
##CFLAGS=-std=c++11

count: TermsCounter.cpp TermsCounter.h UltimateTermsCounterTest.cpp
	g++ $(CFLAGS) -o count TermsCounter.cpp UltimateTermsCounterTest.cpp

counter.o: TermsCounter.cpp TermsCounter.h
	g++ $(CFLAGS) -c TermsCounter.cpp
	
test.o: UltimateTermsCounterTest.cpp
	g++ $(CFLAGS) -c UltimateTermsCounterTest.cpp
	
clean:
	$(RM) count