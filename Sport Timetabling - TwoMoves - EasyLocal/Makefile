EASYLOCAL = /usr/local/include/easylocal
FLAGS = -std=c++14 -Wall -Wfatal-errors -O3
COMPOPTS = -I$(EASYLOCAL)/include $(FLAGS)
LINKOPTS = -lboost_program_options -pthread

OBJECT_FILES = STT_Input.o STT_Output.o STT_Match.o STT_State.o STT_Util.o STT_MatchDay.o STT_OutputManager.o STT_Move.o STT_StateManager.o STT_NeighborhoodExplorer.o STT_CostComponents.o STT_DeltaCostComponents.o STT_Main.o 

SportTimeTabling: $(OBJECT_FILES)
	g++ $(OBJECT_FILES) $(LINKOPTS) -o SportTimeTabling

STT_Input.o: STT_Input.cpp STT_Input.hh 
	g++ -c $(COMPOPTS) STT_Input.cpp

STT_Match.o: STT_Match.cpp STT_Match.hh 
	g++ -c $(COMPOPTS) STT_Match.cpp

STT_Util.o: STT_Util.cpp STT_Util.hh
	g++ -c $(COMPOPTS) STT_Util.cpp

STT_MatchDay.o: STT_MatchDay.cpp STT_MatchDay.hh STT_Input.hh STT_Util.hh STT_Match.hh
	g++ -c $(COMPOPTS) STT_MatchDay.cpp

STT_Output.o: STT_Output.cpp STT_Output.hh STT_Input.hh
	g++ -c $(COMPOPTS) STT_Output.cpp

STT_State.o: STT_State.cpp STT_State.hh STT_Input.hh STT_MatchDay.hh
	g++ -c $(COMPOPTS) STT_State.cpp

STT_OutputManager.o: STT_OutputManager.cpp STT_OutputManager.hh STT_Input.hh STT_Output.hh STT_State.hh
	g++ -c $(COMPOPTS) STT_OutputManager.cpp

STT_Move.o: STT_Move.cpp STT_Move.hh STT_Input.hh
	g++ -c $(COMPOPTS) STT_Move.cpp

STT_StateManager.o : STT_StateManager.cpp STT_StateManager.hh STT_Input.hh STT_State.hh
	g++ -c $(COMPOPTS) STT_StateManager.cpp

STT_NeighborhoodExplorer.o: STT_NeighborhoodExplorer.cpp STT_NeighborhoodExplorer.hh STT_Input.hh STT_State.hh STT_Move.hh
	g++ -c $(COMPOPTS) STT_NeighborhoodExplorer.cpp

STT_CostComponents.o: STT_CostComponents.cpp STT_CostComponents.hh STT_Input.hh STT_State.hh STT_Move.hh
	g++ -c $(COMPOPTS) STT_CostComponents.cpp

STT_DeltaCostComponents.o: STT_DeltaCostComponents.cpp STT_DeltaCostComponents.hh STT_Input.hh STT_State.hh STT_Move.hh STT_CostComponents.hh STT_Util.hh
	g++ -c $(COMPOPTS) STT_DeltaCostComponents.cpp

STT_Main.o: STT_Main.cpp STT_DeltaCostComponents.hh STT_CostComponents.hh STT_StateManager.hh STT_NeighborhoodExplorer.hh STT_OutputManager.hh
	g++ -c $(COMPOPTS) STT_Main.cpp

clean:
	rm -f $(OBJECT_FILES) SportTimeTabling



