#include <iostream>
#include <algorithm>
#include <sstream>
#include <vector>
#include <thread>
#include <chrono>
#include "structs.h"

using namespace std;

/*
                                 &&
                               &&&&&                         &&
         &&&&                &&&&|&&&                       &&&&&
     &&&&& &&o             &&&&&&/ &&&& &&&              &&&&&|&&&
   & &&---\&&&&           && o /------\&&&&&            &&&&& /&&
 &&&&&&&&&&&-\&&           ---/       &\-&&&&&         &&&&&//&&&&
 &&&&&&    && \-          /             &&&&&&&&      /----/\ o&&&&    &
&&&           /    /--\  /         __      o &&&    _/       \ &&&&&&&&&
 o      /---/====/----\/\       //==\--\_        __/       &&\-----&&&&&
        //---|           \------/        \------/           &&&&&&&&&&&&&&&
       //  _____     _        _____            __     __      o & &   && o&
      //  / ___/__ _(_)__ _  / __(_)_ _  __ __/ /__ _/ /____  ____
      // / (_ / _ `/ / _ `/ _\ \/ /  ' \/ // / / _ `/ __/ _ \/ __/          ~
     //  \___/\_,_/_/\_,_/ /___/_/_/_/_/\_,_/_/\_,_/\__/\___/_/    w       ~~
 @@ //                                  ~~~~~~    *                      ~~~~~
@@@@@    ~~~~~~~~~~        w        ~~~~~~~~~~~~~~~~~~~~~~~~~~~ @    ~~~~~~~~
@@@@@@ ~~~~~~~~~~~~~~~~       * ~~~~~~~~~~   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
@@@@@@~~~~~     *  ~~~~~~~~~~~~~~~~~~~       w       /#      ~~~~~~~~~~~
@@@@@~~~  @   w    @    ~~~~~~~~~~                 \|;      *          
@@@@~~    |/
*/

vector<string> title = {
    "                                 &&\n",
    "                               &&&&&                         &&\n",
    "         &&&&                &&&&|&&&                       &&&&&\n",
    "     &&&&& &&o             &&&&&&/ &&&& &&&              &&&&&|&&&\n",
    "   & &&---\\&&&&           && o /------\\&&&&&            &&&&& /&&\n",
    " &&&&&&&&&&&-\\&&           ---/       &\\-&&&&&         &&&&&//&&&&\n",
    " &&&&&&    && \\-          /             &&&&&&&&      /----/\\ o&&&&    &\n",
    "&&&           /    /--\\  /         __      o &&&    _/       \\ &&&&&&&&&\n",
    " o      /---/====/----\\/\\       //==\\--\\_        __/       &&\\-----&&&&&\n",
    "        //---|           \\------/        \\------/           &&&&&&&&&&&&&&&\n",
    "       //  _____     _        _____            __     __      o & &   && o&\n",
    "      //  / ___/__ _(_)__ _  / __(_)_ _  __ __/ /__ _/ /____  ____\n",
    "      // / (_ / _ `/ / _ `/ _\\ \\/ /  ' \\/ // / / _ `/ __/ _ \\/ __/          ~\n",
    "     //  \\___/\\_,_/_/\\_,_/ /___/_/_/_/_/\\_,_/_/\\_,_/\\__/\\___/_/    w       ~~\n",
    " @@ //                                  ~~~~~~    *                      ~~~~~\n",
    "@@@@@    ~~~~~~~~~~        w        ~~~~~~~~~~~~~~~~~~~~~~~~~~~ @    ~~~~~~~~\n",
    "@@@@@@ ~~~~~~~~~~~~~~~~       * ~~~~~~~~~~   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n",
    "@@@@@@~~~~~     *  ~~~~~~~~~~~~~~~~~~~       w       /#      ~~~~~~~~~~~\n",
    "@@@@@~~~  @   w    @    ~~~~~~~~~~                 \\|;      *          \n",
    "@@@@~~    |/      \n"
};

vector<string> title_color = {
    "                                 gg",
    "                               ggggg                         gg",
    "         gggg                gggnonng                       ggggg",
    "     ggggg gge             gnnnggo gggg ggg              gggnnrggg",
    "   g ggooorgnng           nn e rrrrrrrrgnnng            gggnn rgg",
    " gggggnnngggrrgg           rrrr       nrrggggg         ggnnnrrnngg",
    " gnnnng    nn rr          r             nnnngggg      roooorr egggg    g",
    "ggg           r    rrrr  r         rr      e ngg    rr       r nnngggggg",
    " e       rrrroooorrrrrrrrr       rooorrrrr       rrrr       ggrrroonnnngg",
    "        rrrrrr           rrrrrrrr        rrrrrrrr           nnnnnnngggggggg",
    "       ro  ppppp     p        ppppp            pp     pp      e g g   gg eg",
    "      ro  p pppppp pppppp p  p pppppp p  pp ppp ppp pp ppppp  pppp",
    "      ro p pp p p pp p p pp pp pp p  p pp pp p p p pp ppp p pp ppp          b",
    "     ro  ppppppppppppppppp pppppppppppppppppppppppppppppppppppp    n       bb",
    " ss ro                                  bbbbbb    m                      bbccb",
    "sssss    bbbbbbbccc        n        bbbbbccccbbbbbbbbcccbbbbbbb m    bbbccccb",
    "ssssss bbbccccbbbbbbbbb       m bbbcccbbbb   bbbbcccccbbbbbbbbcccbbbbbbbbbb",
    "smmmssbbbbb     m  bbbccccbbbbbbbbbbbb       g       ny      bbbbbbbccbb",
    "mmmmmbbb  e   n    m   bbcccbbbbb                  nnn      m          ",
    "mmmmbb    gg       "
};

int rgb(char c) {
    switch(c) {
        case 'n': return 34;  //green
        case 'g': return 46;  //light green
        case 'o': return 94;  //brown
        case 'r': return 130; //light brown
        case 'p': return 165; //purple
        case 'b': return 33;  //blue
        case 'c': return 75;  //light blue
        case 'e': return 160; //red
        case 's': return 242; //gray
        case 'm': return 248; //light gray
        case 'y': return 226; //yellow
    }
    return 0;
}

/*
for (int i = 0; i < 256; ++i)
std::cout << "\033[48;5;" << i << "m " << i << " \033[0m" << (i % 16 == 15 ? "\n" : "");
*/

enum Menu : int {
    MainMenu, GameStart, Settings, MainTurn, EndGame, SimSettings, Simulation
};

enum Order : int {
    PickPool, PickCard, PlaceToken, PlaceAnchor, ViewToken, ViewAnchor, ClearView, EndTurn, Help
};

int state = Menu::MainMenu;

//GAME MEMORY
int max_card_board = 5;
int max_player_cards = 5;
int players = 4;

struct Player {
    vector<Tile> board_status;
    vector<Animal> player_cards = vector<Animal>(0);
    vector<Animal> complete_cards;
    vector<int> player_tokens = vector<int>(3);
    bool tokens_picked = false;
};

struct Game {
    int turn = 0;
    vector<int> tbag;
    vector<vector<int>> tboard;
    vector<Animal> card_deck; //we take cards from the bottom
    vector<Player> players;

};

