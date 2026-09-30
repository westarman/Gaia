#include <iostream>
#include <algorithm>
#include <cassert>
#include <vector>
#include <random>
#include <string>
#include "structs.h"

using namespace std;

//FUNCTIONS
// initializes the token bag
vector<int> init_token_bag(vector<int> tilecounts) {
    vector<int> bag;
    for(int i = 0; i < tilecounts.size(); i++) {
        for(int j = 0; j < tilecounts[i]; j++) bag.push_back(i);   
    }
    shuffle(bag.begin(), bag.end(), rng);
    return bag;
}

// removes the 'select' pool and restocks the board from the bag
vector<int> select_token_pool(vector<vector<int>> &token_board, vector<int> &token_bag, int select) {
    vector<int> pool = token_board[select];
    token_board[select].clear();
    for(int i = 0; i < pool_size && !token_bag.empty(); i++) {
        token_board[select].push_back(token_bag.back());
        token_bag.pop_back();
    } 
    return pool;
}

// initializes the token board
vector<vector<int>> init_token_board(int pools, vector<int> &token_bag) {
    vector<vector<int>> token_board(pools);
    for(int i = 0; i < pools; i++) {
        select_token_pool(token_board, token_bag, i);
    }
    return token_board;
}

// needs mask for invalid placement in the decision matrix?
void place_token(int token, int node, vector<Tile> &status) {
    for(int i = 0; i < max_tile_stack; i++) {
        if(status[node].stack[i]==-1) { status[node].stack[i]=token; return; }
    }
    string error = "failed to place token " + token_types[token] + " at node " + to_string(node) + '\n';
    assert(false && error.c_str());
}

// returns the top most token of the tile stack
int skim_top(Tile t) { 
    for(int x : t.stack) if(x != -1) return x;
    return -1;
}

// returns mask for valid token placement
void placement_mask(int token, vector<Tile> &status, vector<int> &mask) {
    fill(mask.begin(),mask.end(),0);
    for(int i = 0; i < nodes; i++) {
        
        vector<int> non_stackable = {Token::Fields, Token::Water, Token::Grass};
        if(find(non_stackable.begin(), non_stackable.end(), status[i].stack[0]) != non_stackable.end()) continue;

        if(status[i].stack[0] == -1) { mask[i] = 1; continue; } //stack is empty
        if(status[i].stack[2] != -1) continue; //stack is full

        switch(token) {
            
            case Token::Building: {
                if(status[i].stack[1] != -1) break;

                vector<int> allowed = {Token::Mountain, Token::Building, Token::Trunk, -1};
                if(find(allowed.begin(), allowed.end(), skim_top(status[i])) != allowed.end()) mask[i] = 1;

                break;
            }

            case Token::Fields: 
                if(status[i].stack[0] == -1) mask[i] = 1;
                break;
            
            case Token::Grass: 
                if(skim_top(status[i]) == Token::Trunk) mask[i] = 1;
                break;
            
            case Token::Mountain: {
                if(skim_top(status[i]) == Token::Mountain) mask[i] = 1;
                break;
            }
            
            case Token::Trunk: 
                if(status[i].stack[1] != -1) break;
                if(skim_top(status[i]) == Token::Trunk) mask[i] = 1;
                break;
            
            case Token::Water: 
                if(status[i].stack[0] == -1) mask[i] = 1;
                break;
        }
    }
}

