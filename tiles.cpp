#include <vector>
#include "structs.h"

using namespace std;

int Tile::state_id() { return read_type(stack); };

//this is very inefficient hahhah
int read_type(vector<int> stack) { 
    vector<int> tile;

    tile = {-1,-1,-1};
    if(stack == tile) return State::Empty;

    tile = {Token::Water,-1,-1};
    if(stack == tile) return State::River;

    tile = {Token::Fields,-1,-1};
    if(stack == tile) return State::Field;

    tile = {Token::Grass,-1,-1};
    if(stack == tile) return State::Bush;

    tile = {Token::Trunk,-1,-1};
    if(stack == tile) return State::Trunk1;

    tile = {Token::Building,-1,-1};
    if(stack == tile) return State::Building1;

    tile = {Token::Mountain,-1,-1};
    if(stack == tile) return State::Mountain1;

    tile = {Token::Trunk,Token::Grass,-1};
    if(stack == tile) return State::Tree2;

    tile = {Token::Trunk,Token::Trunk,-1};
    if(stack == tile) return State::Trunk2;
    
    tile = {Token::Trunk,Token::Building,-1};
    if(stack == tile) return State::Building2;
    tile = {Token::Building,Token::Building,-1};
    if(stack == tile) return State::Building2;
    tile = {Token::Mountain,Token::Building,-1};
    if(stack == tile) return State::Building2;

    tile = {Token::Mountain,Token::Mountain,-1};
    if(stack == tile) return State::Mountain2;

    tile = {Token::Trunk,Token::Trunk,Token::Grass};
    if(stack == tile) return State::Tree3;

    tile = {Token::Mountain,Token::Mountain,Token::Mountain};
    if(stack == tile) return State::Mountain3;

    return 0;
}