string color(int i) {
    if(i==-1) return "\033[39m"; //default terminal color
    return "\33[38;5;" + to_string(i) + "m"; 
}

string bold() {
    return "\33[1m";
}

string nofont() {
    return "\33[0m";
}

string triple0(int n) {
    string s = "000";
    s[2] = n % 10 + '0';
    n/=10;
    s[1] = n % 10 + '0';
    n/=10;
    s[0] = n % 10 + '0';
    return s;
}

string stack_chars(vector<int> stack) {
    string s = "\33[1m";
    for(int x : stack) {
        switch(x) {
            case Token::Building:
                s.append("\33[38;5;160mB");
                break;
            case Token::Fields:
                s.append("\33[38;5;226mF");
                break;
            case Token::Grass:
                s.append("\33[38;5;46mG");
                break;
            case Token::Mountain:
                s.append("\33[38;5;242mM");
                break;
            case Token::Trunk:
                s.append("\33[38;5;94mT");
                break;
            case Token::Water:
                s.append("\33[38;5;33mW");
                break;
            default:
                s.append(" ");
                break;
        }
    }
    s.append("\33[0m");
    return s;
}

// only includes finished states used in cards (also it doesnt account for the building quirk)
vector<int> tile_to_stack(int tile_state) {
    switch(tile_state) {
        case State::River:
            return {Token::Water,-1,-1};
        case State::Field:
            return {Token::Fields,-1,-1};
        case State::Bush:
            return {Token::Grass,-1,-1};
        case State::Building2:
            return {Token::Building,Token::Building,-1};
        case State::Tree2:
            return {Token::Trunk,Token::Grass,-1};
        case State::Tree3:
            return {Token::Trunk,Token::Trunk,Token::Grass};
        case State::Mountain1:
            return {Token::Mountain,-1,-1};
        case State::Mountain2:
            return {Token::Mountain,Token::Mountain,-1};
        case State::Mountain3:
            return {Token::Mountain,Token::Mountain,Token::Mountain};
        default:
            cout << "invalid tile state?\n";
            return {};
    }
} 

int char_to_token(char c) {
    switch(c) {
        case 'B': return Token::Building;
        case 'F': return Token::Fields;
        case 'G': return Token::Grass;
        case 'M': return Token::Mountain;
        case 'T': return Token::Trunk;
        case 'W': return Token::Water;
        default: return -1; //invalid token
    }
}

void clear_terminal() {
    cout << "\033[2J\033[3J\033[H" << flush;
}

void clear_lines(int n) {
    cout << "\033[" << n << "A" << "\r" << "\033[J" << flush;
}

void print_turn_delimination(int turn) {
    cout << "\n__________________________________ Turn: " << turn << " ___________________________________\n\n";

}

void print_title() {
    cout << "\n\n\n";
    for(int i = 0; i < title.size(); i++) {
        for(int j = 0; j < title[i].size(); j++) {
            char c = title[i][j];
            char color = 'a';
            switch(c) {
                case ' ':
                    cout << ' ';
                    break;
                case '\n':
                    cout << '\n' << "\033[0m";
                    break;
                default:
                    if(color != title_color[i][j]) {
                        cout << "\033[0m"; 
                        color = title_color[i][j];
                        if(color == 'p') cout << "\033[1m";
                        cout << "\033[38;5;" << to_string(rgb(color)) << 'm';
                    }
                    cout << title[i][j];
                    break;
            }
        }
    }
    cout << "\n\n\n";
}

void print_main_menu() {
    cout << "\033[1m" << "1) Play\n";
    cout << "2) Simulate\n";
    cout << "3) Settings\n";
    cout << "4) Exit\n\n" << "\033[0m";
}

void print_game_start() {
    //TODO
}

void print_token(int t) {
    cout << "\33[1m";
    switch(t) {
        case Token::Building:
        cout << "\33[38;5;160m" << "Building # " << "\33[0m";
        break;
        
        case Token::Fields:
        cout << "\33[38;5;226m" << "  Field  = " << "\33[0m";
        break;
    
        case Token::Grass:
        cout << "\33[38;5;46m" << "  Grass  w " << "\33[0m";
        break;
        
        case Token::Mountain:
        cout << "\33[38;5;242m" << "Mountain A " << "\33[0m";
        break;
        
        case Token::Trunk:
        cout << "\33[38;5;94m" << "  Trunk  @ " << "\33[0m";
        break;
    
        case Token::Water:
        cout << "\33[38;5;33m" << "  Water  ~ " << "\33[0m";
        break;
    }
}

void print_token_pools(vector<vector<int>> &tboard) {
    //TODO make board print out in columns instead of rows
    cout << "Token pools: \n";
    for(int i = 0; i < pool_count; i++) {
        cout << i+1 << ": | ";
        for(int node : tboard[i]){
            print_token(node);
            cout << " | ";
        }
        cout << "\n";
    }
    cout << '\n'<< "\33[1m" << "Select a token pool. Type 'help' at any time to view available orders.\n" << "\33[0m";
}

void print_player_tokens(vector<int> &player_tokens) {
    cout << "Your tokens: | ";
    if(player_tokens.empty()) { cout << "\33[2m" << "empty\n" << "\33[0m"; return; }
    for(int x : player_tokens) {
        print_token(x);
        cout  << " | ";
    }
    cout << '\n';
}

