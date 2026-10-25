#include <algorithm>
#include <iostream>
#include <vector>
#include "structs.h"

using namespace std;

// Animal template { .score={0}, .name="", .anchor=, .middle=, {0,0,0,0,0,0} };
//                                                    {T, TR, BR, B, BL, TL}
// limitations: shapes can only encompass a 1 hop neigborhood of a hex and the root hex must not be empty 
// (AKA the maximum width of the shape is 3 hexes long and you cant have the middle hex empty)
//ANIMAL CARDS
Animal Gecko { .score={0,5,10,16}, .name="Gecko", .anchor=Direction::BR, .middle = State::Field, {0,0,State::Building2,0,0,State::Field} };
Animal Shrew { .score={0,5,10,17}, .name="Shrew", .anchor=-1, .middle=State::Building2, {0,0,State::Field,0,State::Field,0} };
Animal Flamingo { .score={0,4,10,16}, .name="Flamingo", .anchor=-1, .middle=State::River, {0,0,0,0,State::Field,State::Field} };
Animal Meerkat { .score={0,2,5,9,14}, .name="Meerkat", .anchor=-1, .middle=State::Mountain1, {0,0,0,0,0,State::Field} };
Animal Raccoon  { .score={0,6,12}, .name="Raccoon", .anchor=-1, .middle=State::Field, {0,0,State::River,State::River,State::River,0} };
Animal Warthog { .score={0,4,8,13}, .name="Warthog", .anchor=-1, .middle=State::Tree2, {0,0,0,0,0,State::Building2} };
Animal Ladybug { .score={0,2,5,8,12,17}, .name="Ladybug", .anchor=-1, .middle=State::Field, {0,0,0,0,0,State::Bush} };
Animal Frog { .score={0,2,4,6,10,15}, .name="Frog", .anchor=-1, .middle=State::River, {0,0,0,0,0,State::Bush} };
Animal Koala { .score={0,3,6,10,15}, .name="Koala", .anchor=-1, .middle=State::Tree2, {0,0,0,0,0,State::Bush} };
Animal Duck { .score={0,2,4,8,13}, .name="Duck", .anchor=-1, .middle=State::River, {0,0,0,0,0,State::Building2} };
Animal Bat { .score={0,3,6,10,15}, .name="Bat", .anchor=-1, .middle=State::Mountain1, {0,0,0,0,0,State::Tree3} };
Animal Fish { .score={0,3,6,10,16}, .name="Fish", .anchor=-1, .middle=State::River, {0,0,0,0,0,State::Mountain3} };
Animal Squirrel { .score={0,4,9,15}, .name="Squirrel", .anchor=-1, .middle=State::Building2, {0,0,0,0,0,State::Tree3} };
Animal Otter { .score={0,5,10,16}, .name="Otter", .anchor=Direction::BR, .middle=State::Bush, {0,0,State::River,0,0,State::Bush} };
Animal FennecFox { .score={0,4,9,16}, .name="FennecFox", .anchor=Direction::BR, .middle=State::Mountain1, {0,0,State::Mountain1,0,0,State::Field} };
Animal Rabbit { .score={0,5,10,17}, .name="Rabbits", .anchor=Direction::BR, .middle=State::Bush, {0,0,State::Bush,0,0,State::Building2} };
Animal Alligator { .score={0,4,9,15}, .name="Alligator", .anchor=Direction::BR, .middle=State::River, {0,0,State::River,0,0,State::Tree3} };
Animal MantaRay { .score={0,4,10,16}, .name="MantaRay", .anchor=-1, .middle=State::River, {0,0,0,0,State::Mountain1,State::Mountain1} };
Animal Wolf { .score={0,4,10,16}, .name="Wolf", .anchor=-1, .middle=State::Tree3, {0,0,0,0,State::Field,State::Field} };
Animal Parrot { .score={0,4,9,14}, .name="Parrot", .anchor=-1, .middle=State::Tree2, {0,0,0,0,State::River,State::River} };
Animal Penguin { .score={0,4,10,16}, .name="Penguin", .anchor=-1, .middle=State::Mountain1, {0,0,State::River,0,State::River,0} };
Animal Peacock { .score={0,5,10,17}, .name="Peacock", .anchor=-1, .middle=State::Building2, {0,0,State::River,0,State::River,0} };
Animal ArcticFox { .score={0,5,10,17}, .name="ArcticFox", .anchor=-1, .middle=State::Field, {0,0,State::Tree2,0,State::Tree2,0} };
Animal Kingfisher { .score={0,5,11,18}, .name="Kingfisher", .anchor=-1, .middle=State::Tree3, {0,0,State::River,0,State::River,0} };
Animal Falcon { .score={0,5,11}, .name="Falcon", .anchor=-1, .middle=State::Mountain3, {0,0,0,0,0,State::Field} };
Animal Alpaca { .score={0,5,12}, .name="Alpaca", .anchor=Direction::BR, .middle=State::Field, {0,0,State::Field,0,0,State::Mountain2} };
Animal Panther { .score={0,5,11}, .name="Panther", .anchor=Direction::BR, .middle=State::Tree2, {0,0,State::Field,0,0,State::Tree2} };
Animal Raven { .score={0,4,9}, .name="Raven", .anchor=-1, .middle=State::Field, {0,0,State::Building2,0,State::Building2,0} };
Animal BrownBear { .score={0,5,11}, .name="BrownBear", .anchor=-1, .middle=State::Bush, {0,0,0,0,State::Mountain2,State::Mountain2} };
Animal ArcticMonkey { .score={0,5,11}, .name="SnowMonkey", .anchor=-1, .middle=State::Mountain2, {0,0,0,0,State::River,State::River} };
Animal Hedgehog { .score={0,5,12}, .name="Hedgehog", .anchor=-1, .middle=State::Building2, {0,0,0,0,State::Tree2,State::Tree2} };
Animal Bumblebee { .score={0,8,18}, .name="Bumblebee", .anchor=-1, .middle=State::Tree2, {0,0,State::Field,State::Field,State::Field,0} };


