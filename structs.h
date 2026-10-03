#pragma once
#include <vector>
#include <string>
#include <random>

using std::vector;
using std::string;

//RNG
inline std::mt19937 rng{std::random_device{}()};

//GLOBALS
const int pool_size = 3;
const int pool_count = 5;
const int max_tile_stack = 3; //dont change this...xD

//GRAPH
const int cols = 3, colsize = 5; // pls have at least 2 columns and size above 1, bitte 🙏 (otherwise it explodes)
const int nodes = cols*colsize + (cols-1)*(colsize-1); // ! - mustnt exceed 1000
extern const std::vector<std::vector<int>> board;

//TOKENS
enum Token : int {
    Building, Fields, Grass, Mountain, Trunk, Water
};

const vector<string> token_types = {"BUI", "FIE", "GRA", "MOU", "TRU", "WAT"};
const vector<int> token_counts = {15,19,19,23,21,23};


//TILE STATES
enum State : int {
    Empty, River, Field, Bush, Building1, Building2,
    Trunk1, Trunk2, Tree2, Tree3, Mountain1, Mountain2, Mountain3
};

const vector<string> tile_states = {
    "empty", "river", "field", "bush", "building1", "building2", 
    "trunk1", "trunk2", "tree2", "tree3", "mountain1", "mountain2", "mountain3" 
};

//TILE STRUCTURE
struct Tile {
    bool animal = false;
    bool spirit = false;
    vector<int> stack = vector<int>(max_tile_stack, -1); // {-1,-1,-1} from left to right is bottom to top
    int state_id();
};

//DIRECTION HELPER (clockwise from the top)
enum Direction : int {
    T, TR, BR, B, BL, TL
};

//ANIMAL STRUCTURE
struct Animal { // the shape is limited to one hex and its neighbors
    int uses = 0;
    vector<int> score;
    string name = "";

    int anchor; // -1 if middle
    int middle; // middle tile type
    vector<int> neighbors = vector<int>(6);
    vector<int> placement = vector<int>(nodes,0);
};

//ANIMALS
extern Animal Gecko; 
extern Animal Shrew; 
extern Animal Flamingo; 
extern Animal Meerkat; 
extern Animal Raccoon;  
extern Animal Warthog; 

//FUNCTION HEADERS
//backend
int skim_top(Tile t);
int read_type(vector<int> stack);
vector<vector<int>> board_constructor(int columns, int colsize);
void update_anchor_positions (Animal &card, vector<Tile> &status, int node);
void placement_mask(int token, vector<Tile> &status, vector<int> &mask);

//init
vector<int> init_token_bag(vector<int> tilecounts);
vector<vector<int>> init_token_board(int pools, vector<int> &token_bag);

//actions
vector<int> select_token_pool(vector<vector<int>> &token_board, vector<int> &token_bag, int select);
void place_token(int token, int node, vector<Tile> &status);

//scoring
vector<int> tree_score(vector<Tile> &status);
vector<int> mountain_score(vector<Tile> &status);
int building_score(vector<Tile> &status);
int fields_score(vector<Tile> &status);
int river_score(vector<Tile> &status);