// prints first 'n' cards from deck, if 'n' is not provided it prints all cards
void print_cards(vector<Animal> &cards, int n = -1) {
    if(n==-1 || n > cards.size()) n = cards.size();
    if(cards.size()==0) { cout << "no cards\n"; return; }
    /*
        CARD FORMAT
      __           __ 
     |  xxxxxxxxxxx  |
    
        Score:        
        xxxxxxxxxxx 

             o        
      [xxx][xxx][xxx] 
        o  \ | /  o   
           [xxx]o     
        o  / | \  o   
      [xxx][xxx][xxx] 
     |__     o     __|

      __           __
     |     GECKO     |
    
        Score:
        2 5 8 12 17   

                       
      [F  ]          
           \         
           [F  ]    
               \  o 
                [BB ] 
     |__           __|

    */

    for(int i = 0; i < n; i++) cout << "  __           __ ";
    cout << '\n';

    for(int i = 0; i < n; i++) {
        Animal card = cards[i];
        string card_name = card.name;
        
        if(card_name.size() > 11) { //larger name than format allows
            card_name.resize(11);
        } else { //recenter name
            int offset = 11-card_name.size();
            offset /= 2;
            card_name.clear();
            for(int j = 0; j < offset; j++) card_name.push_back(' ');
            card_name.append(card.name);
            while(card_name.size() != 11) card_name.append(" ");
        }

        cout << " |  ";
        cout << bold() << card_name << nofont();
        cout << "  |";
    }
    cout << '\n' << '\n';

    for(int i = 0; i < n; i++) {
        cout << "    Score:        ";
    }
    cout << '\n';

    for(int i = 0; i < n; i++) {
        Animal card = cards[i];
        cout << "    " << color(46);
        string score = "";
        bool endcolor = true;
        for(int i = 0; i < card.score.size(); i++) {
            int x = card.score[i];
            if(x==0) continue;
            
            if(endcolor && i>card.uses) score.append(nofont()), endcolor=false;
            string value = to_string(x);
            if(score.size()+value.size()+1 > 16) { // 11+5 due to nofont()
                cout << color(226) << "warning - card '" << card.name << "' has to many score deliminations\n" << nofont();
                break;
            }
            if(score.size() != 0) score.append(" ");
            score.append(value);
        }
        if(score.size()<16) {
            int offset = score.size();
            for(int j = 0; j < 16-offset; j++) score.append(" ");
        }
        cout << score;
        cout << "   ";
    }
    cout << '\n' << '\n';
    
    for(int i = 0; i < n; i++) {
        cout << "         " << (cards[i].anchor == Direction::T ? color(208)+"o"+nofont() : " ") << "        ";
    }
    cout << '\n';

    // TL T TR
    for(int i = 0; i < n; i++) {
        Animal card = cards[i];
        cout << "  " << (card.neighbors[5] == 0 ? "     " : "[" + stack_chars(tile_to_stack(card.neighbors[5])) + "]");
        cout << (card.neighbors[0] == 0 ? "     " : "[" + stack_chars(tile_to_stack(card.neighbors[0])) + "]");
        cout << (card.neighbors[1] == 0 ? "     " : "[" + stack_chars(tile_to_stack(card.neighbors[1])) + "]") << " ";
    }
    cout << '\n';
    for(int i = 0; i < n; i++) {
        Animal card = cards[i];
        cout << "    " << (card.anchor == Direction::TL ? color(208)+"o"+nofont() : " ");
        cout << "  " << (card.neighbors[5] == 0 ? " " : "\\") << (card.neighbors[0] == 0 ? "   " : " | ") << (card.neighbors[1] == 0 ? " " : "/") << "  ";
        cout << (card.anchor == Direction::TR ? color(208)+"o"+nofont() : " ") << "   ";
    }
    cout << '\n';
    for(int i = 0; i < n; i++) {
        Animal card = cards[i];
        cout << "       " << "[" + stack_chars(tile_to_stack(card.middle)) + "]" << (card.anchor == -1 ? color(208)+"o"+nofont() : " ") << "     ";
    }
    cout << '\n';
    for(int i = 0; i < n; i++) {
        Animal card = cards[i];
        cout << "    " << (card.anchor == Direction::BL ? color(208)+"o"+nofont() : " ");
        cout << "  " << (card.neighbors[4] == 0 ? " " : "/") << (card.neighbors[3] == 0 ? "   " : " | ") << (card.neighbors[2] == 0 ? " " : "\\") << "  ";
        cout << (card.anchor == Direction::BR ? color(208)+"o"+nofont() : " ") << "   ";
    }
    cout << '\n';

    // BL B BR
    for(int i = 0; i < n; i++) {
        Animal card = cards[i];
        cout << "  " << (card.neighbors[4] == 0 ? "     " : "[" + stack_chars(tile_to_stack(card.neighbors[4])) + "]");
        cout << (card.neighbors[3] == 0 ? "     " : "[" + stack_chars(tile_to_stack(card.neighbors[3])) + "]");
        cout << (card.neighbors[2] == 0 ? "     " : "[" + stack_chars(tile_to_stack(card.neighbors[2])) + "]") << " ";
    }
    cout << '\n';

    for(int i = 0; i < n; i++) {
        cout << " |__     " << (cards[i].anchor == Direction::B ? color(208)+"o"+nofont() : " ") << "     __|";
    }
    cout << '\n';


    cout << '\n';
}

// the anchor acts as a sort of state view of the board
void print_board(
    vector<Tile> &board_status,
    vector<int> mask, 
    char anchor_shape = 'o', 
    int anchor_color = 208,
    char Aanchor_shape = ' ',
    int Aanchor_color = -1) {

        string aShape (1,anchor_shape), AShape (1,Aanchor_shape); 

    //
    //0: " /xxx\\      "
    //1: "| xxx |     "
    //2: " \\ x /" + " /xxx\\ \\ x /"
    //3: "   |  " + "| xxx |  |  "
    //4: " /xxx\\" + " \\ x / /xxx\\"
    //5: "| xxx |" + "  |  | xxx |"
    //6: " \\ x /      "
    //
    // 012|3452|3452|...col_size-2...|3452|3416

    cout << "\n\n";

    //0
    for(int i = 0; i < cols; i++) cout << " /" << triple0((2*colsize-1)*i) << "\\      "; 
    cout << '\n';

    //1
    for(int i = 0; i < cols; i++) cout << "| " << stack_chars(board_status[(2*colsize-1)*i].stack) << " |     "; 
    cout << '\n';

    //2
    cout << " \\ " << (mask[0] ? color(anchor_color)+aShape : color(Aanchor_color)+AShape)+nofont()  << " /";
    for(int i = 1; i < cols; i++) cout << " /" << triple0(colsize*i+(colsize-1)*(i-1)) << "\\ \\ " << (mask[(2*colsize-1)*i] ? color(anchor_color)+aShape : color(Aanchor_color)+AShape)+nofont() << " /";
    cout << '\n';

    //3452
    for(int i = 1; i < colsize-1; i++) {
        //3
        cout << "   |  ";
        for(int j = 1; j < cols; j++) cout << "| " << stack_chars(board_status[colsize*j+(colsize-1)*(j-1)+(i-1)].stack) << " |  |  ";
        cout << '\n';

        //4
        cout << " /" << triple0(i) << "\\";
        for(int j = 1; j < cols; j++) cout << " \\ " << (mask[colsize*j+(colsize-1)*(j-1)+(i-1)] ? color(anchor_color)+aShape : color(Aanchor_color)+AShape)+nofont() << " / /" << triple0((2*colsize-1)*j+i) << "\\";
        cout << '\n';
        
        //5
        cout << "| " << stack_chars(board_status[i].stack) << " |";
        for(int j = 1; j < cols; j++) cout << "  |  | " << stack_chars(board_status[(2*colsize-1)*j+i].stack) << " |";
        cout << '\n';

        //2
        cout << " \\ " << (mask[i] ? color(anchor_color)+aShape : color(Aanchor_color)+AShape)+nofont()  << " /";
        for(int j = 1; j < cols; j++) cout << " /" << triple0(colsize*j+(colsize-1)*(j-1)+i) << "\\ \\ " << (mask[(2*colsize-1)*j+i] ? color(anchor_color)+aShape : color(Aanchor_color)+AShape)+nofont() << " /";
        cout << '\n';
    }

    //3
    cout << "   |  ";
    for(int i = 1; i < cols; i++) cout << "| " << stack_chars(board_status[colsize*i+(colsize-1)*(i-1)+(colsize-2)].stack) << " |  |  ";
    cout << '\n';

    //4
    cout << " /" << triple0(colsize-1) << "\\";
    for(int i = 1; i < cols; i++) cout << " \\ " << (mask[colsize*i+(colsize-1)*(i-1)+(colsize-2)] ? color(anchor_color)+aShape : color(Aanchor_color)+AShape)+nofont() << " / /" << triple0((2*colsize-1)*i+(colsize-1)) << "\\";
    cout << '\n';
    
    //1
    for(int i = 0; i < cols; i++) cout << "| " << stack_chars(board_status[(2*colsize-1)*i+(colsize-1)].stack) << " |     "; 
    cout << '\n';

    //6
    for(int i = 0; i < cols; i++) cout << " \\ " << (mask[(2*colsize-1)*i+(colsize-1)] ? color(anchor_color)+aShape : color(Aanchor_color)+AShape)+nofont() << " /      ";

    cout << "\n\n";
}