//SPIRITS


//ANIMAL PLACEMENT
// updates possible 'card' anchor positions based on the recent token placement 
// (!Only for middle anchor cards, the non-middle ones do a whole board recompute)
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
        for(int x : tiles) card.placement[x] = 0; 
    } else {
        // technically non-middle anchor cards need a 2-hop neigbhorhood recompute,
        // but on the classic game board of 23 tiles thats the majority of the board for any tile
        
        tiles.clear();
        for(int i = 0; i < nodes; i++) tiles.push_back(i); 
        fill(card.placement.begin(),card.placement.end(),0);
    }

    for(int i = 0; i < tiles.size(); i++) {
        int tile = tiles[i]; // <- this is the middle node of our shape on the board
        if(status[tile].state_id() == card.middle) { //check middle

            if(card.anchor == -1) { //one middle anchor

                //check anchor
                if(status[tile].animal) continue;

                //check neigbors & rotations
                vector<int> states(6,0);
                for(int j = 0; j < 6; j++) {
                    int neighbor = board[tile][j];
                    if(neighbor<0) continue;  
                    states[j] = status[neighbor].state_id();
                }
                for(int j = 0; j < 6; j++) {
                    bool match = true;
                    for(int k = 0; k < 6; k++) {
                        if(card.neighbors[k]==0) continue;
                        if(card.neighbors[k]!=states[k]) { match=false; break; }
                    }
                    if(match) { card.placement[tile]=1;  break; }
                    std::rotate(states.begin(), states.begin()+1, states.end());
                }
            
            } else { //multiple non-middle anchors

                //check neigbors & rotations
                vector<int> states(6,0);
                for(int j = 0; j < 6; j++) {
                    int neighbor = board[tile][j];
                    if(neighbor<0) continue;  
                    states[j] = status[neighbor].state_id();
                }
                vector<int> anchors;
                for(int j = 0; j < 6; j++) {
                    bool match = true;
                    for(int k = 0; k < 6; k++) {
                        if(card.neighbors[k]==0) continue;
                        if(card.neighbors[k]!=states[k]) { match=false; break; }
                    }
                    //rotate back to locate anchor on board
                    int dir = (card.anchor+j) % 6; 
                    //check anchor
                    int anchor_tile = board[tile][dir];
                    if(anchor_tile<0 || status[anchor_tile].animal) match = false;
                    if(match) anchors.push_back(anchor_tile);
                    std::rotate(states.begin(), states.begin()+1, states.end());
                }
                for(int x : anchors) card.placement[x]=1;
            }

        }
    }
}

// 13 14 18 {19 20} {26 27} {28 29}

// 0:
// 13 27 28 18 26