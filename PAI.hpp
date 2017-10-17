#include "OthelloAI.hpp"

#include <memory>
#include <iostream>
#include <vector>

#ifndef PAI_HPP
#define PAI_HPP

using namespace std;

namespace chaoyiw1
{
	class PAI: public OthelloAI
	{
	public:

		PAI();

		virtual pair<int,int> chooseMove(const OthelloGameState& gs);

		vector< pair<int,int> > validMoves(unique_ptr<OthelloGameState>& gs);
	
		int search(unique_ptr<OthelloGameState>& gs,int depth, int turn);

		int evaluate(unique_ptr<OthelloGameState>& gs, int turn);

	};
}




#endif