// returns an anchor mask of the current board
vector<int> anchor_status(vector<Tile> &board_status) {
    vector<int> anchors(nodes);
    for(int i = 0; i < nodes; i++) {
        if(board_status[i].animal) {
            anchors[i] = 1;
        } else {
            anchors[i] = 0;
        }
    }
    return anchors;
}

// mainmenu substate
void mainmenu(int in) {
    switch(in) {
        case 1: //soloplay
            state = Menu::GameStart;
            //TODO printout settings for gamestart
            cout << "\33[1m" << "Press enter to start.\n" << "\33[0m";
            break;    
        case 2: //simulator
            state = Menu::SimSettings;
            cout << "parameters not implemented yet...press enter to begin default simulation\n\n";
            break;
        case 3: //settings
            cout << "not implemented yet...\n\n";
            break;
        case 4: //exit
            cout << "\033[38;5;1mWARNING: Nuclear launch detected! ETA: 3s\n";
            this_thread::sleep_for(chrono::milliseconds(800));
            for(int i = 3; i>0; i--) {
                cout << i << "...\n";
                this_thread::sleep_for(chrono::seconds(1));
            }
            exit(0);
            break;
        default:
            cout << "\033[38;5;1m" << "! - no option '" << in << "'\n\n" << "\033[0m";
            break;
    }
}

