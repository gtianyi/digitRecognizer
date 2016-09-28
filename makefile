CXX=g++

all: digitRec digitRec_debug digitRecDT digitRecDT_debug digitRecRF digitRecRF_debug

digitRec: digit.h recognizer.cpp
	$(CXX)  $^ -o digitRec


digitRec_debug: digit.h recognizer.cpp
	$(CXX) -g $^ -o digitRec_debug

digitRecDT: digit.h dtree.h recognizerDT.cpp
	$(CXX)  $^ -o digitRecDT


digitRecDT_debug: digit.h dtree.h recognizerDT.cpp
	$(CXX) -g $^ -o digitRecDT_debug

digitRecRF: digit.h dtree.h recognizerRF.cpp
	$(CXX)  $^ -o digitRecRF


digitRecRF_debug: digit.h dtree.h recognizerRF.cpp
	$(CXX) -g $^ -o digitRecRF_debug

clean:
	rm -f digitRec digitRec_debug  digitRecDT digitRecDT_debug digitRecRF digitRecRF_debug 
	rm -f *.o *.gch