//DEBUG STUFFF!!!!
/*int main() {
    
    return 0;

    
    // initial animal deck
    vector<Animal> card_deck = {Gecko, Shrew, Flamingo, Meerkat, Raccoon, Warthog};
    
    cout << "Neighborhood graph: \n";
    for(int i = 0; i < nodes; i++) {
        cout << i << ": ";
        for(auto node : board[i]){
            cout << node << " ";
        }
        cout << "\n";
    } 

    vector<int> bag = init_token_bag(token_counts);
    cout << "\nBag contents(before init): (size=" << bag.size() << ")\n";
    for(int x : bag) cout << x << " ";
    cout << '\n';

    vector<vector<int>> tboard = init_token_board(pool_count,bag);
    cout << "Token board: \n";
    for(int i = 0; i < pool_count; i++) {
        cout << i << ": ";
        for(auto node : tboard[i]){
            cout << node << " ";
        }
        cout << "\n";
    }

    cout << "\nBag contents(after init): (size=" << bag.size() << ")\n";
    for(int x : bag) cout << x << " ";
    cout << '\n';
    
    // debug board from the HAR_rules pdf
    vector<Tile> board_status(nodes);
    vector<int> t = {Token::Fields, Token::Fields, Token::Water, Token::Water, Token::Grass, Token::Water, 
        Token::Water, Token::Fields, Token::Water, Token::Water, Token::Trunk, Token::Building, Token::Fields, 
        Token::Fields, Token::Water, Token::Fields, Token::Mountain, Token::Building, Token::Mountain, Token::Trunk, 
        Token::Grass, Token::Mountain, Token::Mountain, Token::Mountain, Token::Mountain, Token::Mountain, 
        Token::Building, Token::Building, Token::Mountain, Token::Mountain};
    vector<int> n = {0,1,2,3,4,5,6,7,8,9,10,10,11,12,13,14,15,15,16,17,17,18,18,18,19,20,21,21,22,22};
    
    vector<int> order;
    for(int j = 0; j < t.size(); j++) order.push_back(j);
    shuffle(order.begin(),order.end(),rng);
    
    //hahhah owww boy this was convoluted to setup
    //it basically makes a random placement order of the above board set, its different each time and
    //it makes sure it doesnt make a few possible illegal stacks
    //horribly inefficient and non-expandable xD
    for(int j = 0; j < order.size(); j++) {
        if(n[order[j]] == 10 && t[order[j]] == Token::Building && board_status[n[order[j]]].state_id() == State::Empty ||
            n[order[j]] == 15 && t[order[j]] == Token::Building && board_status[n[order[j]]].state_id() == State::Empty ||
            n[order[j]] == 17 && t[order[j]] == Token::Grass && board_status[n[order[j]]].state_id() == State::Empty) {
            order.push_back(order[j]); 
        } else {
            place_token(t[order[j]], n[order[j]], board_status);
            for(int i = 0; i < card_deck.size(); i++) { update_anchor_positions(card_deck[i], board_status,n[order[j]]); }
        }
    }
    
    vector<int> tscore = tree_score(board_status);
    cout << "\nTree score: ";
    for(int x : tscore) cout << x << " ";
    cout << "\n";

    vector<int> mscore = mountain_score(board_status);
    cout << "Mountain score: ";
    for(int x : mscore) cout << x << " ";
    cout << "\n";

    cout << "Building score: " << building_score(board_status) << "\n";
    cout << "Fields score: " << fields_score(board_status) << "\n";
    cout << "River score: " << river_score(board_status) << "\n";

    cout << "Placement order: ";
    for(int x : order) cout << x << " ";
    cout << "\n\n";
    
    cout << "                         0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2\n";
    for(int i = 0; i < card_deck.size(); i++) {
        cout << "Card " << i << " anchor positions: "; 
        for(bool x : card_deck[i].placement) {
            cout << x << " ";
        }
        cout << '\n';
    }

    // 1s on 1 and 7

    
    //Expected score:
    //Trees {1,3,0}
    //Mountains {3,0,7}
    //Fields {10}
    //River {7}
    //Buildings {10}
    
    cout << "[t  ]\n";
    cout << "[w  ]\n";
    cout << "[b  ]\n";
    cout << "[m  ]\n";
    cout << "[f  ]\n";
    cout << "[g  ]\n";
    cout << "[tt ]\n";
    cout << "[tb ]\n";
    cout << "[bb ]\n";
    cout << "[mb ]\n";
    cout << "[tg ]\n";
    cout << "[ttg]\n";
    cout << "[mmm]\n";
    cout << "[   ]\n";
    // "1 5 1 5(empty) 1 5 ... 5 1 \n"
    //6 "      "
    cout << " /000\\       /005\\       /010\\\n";
    cout << "| ttg |     | mb  |     | w   |\n";
    cout << " \\ x / /003\\ \\ x / /008\\ \\ x /\n";
    cout << "   |  | w   |  |  | w   |  |   \n";
    cout << " /001\\ \\ x / /006\\ \\ x / /011\\\n";
    cout << "| g   |  |  |     |  |  | bb  |\n";
    cout << " \\ x / /004\\ \\ x / /009\\ \\ x /\n";
    cout << "   |  | f   |  |  | w   |  |   \n";
    cout << " /002\\ \\ x / /007\\ \\ x / /012\\\n";
    cout << "| f   |     |     |     | w   |\n";
    cout << " \\ x /       \\ x /       \\ x /\n";

    return 0;
}*/

/*
columns = 3
colsize = 4

edges:
2   3   2
4 5 6 5 4
4 6 6 6 4 
2 5 3 5 2

node idx:
0    7    14
1 4  8 11 15
2 5  9 12 16
3 6 10 13 17

graph:
0: 1,4
1: 0,2,4,5
2: 1,3,5,6
3: 2,6
4: 0,1,5,7,8
...
17: 13,16


token: t,w,b,m,f,g
possible states: [t  ],[w  ],[b  ],[m  ],[f  ],[g  ]
                 [tt ],[tb ],[bb ],[mb ],[mm ],[tg ]
                 [ttg],[mmm]
                 [   ]


 /022\ 
| ttf | 
 \ x /

 line 1: just an xxx id of the node
 line 2: the xxx stack, each letter is different color representing a token (t = brown = trunk etc.), left is bottom, right is top
 line 3: the anchor position or when giving command 'valid token_letter' it represents a visual aid for valid placement of what token you want
 (so we type 'valid t' and it shows an x if we can place the t token there, also can work for anchor placement, otherwise normally it just shows x if an anchor is present there)
 */