// gameloop substate(executes orders)
void mainturn(int order, vector<string> &in, Game &game) {
    Player& user = game.players[0];
    if(!user.tokens_picked && order != Order::PickPool && order != Order::Help) {
        cout << "Please select a token pool first.\n";
        return;
    }
        //pick pool <- 'id'
        //place token <- 'place t id'
        //place animal <- 'place name id'
        //view valid tokens <- 'view t'
        //view valid anchors <- 'view name'
        //pick card <- 'take name'
        //end turn <- 'end'
        //help

    switch(order) {

        case Order::PickPool: {
            user.player_tokens = select_token_pool(game.tboard, game.tbag,stoi(in[0])-1);
            user.tokens_picked = true;

            clear_terminal();
            print_turn_delimination(game.turn);
            cout << "AVAILABLE CARDS:\n";
            print_cards(game.card_deck, max_card_board);
            cout << "YOUR CARDS:\n";
            print_cards(user.player_cards);
            cout << "\nCompleted cards: " << user.complete_cards.size() << '\n';
            print_board(user.board_status,anchor_status(user.board_status));
            print_player_tokens(user.player_tokens);
            break; }
        
        case Order::PickCard: {
            if(user.player_cards.size() == max_player_cards) {
                cout << "\033[38;5;1m" << "! - you can only have " << max_player_cards << " cards at a time\n\n" << "\033[0m";
                return;
            }
            string card_name = in[1];
            int draw_size = (max_card_board > game.card_deck.size() ? game.card_deck.size() : max_card_board);
            for(int i = 0; i < user.player_cards.size(); i++) {
                if(card_name == user.player_cards[i].name) {
                    cout << "\033[38;5;1m" << "! - you already have this card\n\n" << "\033[0m";
                    return;
                }
            }

            for(int i = 0; i < draw_size; i++) {
                if(card_name == game.card_deck[i].name) {
                    user.player_cards.push_back(game.card_deck[i]);
                    game.card_deck.erase(game.card_deck.begin()+i);
                    break;
                }
                if(i == draw_size-1) {
                    cout << "\033[38;5;1m" << "! - this card isn't available or no such card exists\n\n" << "\033[0m";
                    return;
                }
            }

            clear_terminal();
            print_turn_delimination(game.turn);
            cout << "AVAILABLE CARDS:\n";
            print_cards(game.card_deck, max_card_board);
            cout << "YOUR CARDS:\n";
            print_cards(user.player_cards);
            cout << "\nCompleted cards: " << user.complete_cards.size() << '\n';
            print_board(user.board_status,anchor_status(user.board_status));
            print_player_tokens(user.player_tokens);
            break;
        }

        case Order::PlaceToken: {
            int t = char_to_token(in[1][0]);
            int n; size_t pos;
            
            if(user.player_tokens.empty())  {
                cout <<"\033[38;5;1m" << "! - you don't have any tokens, end your turn to pick new ones\n\n" << "\033[0m";
                return;
            }
            try {
                n = stoi(in[2], &pos);
                if(pos != in[2].size()) {
                    cout << "\033[38;5;1m" << "! - invalid hex input\n\n" << "\033[0m";
                    return;
                }
            } catch(...) {
                cout << "\033[38;5;1m" << "! - thats not a hex number buddy\n\n" << "\033[0m";
                return;
            }

            if(t < 0 || in[1].size() > 1) { cout << "\033[38;5;1m" << "! - no such token exists\n\n" << "\033[0m"; return; }
            if(n > nodes-1 || n < 0) { cout << "\033[38;5;1m" << "! - invalid hex position \n\n" << "\033[0m"; return; }
            
            vector<int> mask(nodes);
            placement_mask(t, user.board_status, mask);
            if(!mask[n]) { cout << "\033[38;5;1m" << "! - this token can't be placed here\n\n" << "\033[0m"; return; }

            //remove token from player pool (this has to be last otherwise it deletes the token prematurely)
            bool flag = true;
            vector<int> tmp;
            for(int i = 0; i < user.player_tokens.size(); i++) { 
                if(user.player_tokens[i]==t && flag) flag = false;
                else tmp.push_back(user.player_tokens[i]);
            }
            user.player_tokens = tmp;
            if(flag) { cout << "\033[38;5;1m" << "! - you don't have this token\n\n" << "\033[0m"; return; }

            place_token(t,n,user.board_status);
            for(auto &card : game.card_deck) update_anchor_positions(card, user.board_status,n);
            for(auto &card : user.player_cards) update_anchor_positions(card, user.board_status,n);
            clear_terminal();
            print_turn_delimination(game.turn);
            cout << "AVAILABLE CARDS:\n";
            print_cards(game.card_deck, max_card_board);
            cout << "YOUR CARDS:\n";
            print_cards(user.player_cards);
            cout << "\nCompleted cards: " << user.complete_cards.size() << '\n';
            print_board(user.board_status,anchor_status(user.board_status));
            print_player_tokens(user.player_tokens);
            break;
        }

        case Order::PlaceAnchor: {
            string anchor = in[1];
            int n; size_t pos;
            try {
                n = stoi(in[2], &pos);
                if(pos != in[2].size()) {
                    cout << "\033[38;5;1m" << "! - invalid hex input\n\n" << "\033[0m";
                    return;
                }
            } catch(...) {
                cout << "\033[38;5;1m" << "! - thats not a hex number buddy\n\n" << "\033[0m";
                return;
            }

            if(n > nodes-1 || n < 0) { cout << "\033[38;5;1m" << "! - invalid hex position \n\n" << "\033[0m"; return; }

            for(auto &card : user.player_cards) {
                if(card.name == anchor) {
                    if(card.placement[n] == 1) { 
                        user.board_status[n].animal=1;
                        card.uses++;
                        
                        //animal used up check
                        if(card.uses == card.score.size()-1) {
                            user.complete_cards.push_back(card);
                            user.player_cards.erase(user.player_cards.begin()+(&card - &user.player_cards[0]));
                        }

                        anchor = "flag"; 
                    } else { cout << "\033[38;5;1m" << "! - you can't place this animal here\n\n" << "\033[0m"; return;}
                    break;
                }

            }

            if(anchor!="flag") { cout << "\033[38;5;1m" << "! - you don't have this animal card or it doesn't exist\n\n" << "\033[0m"; return; }

            for(auto &card : game.card_deck) update_anchor_positions(card, user.board_status, n);
            for(auto &card : user.player_cards) update_anchor_positions(card, user.board_status, n);
            clear_terminal();
            print_turn_delimination(game.turn);
            cout << "AVAILABLE CARDS:\n";
            print_cards(game.card_deck, max_card_board);
            cout << "YOUR CARDS:\n";
            print_cards(user.player_cards);
            cout << "\nCompleted cards: " << user.complete_cards.size() << '\n';
            print_board(user.board_status,anchor_status(user.board_status));
            print_player_tokens(user.player_tokens);
            break;
        }
        
        case Order::ViewToken: {
            int t = char_to_token(in[1][0]);
            if(t < 0 || in[1].size() > 1) { cout << "\033[38;5;1m" << "! - no such token exists\n\n" << "\033[0m"; return; }

            clear_terminal();
            print_turn_delimination(game.turn);
            cout << "AVAILABLE CARDS:\n";
            print_cards(game.card_deck, max_card_board);
            cout << "YOUR CARDS:\n";
            print_cards(user.player_cards);
            cout << "\nCompleted cards: " << user.complete_cards.size() << '\n';

            vector<int> m(nodes);
            placement_mask(t,user.board_status,m);
            print_board(user.board_status,m,'o',46,'x',1);
            
            print_player_tokens(user.player_tokens);
            break;
        }

        case Order::ViewAnchor: {
            string a = in[1];
            Animal A;

            int draw_size = (max_card_board > game.card_deck.size() ? game.card_deck.size() : max_card_board);
            for(int i = 0; i < draw_size && A.name == ""; i++) {
                if(a == game.card_deck[i].name)  A = game.card_deck[i];
            }
            for(int i = 0; i < user.player_cards.size() && A.name == ""; i++) {
                if(a == user.player_cards[i].name)  A = user.player_cards[i];
            }

            if(A.name == "") { cout << "\033[38;5;1m" << "! - this animal isn't available or doesn't exists\n\n" << "\033[0m"; return;}

            clear_terminal();
            print_turn_delimination(game.turn);
            cout << "AVAILABLE CARDS:\n";
            print_cards(game.card_deck, max_card_board);
            cout << "YOUR CARDS:\n";
            print_cards(user.player_cards);
            cout << "\nCompleted cards: " << user.complete_cards.size() << '\n';

            print_board(user.board_status,A.placement,'o',46,'x',1);
            
            print_player_tokens(user.player_tokens);
            break;
        }

        case Order::ClearView: {
            clear_terminal();
            print_turn_delimination(game.turn);
            cout << "AVAILABLE CARDS:\n";
            print_cards(game.card_deck, max_card_board);
            cout << "YOUR CARDS:\n";
            print_cards(user.player_cards);
            cout << "\nCompleted cards: " << user.complete_cards.size() << '\n';
            print_board(user.board_status,anchor_status(user.board_status));
            print_player_tokens(user.player_tokens);
            cout << "\nReset anchor/token view.\n";
            break; 
        }

        case Order::EndTurn: {
            
            int empty_tile = 0;
            for(Tile t : user.board_status) if(t.state_id() == State::Empty) empty_tile++;
            if(game.tbag.empty() || empty_tile < 3) { 
                cout << bold() << "You have completed the game! Press Enter to view your score.\n";
                state = Menu::EndGame; 
                return;
            }
            
            game.turn++;
            clear_terminal();
            print_turn_delimination(game.turn);
            cout << "AVAILABLE CARDS:\n";
            print_cards(game.card_deck, max_card_board);
            cout << "YOUR CARDS:\n";
            print_cards(user.player_cards);
            cout << "\nCompleted cards: " << user.complete_cards.size() << '\n';
            print_board(user.board_status,anchor_status(user.board_status));
            print_token_pools(game.tboard);
            user.tokens_picked = false;
            break;
        }
        
        case Order::Help: {
            cout << bold() <<"Available orders:\n\n" << nofont();
            cout << bold() << "> 'n' " << nofont() << "- select token pool number 'n'\n";
            cout << bold() << "> place 't' 'n' " << nofont() << "- place token 't' (capital letter of its name) at tile number 'n' (numbered on the board)\n";
            cout << bold() << "> place 'name' 'n' " << nofont() << "- place animal ('name' is case sensitive!) at tile number 'n'\n";
            cout << bold() << "> view 't' " << nofont() << "- displays valid locations for placing token 't'\n";
            cout << bold() << "> view 'name' " << nofont() << "- displays valid locations for placing animals\n";
            cout << bold() << "> take 'name' " << nofont() << "- takes the animal card from the deck board to your hand\n";
            cout << bold() << "> end " << nofont() << "- ends your turn after you place all your tokens\n";
            cout << "\n Press Enter to return back to the game.\n";
            
            string tmp;
            getline(cin,tmp);
            clear_lines(13);
            break;
        }

        default:
            cout << "ouch\n";
            break;
    }
}

// determines which order is given and failsafes the metadata of the order(the content of the order is dealt with in execution)
int give_order(vector<string> &in, Player &user) {
    string o = in[0];
    if(o == "help") { return Order::Help; }

    if(in.size() == 1 && !user.tokens_picked) { //select pool
        int n; size_t pos;
        try {
            n = stoi(in[0], &pos);
            if(pos != in[0].size()) {
                cout << "\033[38;5;1m" << "! - invalid input\n\n" << "\033[0m";
                return -1;
            }
            if(n < 1 || n > pool_count+1) {
                cout << "\033[38;5;1m" << "! - invalid token pool, a value in range 1-" << pool_count << " is required\n\n" << "\033[0m";
                return -1;
            }
        } catch(...) {
                cout << "\033[38;5;1m" << "! - thats not a number buddy\n\n" << "\033[0m";
                return -1;
            }
        return Order::PickPool;

    } else if(o == "take") {
        if(in.size()<2) { cout << "\033[38;5;1m" << "! - no animal name was given\n\n" << "\033[0m"; return -1; }
        return Order::PickCard;

    } else if(o == "place") {
        if(in.size()<2) { cout << "\033[38;5;1m" << "! - no token/animal was given\n\n" << "\033[0m"; return -1; }
        if(in.size()<3) { cout << "\033[38;5;1m" << "! - no hex position was given\n\n" << "\033[0m"; return -1; }

        if(in[1].size() > 1) {
            return Order::PlaceAnchor;
        } else {
            return Order::PlaceToken;
        }

    } else if(o == "view") {
        if(in.size()==1) {
            return Order::ClearView;
        } else if(in[1].size() == 1) {
            return Order::ViewToken;
        } else {
            return Order::ViewAnchor;
        }

    } else if(o == "end") {
        if(!user.player_tokens.empty()) { cout << "\033[38;5;1m" << "! - you must place all tokens before ending your turn\n\n" << "\033[0m"; return -1; }
        return Order::EndTurn;
        
    } else {
        cout << "\033[38;5;1m" << "! - no order named '" << o << "'\n\n" << "\033[0m";
        return -1;
    }
    return -1;
}

