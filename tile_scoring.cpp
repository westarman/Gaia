#include <queue>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include "structs.h"

using namespace std;

//checks wether at least one neighbor of 'root' is one of the types in 'types'
bool neighbor_type(vector<Tile> &status, vector<int> types, int root) {
    for(int neighbor : board[root]) {
        if(neighbor < 0) continue;
        int state = status[neighbor].state_id();
        for(int type : types) {
            if(state == type) return true;
        }
    }
    return false;
}

//returns all tiles of type 'type'
vector<int> find_types(int type, vector<Tile> &status) {
    vector<int> types(0);
    for(int i = 0; i < status.size(); i++) {
        if(status[i].state_id() == type) types.push_back(i);
    }
    return types;
}

// theyre gonna be more descriptive than necessary but it will give us fun statistics :)
// also we could merge em but ehhh whatever...

// {bush = 1, tree1 = 3, tree2 = 7}
vector<int> tree_score(vector<Tile> &status) {
    vector<int> score(3,0);
    for(int i = 0; i < status.size(); i++) {
        int tile_state = status[i].state_id();

        switch (tile_state) {
            case State::Bush:  { score[0] += 1; break;}
            case State::Tree2:  { score[1] += 3; break; }
            case State::Tree3:  { score[2] += 7; break; }
            break; 
        }
    }
    return score;
}

// {mountain1 = 1, mountain2 = 3, mountain3 = 7}
vector<int> mountain_score(vector<Tile> &status) {
    vector<int> score(3,0);
    vector<int> mountains = {State::Mountain1,State::Mountain2,State::Mountain3};

    for(int i = 0; i < status.size(); i++) {
        int tile_state =  status[i].state_id();
        
        switch (tile_state) {
            case State::Mountain1: if(neighbor_type(status,mountains,i)) { score[0] += 1; break;}
            case State::Mountain2: if(neighbor_type(status,mountains,i)) { score[1] += 3; break; }
            case State::Mountain3: if(neighbor_type(status,mountains,i)) { score[2] += 7; break; }
            break; 
        }
    }
    return score;
}

int building_score(vector<Tile> &status) {
    //this has a bug... something with trees,trunks and bushes
    int score = 0;
    for(int i = 0; i < status.size(); i++) {
        unordered_set<int> uniques(0);
        if(status[i].state_id() == State::Building2) {
            for(int neighbor : board[i]) if(neighbor >= 0) uniques.insert(skim_top(status[neighbor]));
            if(uniques.size() > 2) score += 5;
        }
    }
    return score;
    //we could add info about what base token the building used, but it doesnt seem like an interesting statistic
}

// only groups of 2 or more fields give +5 score
int fields_score(vector<Tile> &status) {
    int score = 0;
    unordered_set<int> grouped(0);
    for(int i = 0; i < status.size(); i++) {
        
        if(status[i].state_id() == State::Field) {
            
            if(find(grouped.begin(), grouped.end(), i) == grouped.end()) {
                //field neighbors
                vector<int> nfields(0);
                for(int neighbor : board[i]) {
                    if(neighbor >= 0 && status[neighbor].state_id() == State::Field) nfields.push_back(neighbor);
                }
                if(nfields.empty()) continue;
                
                //check if any neigbhoring fields are grouped
                score += 5;
                for(int field : nfields) {
                    if(find(grouped.begin(), grouped.end(), field) != grouped.end()) { score -= 5; break; }
                }
                
                grouped.insert(i);
                //group any neighboring fields
                for(int field : nfields) grouped.insert(field);
            }
        }
    }
    return score;
}

//a graph diameter algorithm using BFS
int river_score(vector<Tile> &status) {
    int max = 0;
    vector<int> rivers = find_types(State::River, status);

    //BFS to find longest length
    for(int river : rivers) {
        queue<int> Q;
        Q.push(river);
        vector<bool> visited(nodes,false);
        visited[river] = true;
        int length = 1;
        vector<bool> layer(nodes,false);
        while (!(Q.empty())) {
            int u = Q.front(); Q.pop();
            for(int neighbor : board[u]) {
                if(neighbor < 0 || status[neighbor].state_id() != State::River) continue;
                if(!(visited[neighbor])) {
                    visited[neighbor] = true;
                    Q.push(neighbor);
                    if(!(layer[u])) length++; layer[u]=true; // this is for proper counting the length by layer
                }
            }
        }
        if(length > max) max = length;
    }   

    //Scoring the length
    int score;
    if(max < 2) score = 0;
    else if(max == 2) score = 2;
    else if(max>2 && max<6) score = (max-2)*3 + 2;
    else score = (max-5)*4 + 11;

    return score;
}