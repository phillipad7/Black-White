#include "PAI.hpp"

#include <ics46/factory/DynamicFactory.hpp>
#include <memory>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

ICS46_DYNAMIC_FACTORY_REGISTER(OthelloAI, chaoyiw1::PAI, "A Phil's AI (Required)");


chaoyiw1::PAI::PAI()
{
}


pair<int, int> chaoyiw1::PAI::chooseMove(const OthelloGameState& state)
{
	unique_ptr<OthelloGameState> gscopy = state.clone();

	pair<int, int> thisMove;
	
	vector<pair<int, int> > val_moves = validMoves(gscopy);

	int ai_turn, best_score = -999;

	if(state.isBlackTurn())
		ai_turn = 0;
	else
		ai_turn = 1;
	

	for(int i=0; i<val_moves.size(); i++)
	{
		unique_ptr<OthelloGameState> gstate_copy = state.clone();

		gstate_copy-> makeMove(val_moves[i].first, val_moves[i].second);

		int score = search(gstate_copy, 8, ai_turn);

		// cout<<"--ai_turn turn:	"<<ai_turn<<endl;
		// cout<<"--score_ in chooseMove: "<<score<<endl;

		if(score>best_score)
		{
			best_score = score;

			thisMove.first = val_moves[i].first;
			thisMove.second = val_moves[i].second;

		}

		// cout<<"best_score:	"<<best_score<<endl;
		// cout<<"~~~	!!!	~~~		~~~	!!!	~~~\n";
		
	}
	// cout<<"best_ outside loop:	"<<best_score<<endl;
	// cout<< "~~~~~~~~~~~~	next move	~~~~~~~~~~~~\n\n";
	return thisMove;

}

vector< pair<int,int> > chaoyiw1::PAI::validMoves(unique_ptr<OthelloGameState>& gs)
{
	vector< pair<int,int> > moves;

	vector< pair<int,int> > corners;
	vector< pair<int,int> > dead;
	vector< pair<int,int> > better;
	vector< pair<int,int> > edges;

	int width = gs->board().width();
	int height = gs->board().height();

	for (int w = 0; w < gs->board().width(); w++)
		for (int h = 0; h < gs->board().height(); h++)
		{
			if (gs->isValidMove(w,h))
			{
				if ((w==0 && h==0) || (w==0 && h==height-1) || (w==width-1 && h==0) || (w==width-1 && h==height-1))
					corners.push_back(pair<int,int> (w,h));
				else if ((w==2 && h==2) || (w==2 && h==height-3) || (w==width-3 && h==2) || (w==width-3 && h==height-3))
				{
					better.push_back(pair<int,int> (w,h));
				}
				else if ((w==1 && h==1) || (w==1 && h==height-2) || (w==width-2 && h==1) || (w==width-2 && h==height-2))
				{
					dead.push_back(pair<int,int>(w,h));
				}
				else
					moves.push_back(pair<int,int> (w,h));

				if( ((w==0||w==width-1) && h>1 && h<height-2 ) || ( (h==0 || h==height-1) && w>1 && w<width-2) )
					edges.push_back(pair<int,int> (w,h));

			}
		}
	// cout<<"\nsize of edges: "<<edges.size()<<endl;
	// for(int i=0; i<edges.size(); i++)
	// 	cout<<"("<<edges[i].first<<","<<edges[i].second<<")"<<endl;


	// cout<<"\nsize of moves: "<<moves.size()<<endl;
	// for(int i=0; i<moves.size(); i++)
	// 	cout<<"("<<moves[i].first<<","<<moves[i].second<<")"<<endl;
	
	// cout<<"\nsize of corners: "<<corners.size()<<endl;
	// for(int i=0; i<corners.size(); i++)
	// 	cout<<"("<<corners[i].first<<","<<corners[i].second<<")"<<endl;

	// cout<<"-------"<<endl;


	if (corners.size() > 0)
		return corners;
	else if (edges.size() > 0)
		return edges;
	else if (better.size() > 0)
		return better;

	else if (moves.size() > 0)
		return moves;
	else
		return dead;
}






int chaoyiw1::PAI::search(unique_ptr<OthelloGameState>& gs,int depth, int turn)
{
	vector< pair<int,int> > candidate_moves = validMoves(gs);

	int high_score = -9999, low_score = 9999;
	int score;

	if ((depth == 0) || gs->isGameOver())
		return evaluate(gs, turn);

	else
	{
		if( (gs->isBlackTurn() && turn == 0) || (gs->isWhiteTurn() && turn == 1) )
		{
			for(auto a_move: candidate_moves)
			{
				unique_ptr<OthelloGameState> copy_gs = gs->clone();
				// if(!gs->isGameOver())
				copy_gs->makeMove(a_move.first, a_move.second);

				score = search(copy_gs, depth-1, turn);

				if (score > high_score)
					high_score = score;
				
				//cout<<"high_score:	"<<high_score<<endl; 
			}
			return high_score;
		}
		else
		{
			for(auto a_move: candidate_moves)
			{
				unique_ptr<OthelloGameState> copy_gs = gs->clone();
				copy_gs->makeMove(a_move.first, a_move.second);

				score = search(copy_gs, depth-1, turn);
				if (score < low_score)
					low_score = score;

				//cout<<"low_score:	"<<low_score<<endl;
			}
			return low_score;
		}
	}
}


int chaoyiw1::PAI::evaluate(std::unique_ptr<OthelloGameState>& gs, int turn)
{
	if (turn==0)
		return (gs->blackScore() - gs->whiteScore());
	else
		return (gs->whiteScore() - gs->blackScore());
}