//initializes the game struct
void init_game(Game &game) {
    game.card_deck = {Gecko, Shrew, Flamingo, Meerkat, Raccoon, Warthog, Ladybug, Frog, Koala, Duck, Bat, Fish, Squirrel, Otter, FennecFox,
        Rabbit, Alligator, MantaRay, Wolf, Parrot, Penguin, Peacock, ArcticFox, Kingfisher, Falcon, Alpaca, Panther, Raven,
        BrownBear, ArcticMonkey, Hedgehog, Bumblebee};
    shuffle(game.card_deck.begin(), game.card_deck.end(), rng);
    
    game.tbag = init_token_bag(token_counts);
    game.tboard = init_token_board(pool_count, game.tbag);
    game.turn = 1;

    for(int i = 0; i < players; i++) { Player player; game.players.push_back(player); }
    
    const Tile t;
    for(Player &player : game.players) {
        for(int i = 0; i < nodes; i++) player.board_status.push_back(t);
    }
}

void print_player_score(Player &user, int playerID) {
    int t=0, m=0, b=0, f=0, r=0, a=0;
    vector<int> tmp;
    tmp = tree_score(user.board_status);
    for(int x : tmp) t += x;
    tmp = mountain_score(user.board_status);
    for(int x : tmp) m += x;
    b = building_score(user.board_status);
    f = fields_score(user.board_status);
    r = river_score(user.board_status);

    for(auto card : user.complete_cards) a += card.score.back();
    for(auto card : user.player_cards) a += card.score[card.uses];

    cout << bold() << "PLAYER " << playerID << " SCORE: \n" << nofont();
    cout << color(46) << "  Trees and bushes: " << nofont() << bold() << t << nofont() << '\n';
    cout << color(242) << "Mountain and hills: " << nofont() << bold() << m << nofont() << '\n';
    cout << color(160) << "         Buildings: " << nofont() << bold() << b << nofont() << '\n';
    cout << color(226) << "            Fields: " << nofont() << bold() << f << nofont() << '\n';
    cout << color(33) << "  Rivers and lakes: " << nofont() << bold() << r << nofont() << "\n\n";

    cout << color(208) << "   Animal habitats: " << nofont() << bold() << a << nofont();
    cout << " ( ";
    for(auto card : user.complete_cards) cout << card.name << '-' << card.score[card.uses] << " ";
    cout << ") " << '\n';
    cout << "Your unfinished habitats: ( ";
    for(auto card : user.player_cards) cout << card.name << '-' << card.score[card.uses] << " ";
    cout << ") " << '\n';
    cout << "TOTAL SCORE: " << bold() << t+m+b+f+r+a << nofont() << "\n\n\n";
}

string recenter(int cap, string s) {
    string tmp = s;
    int offset = cap-s.size();
    offset /= 2;
    s.clear();
    for(int j = 0; j < offset; j++) s.push_back(' ');
    s.append(tmp);
    while(s.size() != cap) s.append(" ");
    return s;
}

string format_dec(string v) {
    v.resize(v.find('.') + 3);
    return v;
}

void print_progress(int current, int total, int bar_width = 50) {
    float frac = (float)current / total;
    int filled = (int)(frac * bar_width);

    std::cout << "\r[";
    for (int i = 0; i < bar_width; ++i) std::cout << (i < filled ? '#' : ' ');
    std::cout << "] " << int(frac * 100) << "%" << std::flush;
}

