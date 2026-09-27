#include <algorithm>
#include <iostream>
#include <vector>
#include "structs.h"

using namespace std;

// Animal template { .score = { }, .anchor = , .middle = , {0,0,0,0,0,0} };
//                                                  {T, TR, BR, B, BL, TL}
//ANIMAL CARDS
Animal Gecko { .score =  {0,5,10,16}, .anchor = Direction::BR, .middle = State::Field, {0,0,State::Building2,0,0,State::Field} };
Animal Shrew { .score = {0,5,10,17}, .anchor = -1, .middle = State::Building2, {0,0,State::Field,0,State::Field,0} };
Animal Flamingo { .score = {0,4,10,16}, .anchor = -1, .middle = State::River, {0,0,0,0,State::Field,State::Field} };
Animal Meerkat { .score = {0,2,5,9,14}, .anchor = -1, .middle = State::Mountain1, {0,0,0,0,0,State::Field} };
Animal Raccoon  { .score = {0,6,12}, .anchor = -1, .middle = State::Field, .neighbors = {0,0,State::River,State::River,State::River,0} };
Animal Warthog { .score = {0,4,8,13}, .anchor = -1, .middle = State::Tree2, {0,0,0,0,0,State::Building2} };

//SPIRITS


//ANIMAL PLACEMENT
// updates possible 'card' anchor positions based on the recent token placement
void update_anchor_positions(Animal &card, vector<Tile> &status, int node) {
    // were gonna check the placed tile and all its neighbors as the center of the shape
    vector<int> tiles; 
    tiles.push_back(node);
    for(int x : board[node]) {
        if(x<0) continue;
        tiles.push_back(x);
    }
    
    // reset previous set anchors
    if(card.anchor == -1) {
        for(int x : tiles) card.placement[x] = false; 
    } else {
        //for every neighbor tile that has the current card anchor we need to
        //check its neighbors as the middle if the anchor still fits
        for(int i = 1; i < tiles.size(); i++) {
            //hmmm so far it works without this, but i might need to check a few more tests...
        }
    }

    for(int i = 0; i < tiles.size(); i++) {
        int tile = tiles[i]; // <- this is the middle node of our shape on the board
        if(status[tile].state_id() == card.middle) { //check middle

            if(card.anchor == -1) { //one middle anchor

                //check anchor
                if(status[tile].animal) continue;

                //check neigbors & rotations
                vector<int> states(6,0);
                for(int i = 0; i < 6; i++) {
                    int neighbor = board[tile][i];
                    if(neighbor<0) continue;  
                    states[i] = status[neighbor].state_id();
                }
                for(int i = 0; i < 6; i++) {
                    bool match = true;
                    for(int j = 0; j < 6; j++) {
                        if(card.neighbors[j]==0) continue;
                        if(card.neighbors[j]!=states[j]) { match=false; break; }
                    }
                    if(match) { card.placement[tile]=true;  break; }
                    std::rotate(states.begin(), states.begin()+1, states.end());
                }
            
            } else { //multiple non-middle anchors
                //check neigbors & rotations
                vector<int> states(6,0);
                for(int i = 0; i < 6; i++) {
                    int neighbor = board[tile][i];
                    if(neighbor<0) continue;  
                    states[i] = status[neighbor].state_id();
                }
                vector<int> anchors;
                for(int i = 0; i < 6; i++) {
                    bool match = true;
                    for(int j = 0; j < 6; j++) {
                        if(card.neighbors[j]==0) continue;
                        if(card.neighbors[j]!=states[j]) { match=false; break; }
                    }
                    //rotate back to locate anchor on board
                    int dir = (card.anchor+i) % 6; 
                    //check anchor
                    int anchor_tile = board[tile][dir];
                    if(anchor_tile>0 && status[anchor_tile].animal || anchor_tile<0) match = false;
                    if(match) anchors.push_back(anchor_tile);
                    std::rotate(states.begin(), states.begin()+1, states.end());
                }
                for(int x : anchors) card.placement[x]=true;
            }

        }
    }
}

// 13 14 18 {19 20} {26 27} {28 29}

// 0:
// 13 27 28 18 26