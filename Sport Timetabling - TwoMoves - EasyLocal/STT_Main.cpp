#include "STT_CostComponents.hh"
#include "STT_DeltaCostComponents.hh"
#include "STT_StateManager.hh"
#include "STT_NeighborhoodExplorer.hh"
#include "STT_OutputManager.hh"


using namespace EasyLocal::Debug;

int main(int argc, const char* argv[])
{
	ParameterBox main_parameters("main", "Main Program options");

	Parameter<string> instance("instance", "Input instance", main_parameters); 
	Parameter<int> seed("seed", "Random seed", main_parameters);
	Parameter<string> method("method", "Solution method (empty for tester)", main_parameters);   
	Parameter<string> init_state("init_state", "Initial state (to be read from file)", main_parameters);
	Parameter<string> output_file("output_file", "Write the output to a file (filename required)", main_parameters);

	CommandLineParameters::Parse(argc, argv, false, true);  

	STT_Input input(instance);

	if (seed.IsSet())
		Random::SetSeed(seed);

	STT_StateManager sttStateManager(input);
	STT_SwapNeighborhoodExplorer sttSwapNeighbourExplorer(input, sttStateManager);
	STT_ChangeHospitalityNeighborhoodExplorer sttCHNeighbourExplorer(input, sttStateManager);
	STT_OutputManager sttOutputManager(input);

	//cost components;
	RePlayedMatchesCostComponent replayedMatchCC(input);
	NoMatchesPlayedCostComponent noMatchesCC(input);
	BigMatchesPerMatchDayCostComponent bigMatchesCC(input);
	ForbiddenInMatchDayCostComponent forbiddenInMCC(input);
	ComplementaryTeamsCostComponent complementaryCC(input);
	SequenceOfHomeGamesCostComponent seqOfHomeGamesCC(input);
	SequenceOfAwayGamesCostComponent seqOfAwayGamesCC(input);
	ForbiddenTogetherCostComponent forbiddenTogetherCC(input);
	ShouldPlayAtHomeCostComponent shoulPlayAtHomeCC(input);

	//delta cost components for swap
	SwapRePlayedMatchesDeltaCostComponent swapReplayedDelta(input, replayedMatchCC);
	SwapBigMatchesPerMatchDayDeltaCostComponent swapBigMatchesDelta(input, bigMatchesCC);
	SwapForbiddenInMatchDayDeltaCostComponent swapForbiddenMDelta(input, forbiddenInMCC);
	SwapComplementaryTeamsDeltaCostComponent swapComplementaryDelta(input, complementaryCC);
	SwapSequenceOfHomeGamesDeltaCostComponent swapSeqOfHomeDelta(input, seqOfHomeGamesCC);
	SwapSequenceOfAwayGamesDeltaCostComponent swapSeqOfAwayDelta(input, seqOfAwayGamesCC);
	SwapForbiddenTogetherDeltaCostComponent swapForbiddenTogetherDelta(input, forbiddenTogetherCC);
	SwapShouldPlayAtHomeDeltaCostComponent swapShouldPlayAtHomeDelta(input, shoulPlayAtHomeCC);

	//delta cost components for CH
	CHRePlayedMatchesDeltaCostComponent chReplayedDelta(input, replayedMatchCC);
	CHBigMatchesPerMatchDayDeltaCostComponent chBigMatchesDelta(input, bigMatchesCC);
	CHForbiddenInMatchDayDeltaCostComponent chForbiddenMDelta(input, forbiddenInMCC);
	CHComplementaryTeamsDeltaCostComponent chComplementaryDelta(input, complementaryCC);
	CHSequenceOfHomeGamesDeltaCostComponent chSeqOfHomeDelta(input, seqOfHomeGamesCC);
	CHSequenceOfAwayGamesDeltaCostComponent chSeqOfAwayDelta(input, seqOfAwayGamesCC);
	CHForbiddenTogetherDeltaCostComponent chForbiddenTogetherDelta(input, forbiddenTogetherCC);
	CHShouldPlayAtHomeDeltaCostComponent chShouldPlayAtHomeDelta(input, shoulPlayAtHomeCC);


	sttStateManager.AddCostComponent(replayedMatchCC);
	sttStateManager.AddCostComponent(bigMatchesCC);
	sttStateManager.AddCostComponent(forbiddenInMCC);
	sttStateManager.AddCostComponent(complementaryCC);
	sttStateManager.AddCostComponent(seqOfHomeGamesCC);
	sttStateManager.AddCostComponent(seqOfAwayGamesCC);
	sttStateManager.AddCostComponent(forbiddenTogetherCC);
	sttStateManager.AddCostComponent(shoulPlayAtHomeCC);

	sttSwapNeighbourExplorer.AddDeltaCostComponent(swapReplayedDelta);
	sttSwapNeighbourExplorer.AddDeltaCostComponent(swapBigMatchesDelta);
	sttSwapNeighbourExplorer.AddDeltaCostComponent(swapForbiddenMDelta);
	sttSwapNeighbourExplorer.AddDeltaCostComponent(swapComplementaryDelta);
	sttSwapNeighbourExplorer.AddDeltaCostComponent(swapSeqOfHomeDelta);
	sttSwapNeighbourExplorer.AddDeltaCostComponent(swapSeqOfAwayDelta);
	sttSwapNeighbourExplorer.AddDeltaCostComponent(swapForbiddenTogetherDelta);
	sttSwapNeighbourExplorer.AddDeltaCostComponent(swapShouldPlayAtHomeDelta);


	sttCHNeighbourExplorer.AddDeltaCostComponent(chForbiddenMDelta);
	sttCHNeighbourExplorer.AddDeltaCostComponent(chComplementaryDelta);
	sttCHNeighbourExplorer.AddDeltaCostComponent(chSeqOfHomeDelta);
	sttCHNeighbourExplorer.AddDeltaCostComponent(chSeqOfAwayDelta);
	sttCHNeighbourExplorer.AddDeltaCostComponent(chForbiddenTogetherDelta);
	sttCHNeighbourExplorer.AddDeltaCostComponent(chShouldPlayAtHomeDelta);


	// runners with swap neighbour explorer
	HillClimbing<STT_Input, STT_State, STT_Move_Swap> sttSwapHC(input, sttStateManager, sttSwapNeighbourExplorer, "HC-SWAP");
	SteepestDescent<STT_Input, STT_State, STT_Move_Swap> sttSwapSD(input, sttStateManager, sttSwapNeighbourExplorer, "SD-SWAP");
	SimulatedAnnealing<STT_Input, STT_State, STT_Move_Swap> sttSwapSA(input, sttStateManager, sttSwapNeighbourExplorer, "SA-SWAP");
	TabuSearch<STT_Input, STT_State, STT_Move_Swap> sttSwapTS(input, sttStateManager, sttSwapNeighbourExplorer, "TS-SWAP", 
		 [](const STT_Move_Swap& m1, const STT_Move_Swap& m2)->bool
       { return m1.matchDay == m2.matchDay && ((m1.firstTeam == m2.firstTeam && m1.secondTeam == m2.secondTeam) || (m1.secondTeam == m2.firstTeam && m1.firstTeam == m2.secondTeam)); });


	// runners with swap Change Hospitality explorer
	HillClimbing<STT_Input, STT_State, STT_Move_ChangeHospitality> sttCHHC(input, sttStateManager, sttCHNeighbourExplorer, "HC-CH");
	SteepestDescent<STT_Input, STT_State, STT_Move_ChangeHospitality> sttCHSD(input, sttStateManager, sttCHNeighbourExplorer, "SD-CH");
	SimulatedAnnealing<STT_Input, STT_State, STT_Move_ChangeHospitality> sttCHSA(input, sttStateManager, sttCHNeighbourExplorer, "SA-CH");
	TabuSearch<STT_Input, STT_State, STT_Move_ChangeHospitality> sttCHTS(input, sttStateManager, sttCHNeighbourExplorer, "TS-CH", 
		 [](const STT_Move_ChangeHospitality& m1, const STT_Move_ChangeHospitality& m2)->bool
       { return m1.matchDay == m2.matchDay && m1.team == m2.team; });


	if (!CommandLineParameters::Parse(argc, argv, true, false))
		return 1;

	if (!instance.IsSet())
	{
		cout << "Error: --main::instance=filename option must always be set" << endl;
		return 1;
	}

	// tester
	Tester<STT_Input, STT_Output, STT_State> tester(input, sttStateManager, sttOutputManager);
	MoveTester<STT_Input, STT_Output, STT_State, STT_Move_Swap> swap_move_test(input, sttStateManager, sttOutputManager, sttSwapNeighbourExplorer, "Swap move", tester); 
	MoveTester<STT_Input, STT_Output, STT_State, STT_Move_ChangeHospitality> changeOspitality_move_test(input, sttStateManager, sttOutputManager, sttCHNeighbourExplorer, "Change ospitality move", tester); 


	// solver
	SimpleLocalSearch<STT_Input, STT_Output, STT_State> solver(input, sttStateManager, sttOutputManager, "Solver");

	
	if (!CommandLineParameters::Parse(argc, argv, true, false))
		return 1;

	if (!instance.IsSet())
	{
		cout << "Error: --main::instance=filename option must always be set" << endl;
		return 1;
	}

	if(!method.IsSet()){

		if (init_state.IsSet()){
			tester.RunMainMenu(init_state);
		}else{ 
			tester.RunMainMenu();
		}

	}else{
		if(method == string("HC")){
			solver.SetRunner(sttSwapHC);
		}else{
			if(method == string("SD")){
				solver.SetRunner(sttSwapSD);
			}else{
				if(method == string("SA")){
					solver.SetRunner(sttSwapSA);
				}else{
					if(method == string("TS")){
						solver.SetRunner(sttSwapTS);
					}else{
						cout << "No solver found for the provided method" << endl;
						return 1;
					}
					
				}
			}	
		}
		

		//cout << "Running solver on input instance..." << endl;
		SolverResult<STT_Input, STT_Output> result = solver.Solve();
		STT_Output timetabling = result.output;

		if(method == string("HC")){
			solver.SetRunner(sttCHHC);
		}else{
			if(method == string("SD")){
				solver.SetRunner(sttCHSD);
			}else{
				if(method == string("SA")){
					solver.SetRunner(sttCHSA);
				}else{
					if(method == string("TS")){
						solver.SetRunner(sttCHTS);
					}else{
						cout << "No solver found for the provided method" << endl;
						return 1;
					}
				}
			}
		}
		
		SolverResult<STT_Input, STT_Output> secondResult = solver.Resolve(timetabling);
		cout << "Cost: " << result.cost.total << " -> " << secondResult.cost.total << " in time: " << (result.running_time + secondResult.running_time) << endl;
		STT_Output finalTimetabling = secondResult.output;

		cout << finalTimetabling;

		// STT_State state(input);
		
		// //converting output to state, to check the cost of the solution
		// sttOutputManager.InputState(state, finalTimetabling);

		// cout << state; //to eventually check the solution with minizinc

 		// int replayedCCViolations = replayedMatchCC.ComputeCost(state);
		// int bigmatchesCCViolations = bigMatchesCC.ComputeCost(state);
		// int forbiddenInMCCViolations = forbiddenInMCC.ComputeCost(state);
		// int complementaryCCViolations = complementaryCC.ComputeCost(state);
		// int seqOfHomeGamesCCViolations = seqOfHomeGamesCC.ComputeCost(state);
		// int seqOfAwayGamesCCViolations = seqOfAwayGamesCC.ComputeCost(state);
		// int forbiddenTogetherCCViolations = forbiddenTogetherCC.ComputeCost(state);
		// int shoulPlayAtHomeCCViolations = shoulPlayAtHomeCC.ComputeCost(state);

		// cout << "Hard Constraint costs: {" << replayedCCViolations << ", " <<  bigmatchesCCViolations << ", " << forbiddenInMCCViolations << ", " << complementaryCCViolations << ", " << seqOfHomeGamesCCViolations << ", " << seqOfAwayGamesCCViolations << "}" << endl;
		// cout << "Soft Constraint costs: {" << forbiddenTogetherCCViolations << ", " << shoulPlayAtHomeCCViolations << "}" << endl;

	}
}