// main simulation execution
void simulate(int games) {

    struct Stats {
        // 0 = AVG | 1 = MAX | 2 = SUM
        // the average is obsolete, i forgot it needs a decimal :/
        vector<int> singleGame = vector<int> (3,0);
        vector<int> doneC = vector<int> (3,0); 
        vector<int> scoreC = vector<int> (3,0);
        vector<int> river = vector<int> (3,0);
        vector<int> building = vector<int> (3,0);
        vector<int> field = vector<int> (3,0);
        vector<int> mt1 = vector<int> (3,0);
        vector<int> mt2 = vector<int> (3,0);
        vector<int> mt3 = vector<int> (3,0);
        vector<int> t1 = vector<int> (3,0);
        vector<int> t2 = vector<int> (3,0); 
        vector<int> t3 = vector<int> (3,0);
    };

    vector<Stats> stats(players);
    int turns = 0, turn_max = 0;

    for(int k = 0; k < games; k++) {

        //1 game
        // 5 possible actions per turn: pickPool, pickCard, placeToken, placeAnchor, endTurn
        // at minimum 4 mandatory decisions, picking the pool and placing 3 tokens (location for each)
        // each decision is masked
        // non-mandatory decisions have a 'skip' option
        Game game;
        init_game(game);
        bool end_game = false;

        vector<int> turn_order; // player turn order
        for(int i = 0; i < players; i++)  turn_order.push_back(i);
        shuffle(turn_order.begin(), turn_order.end(), rng);
        
        while(!end_game) {
            for(int playerID : turn_order) {
                Player &user = game.players[playerID];
        
                //AGENT IMPLEMENTATION GOES HERE
        
                //RANDOM AGENT:
                // picks pool at random, always fills up card slots selecting at random, places anchors whenever able
                // places tokens in random order and random tiles
        
                //pick pool
                user.player_tokens = select_token_pool(game.tboard, game.tbag, rand_int(0,4));
                user.tokens_picked = true;
        
                //one of three placeToken,placeAnchor,pickCard (masked decisions)
                vector<int> t_order; // random token placement order
                for(int i = 0; i < 3; i++)  t_order.push_back(i);
                shuffle(t_order.begin(), t_order.end(), rng);

                for(int t : t_order) {
                    vector<int> mask(nodes);
                    placement_mask(user.player_tokens[t], user.board_status, mask);
                    vector<int> valid(0);
                    for(int i = 0; i < nodes; i++) { if(mask[i])valid.push_back(i); }
                    if(valid.empty()) {
                        cout << "ERROR: valid placement for token " << t << " was empty, EXITING SIMULATION!\n";

                        exit(1);
                    }
                    int tile = valid[rand_int(0,valid.size()-1)]; //random token placement tile
                    place_token(user.player_tokens[t], tile, user.board_status);
                    for(auto &card : user.player_cards) update_anchor_positions(card, user.board_status,tile);
        
                    while(user.player_cards.size() != max_player_cards) {
                        int draw_size = (max_card_board > game.card_deck.size() ? game.card_deck.size() : max_card_board);
                        int i = rand_int(0,draw_size-1); //random card selection fill
                        for(int n = 0; n < nodes; n++) update_anchor_positions(game.card_deck[i], user.board_status, n);
                        user.player_cards.push_back(game.card_deck[i]);
                        game.card_deck.erase(game.card_deck.begin()+i);
                    }
        
                    //auto place possible anchors
                    vector<int> completed;
                    for(int j = 0; j < user.player_cards.size(); j++) {
                        auto &card = user.player_cards[j];
                        for(int i = 0; i < nodes; i++) {
                            if(card.placement[i]) {
                                user.board_status[i].animal=1;
                                card.uses++;
        
                                //animal used up check
                                if(card.uses == card.score.size()-1) {
                                    user.complete_cards.push_back(card);
                                    completed.push_back(j);
                                }
        
                                for(auto &c : user.player_cards) update_anchor_positions(c, user.board_status, i);
                            }        
                        }
                    }
                    while(!completed.empty()) {
                        user.player_cards.erase(user.player_cards.begin()+completed.back());
                        completed.pop_back();
                    }
        
                }
                user.player_tokens.clear();
                user.tokens_picked = false;
        
                //end turn 
                if(&game.players[playerID] == &game.players.back()) game.turn++;

                int empty_tile = 0;
                for(Tile t : user.board_status) if(t.state_id() == State::Empty) empty_tile++;
                if(game.tbag.empty() || empty_tile < 3) end_game = true;
            }

        }

        /*
        //GAME PRINTOUT
        clear_terminal();
        cout << bold() << "\n__________________________________ GAME COMPLETE ___________________________________\n\n" << nofont();
        cout << "Game length: " << game.turn << " turns\n\n";
        for(int i = 0; i < game.players.size(); i++) print_player_score(game.players[i],i);
        */

        // SAVE THE GAME STATS
        turns += game.turn;
        turn_max = game.turn > turn_max ? game.turn : turn_max;
        for(int i = 0; i < players; i++) {
            Player user = game.players[i];
            Stats &stat = stats[i];
    
            int t=0, m=0, b=0, f=0, r=0, a=0, total=0;
            vector<int> tmp;
            tmp = tree_score(user.board_status);
            //MAX
            stat.t1[1] = stat.t1[1] > tmp[0] ? stat.t1[1] : tmp[0];
            stat.t2[1] = stat.t2[1] > tmp[1] ? stat.t2[1] : tmp[1];
            stat.t3[1] = stat.t3[1] > tmp[2] ? stat.t3[1] : tmp[2];
    
            stat.t1[2] += tmp[0], stat.t2[2] += tmp[1], stat.t3[2] += tmp[2]; //SUM
            for(int x : tmp) total += x;
            
            tmp = mountain_score(user.board_status);
            //MAX
            stat.mt1[1] = stat.mt1[1] > tmp[0] ? stat.mt1[1] : tmp[0];
            stat.mt2[1] = stat.mt2[1] > tmp[1] ? stat.mt2[1] : tmp[1];
            stat.mt3[1] = stat.mt3[1] > tmp[2] ? stat.mt3[1] : tmp[2];
    
            stat.mt1[2] += tmp[0], stat.mt2[2] += tmp[1], stat.mt3[2] += tmp[2]; //SUM
            for(int x : tmp) total += x;
    
            b = building_score(user.board_status);
            stat.building[1] = stat.building[1] > b ? stat.building[1] : b; //MAX
            stat.building[2] += b; //SUM
            
            f = fields_score(user.board_status);
            stat.field[1] = stat.field[1] > f ? stat.field[1] : f; //MAX
            stat.field[2] += f; //SUM
            total += f;
    
            r = river_score(user.board_status);
            stat.river[1] = stat.river[1] > r ? stat.river[1] : r; //MAX
            stat.river[2] += r; //SUM
            total += r;
    
            for(auto card : user.complete_cards) a += card.score.back();
            for(auto card : user.player_cards) a += card.score[card.uses];
            stat.doneC[1] = stat.doneC[1] > user.complete_cards.size() ? stat.doneC[1] : user.complete_cards.size(); //MAX
            stat.doneC[2] += user.complete_cards.size(); //SUM
    
            stat.scoreC[1] = stat.scoreC[1] > a ? stat.scoreC[1] : a; //MAX
            stat.scoreC[2] += a; //SUM
            total += a;

            stat.singleGame[1] = stat.singleGame[1] > total ? stat.singleGame[1] : total; //MAX
            stat.singleGame[2] += total; //SUM
    
        }


        if (k % (games/100) == 0 || k == games - 1) {
            print_progress(k + 1, games);
        }
    }

    //calc the average
    for(int i = 0; i < players; i++) {
        Stats &stat = stats[i];
        stat.singleGame[0] = stat.singleGame[2] / games;
        stat.doneC[0] = stat.doneC[2] / games;
        stat.scoreC[0] = stat.scoreC[2] / games;
        stat.river[0] = stat.river[2] / games;
        stat.building[0] = stat.building[2] / games;
        stat.field[0] = stat.field[2] / games;
        stat.mt1[0] = stat.mt1[2] / games;
        stat.mt2[0] = stat.mt2[2] / games;
        stat.mt3[0] = stat.mt3[2] / games;
        stat.t1[0] = stat.t1[2] / games;
        stat.t2[0] = stat.t2[2] / games;
        stat.t3[0] = stat.t3[2] / games;
    }

    //SESSION PRINTOUT
    // per card rundown?
    // turns?

    clear_terminal();
    cout << bold() << "\n__________________________________ SESSION of " << games << " GAMES COMPLETE ___________________________________\n\n" << nofont();
    cout << "AVG. TURNS: " << format_dec(to_string((float)turns/games)) << '\n';
    cout << "MAX. TURNS: " << turn_max << '\n';
    cout << "SUM. TURNS: " << turns << '\n';
    for(int i = 0; i < players; i++) {
        Stats &s = stats[i];
        cout << "\nPLAYER " << i << ":\n\n";
        cout << bold() << "      | GAME     | DONE C   | SCORE C  | RIVER    | BUILDING | FIELD    | MT.1     | MT.2     | MT.3     | T1       | T2       | T3       |\n" << nofont(); 

        cout << bold() << "AVG.  |" << nofont() << ' ' << recenter(8,format_dec(to_string((float)s.singleGame[2]/games))) << " |"
            << ' ' << recenter(8,format_dec(to_string((float)s.doneC[2]/games))) << " |"
            << ' ' << recenter(8,format_dec(to_string((float)s.scoreC[2]/games))) << " |"
            << ' ' << recenter(8,format_dec(to_string((float)s.river[2]/games))) << " |"
            << ' ' << recenter(8,format_dec(to_string((float)s.building[2]/games))) << " |"
            << ' ' << recenter(8,format_dec(to_string((float)s.field[2]/games))) << " |"
            << ' ' << recenter(8,format_dec(to_string((float)s.mt1[2]/games))) << " |"
            << ' ' << recenter(8,format_dec(to_string((float)s.mt2[2]/games))) << " |"
            << ' ' << recenter(8,format_dec(to_string((float)s.mt3[2]/games))) << " |"
            << ' ' << recenter(8,format_dec(to_string((float)s.t1[2]/games))) << " |"
            << ' ' << recenter(8,format_dec(to_string((float)s.t2[2]/games))) << " |"
            << ' ' << recenter(8,format_dec(to_string((float)s.t3[2]/games))) << " |"
            << '\n';
        cout << bold() << "MAX.  |" << nofont() << ' ' << recenter(8,to_string(s.singleGame[1])) << " |"
            << ' ' << recenter(8,to_string(s.doneC[1])) << " |"
            << ' ' << recenter(8,to_string(s.scoreC[1])) << " |"
            << ' ' << recenter(8,to_string(s.river[1])) << " |"
            << ' ' << recenter(8,to_string(s.building[1])) << " |"
            << ' ' << recenter(8,to_string(s.field[1])) << " |"
            << ' ' << recenter(8,to_string(s.mt1[1])) << " |"
            << ' ' << recenter(8,to_string(s.mt2[1])) << " |"
            << ' ' << recenter(8,to_string(s.mt3[1])) << " |"
            << ' ' << recenter(8,to_string(s.t1[1])) << " |"
            << ' ' << recenter(8,to_string(s.t2[1])) << " |"
            << ' ' << recenter(8,to_string(s.t3[1])) << " |"
            << '\n';
        cout << bold() << "SUM.  |" << nofont() << ' ' << recenter(8,to_string(s.singleGame[2])) << " |"
            << ' ' << recenter(8,to_string(s.doneC[2])) << " |"
            << ' ' << recenter(8,to_string(s.scoreC[2])) << " |"
            << ' ' << recenter(8,to_string(s.river[2])) << " |"
            << ' ' << recenter(8,to_string(s.building[2])) << " |"
            << ' ' << recenter(8,to_string(s.field[2])) << " |"
            << ' ' << recenter(8,to_string(s.mt1[2])) << " |"
            << ' ' << recenter(8,to_string(s.mt2[2])) << " |"
            << ' ' << recenter(8,to_string(s.mt3[2])) << " |"
            << ' ' << recenter(8,to_string(s.t1[2])) << " |"
            << ' ' << recenter(8,to_string(s.t2[2])) << " |"
            << ' ' << recenter(8,to_string(s.t3[2])) << " |"
            << '\n';
    }

    cout << "\n\n";

    //PLAYER 0:

    //      | GAME     | DONE C   | SCORE C  | RIVER    | BUILDING | FIELD    | MT.1     | MT.2     | MT.3     | T1       | T2       | T3       |
    //AVG.  | 99999999 | 99999999 | 99999999 | 99999999 | 99999999 | 99999999 | 99999999 | 99999999 | 99999999 | 99999999 | 99999999 | 99999999 |
    //MAX.  |          |          |          |          |          |          |          |          |          |          |          |          |
    //SUM.  |
    
    //    | P0       | P1       | P2       | P3       |
    //1st | 99999999 | 99999999 | 99999999 | 99999999 |
    //2nd | X  |
    //3rd | X  |
    //4th | X  |

}

Game game; // we assume the user is player 0
//put this in main and probbly pass it to statemachine?

// the main interface state function
void state_machine(vector<string> &in) {

    switch(state) {

        case Menu::MainMenu: {
            int n; size_t pos;
            try {
                n = stoi(in[0], &pos);
                if(n < 1 || n > 4 || in.size()>1 || pos != in[0].size()) {
                    cout << "\033[38;5;1m" << "! - invalid input\n\n" << "\033[0m";
                    return;
                }
            } catch(...) {
                cout << "\033[38;5;1m" << "! - thats not a number buddy\n\n" << "\033[0m";
                return;
            }
            mainmenu(n);
            break; }
        
        case Menu::GameStart: {
            //settings for singleplayer
            //initializes the game
            // GameStart -> TokenSelection 
            
            init_game(game);
            
            clear_terminal();
            print_turn_delimination(game.turn);
            cout << "AVAILABLE CARDS:\n";
            print_cards(game.card_deck, max_card_board);
            print_token_pools(game.tboard);
            state = MainTurn;
            break; }
        
        case Menu::MainTurn: {
            // select pool
            // user can take cards, place tokens, place anchors as well as view valid placements
            // end turn
            // MainTurn -> MainTurn OR MainTurn -> EndGame
            int order = give_order(in, game.players[0]);
            if(order<0) return;
            mainturn(order,in, game);
            break; }

        case Menu::EndGame: {
            //post game screen
            //scoring, clearing memory etc.
            // EndGame -> MainMenu
            Player &user = game.players[0];
            clear_terminal();
            cout << bold() << "\n__________________________________ GAME COMPLETE ___________________________________\n\n" << nofont();
            cout << "Game length: " << game.turn << " turns\n\n";

            int t=0, m=0, b=0, f=0, r=0, a=0;
            vector<int> tmp;
            tmp = tree_score(user.board_status);
            for(int x : tmp) t += x;
            tmp = mountain_score(user.board_status);
            for(int x : tmp) m += x;
            b = building_score(user.board_status);
            f = fields_score(user.board_status);
            r = river_score(user.board_status);

            for(auto card : user.complete_cards) a += card.score.back();
            for(auto card : user.player_cards) a += card.score[card.uses];

            cout << bold() << "YOUR SCORE: \n" << nofont();
            cout << color(46) << "  Trees and bushes: " << nofont() << bold() << t << nofont() << '\n';
            cout << color(242) << "Mountain and hills: " << nofont() << bold() << m << nofont() << '\n';
            cout << color(160) << "         Buildings: " << nofont() << bold() << b << nofont() << '\n';
            cout << color(226) << "            Fields: " << nofont() << bold() << f << nofont() << '\n';
            cout << color(33) << "  Rivers and lakes: " << nofont() << bold() << r << nofont() << "\n\n";

            cout << color(208) << "   Animal habitats: " << nofont() << bold() << a << nofont();
            cout << " ( ";
            for(auto card : user.complete_cards) cout << card.name << '-' << card.score[card.uses] << " ";
            cout << ") " << '\n';
            cout << "Your unfinished habitats: ( ";
            for(auto card : user.player_cards) cout << card.name << '-' << card.score[card.uses] << " ";
            cout << ") " << '\n';
            cout << "-------------------- TOTAL SCORE: " << bold() << t+m+b+f+r+a << nofont() << " --------------------\n\n\n";

            cout << "Your habitat board: \n";
            print_board(user.board_status,anchor_status(user.board_status));

            cout  << "Press Enter to return to the Main Menu.\n";
            state = Menu::MainMenu;
            string input;
            getline(cin,input);
            game = Game{};
            clear_terminal();
            print_title();
            print_main_menu();
            break;
        }

        case Menu::SimSettings: {
            state = Menu::Simulation;
            cout << "pending settings...\n";
            break;
        }
        
        case Menu::Simulation: {
            simulate(stoi(in[0]));
            cout  << "Press Enter to return to the Main Menu.\n";
            state = Menu::MainMenu;
            string input;
            getline(cin,input);
            clear_terminal();
            print_title();
            print_main_menu();
            break;
        }

        //TODO
        case Menu::Settings:  {

            break;
        }

        default:
            cout << "wtf\n";
            break;
    }
} 

int main() {
    //TODO make previous cmd autocomplete
    string input;
    vector<string> tokens;
    
    print_title();
    print_main_menu();
    
    while(1) {
        getline(cin,input);
        istringstream stream(input);
        while(stream >> input) tokens.push_back(input);
        state_machine(tokens);
        tokens.clear();
        input.clear();
    }
    return 0;
}