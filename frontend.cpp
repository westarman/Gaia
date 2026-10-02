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
    MainMenu, GameStart, Settings, MainTurn, EndGame
};

enum Order : int {
    PickPool, PickCard, PlaceToken, PlaceAnchor, ViewToken, ViewAnchor, ClearView, EndTurn
};

int state = Menu::MainMenu;

//GAME MEMORY
//one only
vector<int> tbag(0);
vector<vector<int>> tboard(0);
vector<Animal> card_deck(0); //we take cards from the top
int max_card_board = 5;
int max_player_cards = 3;
int turn = 0;

//per player
vector<Tile> board_status(0);
vector<Animal> player_cards(0);
vector<Animal> complete_cards(0);
vector<int> player_tokens(0);
bool tokens_picked = false;

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

void print_turn_delimination() {
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

void print_token_pools() {
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
    cout << '\n'<< "\33[1m" << "Select a token pool.\n" << "\33[0m";
}

void print_player_tokens() {
    cout << "Your tokens: | ";
    if(player_tokens.empty()) { cout << "\33[2m" << "empty\n" << "\33[0m"; return; }
    for(int x : player_tokens) {
        print_token(x);
        cout  << " | ";
    }
    cout << '\n';
}

// prints first 'n' cards from deck, if 'n' is not provided it prints all cards
void print_cards(vector<Animal> cards, int n = -1) {
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
vector<int> anchor_status() {
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
        case 1:
            state = Menu::GameStart;
            //TODO printout settings for gamestart
            cout << "\33[1m" << "Press enter to start.\n" << "\33[0m";
            break;    
        case 2:
            cout << "not implemented yet...\n\n";
            break;
        case 3:
            cout << "not implemented yet...\n\n";
            break;
        case 4:
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
void mainturn(int order, vector<string> &in) {
    if(!tokens_picked && order != Order::PickPool) {
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
            player_tokens = select_token_pool(tboard,tbag,stoi(in[0])-1);
            tokens_picked = true;

            clear_terminal();
            print_turn_delimination();
            cout << "AVAILABLE CARDS:\n";
            print_cards(card_deck, max_card_board);
            cout << "YOUR CARDS:\n";
            print_cards(player_cards);
            cout << "\nCompleted cards: " << complete_cards.size() << '\n';
            print_board(anchor_status());
            print_player_tokens();
            break; }
        
        case Order::PickCard: {
            if(player_cards.size() == max_player_cards) {
                cout << "\033[38;5;1m" << "! - you can only have " << max_player_cards << " cards at a time\n\n" << "\033[0m";
                return;
            }
            string card_name = in[1];
            int draw_size = (max_card_board > card_deck.size() ? card_deck.size() : max_card_board);
            for(int i = 0; i<player_cards.size(); i++) {
                if(card_name == player_cards[i].name) {
                    cout << "\033[38;5;1m" << "! - you already have this card\n\n" << "\033[0m";
                    return;
                }
            }

            for(int i = 0; i < draw_size; i++) {
                if(card_name == card_deck[i].name) {
                    player_cards.push_back(card_deck[i]);
                    card_deck.erase(card_deck.begin()+i);
                    break;
                }
                if(i == draw_size-1) {
                    cout << "\033[38;5;1m" << "! - this card isn't available or no such card exists\n\n" << "\033[0m";
                    return;
                }
            }

            clear_terminal();
            print_turn_delimination();
            cout << "AVAILABLE CARDS:\n";
            print_cards(card_deck, max_card_board);
            cout << "YOUR CARDS:\n";
            print_cards(player_cards);
            cout << "\nCompleted cards: " << complete_cards.size() << '\n';
            print_board(anchor_status());
            print_player_tokens();
            break;
        }

        case Order::PlaceToken: {
            int t = char_to_token(in[1][0]);
            int n; size_t pos;
            
            if(player_tokens.empty())  {
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
            placement_mask(t, board_status, mask);
            if(!mask[n]) { cout << "\033[38;5;1m" << "! - this token can't be placed here\n\n" << "\033[0m"; return; }

            //remove token from player pool (this has to be last otherwise it deletes the token prematurely)
            bool flag = true;
            vector<int> tmp;
            for(int i = 0; i < player_tokens.size(); i++) { 
                if(player_tokens[i]==t && flag) flag = false;
                else tmp.push_back(player_tokens[i]);
            }
            player_tokens = tmp;
            if(flag) { cout << "\033[38;5;1m" << "! - you don't have this token\n\n" << "\033[0m"; return; }

            place_token(t,n,board_status);
            for(auto &card : card_deck) update_anchor_positions(card, board_status,n);
            for(auto &card : player_cards) update_anchor_positions(card, board_status,n);
            clear_terminal();
            print_turn_delimination();
            cout << "AVAILABLE CARDS:\n";
            print_cards(card_deck, max_card_board);
            cout << "YOUR CARDS:\n";
            print_cards(player_cards);
            cout << "\nCompleted cards: " << complete_cards.size() << '\n';
            print_board(anchor_status());
            print_player_tokens();
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

            for(auto &card : player_cards) {
                if(card.name == anchor) {
                    if(card.placement[n] == 1) { 
                        board_status[n].animal=1;
                        card.uses++;
                        
                        //animal used up check
                        if(card.uses == card.score.size()-1) {
                            complete_cards.push_back(card);
                            player_cards.erase(player_cards.begin()+(&card - &player_cards[0]));
                        }

                        anchor = "flag"; 
                    } else { cout << "\033[38;5;1m" << "! - you can't place this animal here\n\n" << "\033[0m"; return;}
                    break;
                }

            }

            if(anchor!="flag") { cout << "\033[38;5;1m" << "! - you don't have this animal card or it doesn't exist\n\n" << "\033[0m"; return; }

            for(auto &card : card_deck) update_anchor_positions(card, board_status,n);
            for(auto &card : player_cards) update_anchor_positions(card, board_status,n);
            clear_terminal();
            print_turn_delimination();
            cout << "AVAILABLE CARDS:\n";
            print_cards(card_deck, max_card_board);
            cout << "YOUR CARDS:\n";
            print_cards(player_cards);
            cout << "\nCompleted cards: " << complete_cards.size() << '\n';
            print_board(anchor_status());
            print_player_tokens();
            break;
        }
        
        case Order::ViewToken: {
            int t = char_to_token(in[1][0]);
            if(t < 0 || in[1].size() > 1) { cout << "\033[38;5;1m" << "! - no such token exists\n\n" << "\033[0m"; return; }

            clear_terminal();
            print_turn_delimination();
            cout << "AVAILABLE CARDS:\n";
            print_cards(card_deck, max_card_board);
            cout << "YOUR CARDS:\n";
            print_cards(player_cards);
            cout << "\nCompleted cards: " << complete_cards.size() << '\n';

            vector<int> m(nodes);
            placement_mask(t,board_status,m);
            print_board(m,'o',46,'x',1);
            
            print_player_tokens();
            break;
        }

        case Order::ViewAnchor: {
            string a = in[1];
            Animal A;

            int draw_size = (max_card_board > card_deck.size() ? card_deck.size() : max_card_board);
            for(int i = 0; i < draw_size && A.name == ""; i++) {
                if(a == card_deck[i].name)  A = card_deck[i];
            }
            for(int i = 0; i < player_cards.size() && A.name == ""; i++) {
                if(a == player_cards[i].name)  A = player_cards[i];
            }

            if(A.name == "") { cout << "\033[38;5;1m" << "! - this animal isn't available or doesn't exists\n\n" << "\033[0m"; return;}

            clear_terminal();
            print_turn_delimination();
            cout << "AVAILABLE CARDS:\n";
            print_cards(card_deck, max_card_board);
            cout << "YOUR CARDS:\n";
            print_cards(player_cards);
            cout << "\nCompleted cards: " << complete_cards.size() << '\n';

            print_board(A.placement,'o',46,'x',1);
            
            print_player_tokens();
            break;
        }

        case Order::ClearView: {
            clear_terminal();
            print_turn_delimination();
            cout << "AVAILABLE CARDS:\n";
            print_cards(card_deck, max_card_board);
            cout << "YOUR CARDS:\n";
            print_cards(player_cards);
            cout << "\nCompleted cards: " << complete_cards.size() << '\n';
            print_board(anchor_status());
            print_player_tokens();
            cout << "\nReset anchor/token view.\n";
            break; 
        }

        case Order::EndTurn: {
            
            int empty_tile = 0;
            for(Tile t : board_status) if(t.state_id() == State::Empty) empty_tile++;
            if(tbag.empty() || empty_tile < 3) { 
                cout << bold() << "You have completed the game! Press Enter to view your score.\n";
                state = Menu::EndGame; 
                return;
            }
            
            turn++;
            clear_terminal();
            print_turn_delimination();
            cout << "AVAILABLE CARDS:\n";
            print_cards(card_deck, max_card_board);
            cout << "YOUR CARDS:\n";
            print_cards(player_cards);
            cout << "\nCompleted cards: " << complete_cards.size() << '\n';
            print_board(anchor_status());
            print_token_pools();
            tokens_picked = false;
            break;
        }
        
        default:
            cout << "ouch\n";
            break;
    }
}

// determines which order is given and failsafes the metadata of the order(the content of the order is dealt with in execution)
int give_order(vector<string> &in) {
    string o = in[0];
    if(in.size() == 1 && !tokens_picked) { //select pool
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
        if(!player_tokens.empty()) { cout << "\033[38;5;1m" << "! - you must place all tokens before ending your turn\n\n" << "\033[0m"; return -1; }
        return Order::EndTurn;
    } else {
        cout << "\033[38;5;1m" << "! - no order named '" << o << "'\n\n" << "\033[0m";
        return -1;
    }
    return -1;
}

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
            
            card_deck = {Gecko, Shrew, Flamingo, Meerkat, Raccoon, Warthog};
            shuffle(card_deck.begin(), card_deck.end(), rng);
            tbag = init_token_bag(token_counts);
            tboard = init_token_board(pool_count, tbag);
            turn = 1;
            const Tile t;
            for(int i = 0; i < nodes; i++) board_status.push_back(t);
            
            clear_terminal();
            print_turn_delimination();
            cout << "AVAILABLE CARDS:\n";
            print_cards(card_deck, max_card_board);
            print_token_pools();
            state = MainTurn;
            break; }
        
        case Menu::Settings:
        
            break;
        
        case Menu::MainTurn: {
            // select pool
            // user can take cards, place tokens, place anchors as well as view valid placements
            // end turn
            // MainTurn -> MainTurn OR MainTurn -> EndGame

            int order = give_order(in);
            if(order<0) return;
            mainturn(order,in);
            break; }

        case Menu::EndGame: {
            //post game screen
            //scoring, clearing memory etc.
            // EndGame -> MainMenu
            clear_terminal();
            cout << bold() << "\n__________________________________ GAME COMPLETE ___________________________________\n\n" << nofont();
            cout << "Game length: " << turn << " turns\n\n";

            int t=0, m=0, b=0, f=0, r=0, a=0;
            vector<int> tmp;
            tmp = tree_score(board_status);
            for(int x : tmp) t += x;
            tmp = mountain_score(board_status);
            for(int x : tmp) m += x;
            b = building_score(board_status);
            f = fields_score(board_status);
            r = river_score(board_status);

            for(auto card : complete_cards) a += card.score.back();
            for(auto card : player_cards) a += card.score[card.uses];

            cout << bold() << "YOUR SCORE: \n" << nofont();
            cout << color(46) << "  Trees and bushes: " << nofont() << bold() << t << nofont() << '\n';
            cout << color(242) << "Mountain and hills: " << nofont() << bold() << m << nofont() << '\n';
            cout << color(160) << "         Buildings: " << nofont() << bold() << b << nofont() << '\n';
            cout << color(226) << "            Fields: " << nofont() << bold() << f << nofont() << '\n';
            cout << color(33) << "  Rivers and lakes: " << nofont() << bold() << r << nofont() << "\n\n";

            cout << color(208) << "   Animal habitats: " << nofont() << bold() << a << nofont();
            cout << " ( ";
            for(auto card : complete_cards) cout << card.name << '-' << card.score[card.uses] << " ";
            cout << ") " << '\n';
            cout << "Your unfinished habitats: ( ";
            for(auto card : player_cards) cout << card.name << '-' << card.score[card.uses] << " ";
            cout << ") " << '\n';
            cout << "-------------------- TOTAL SCORE: " << bold() << t+m+b+f+r+a << nofont() << " --------------------\n\n\n";

            cout << "Your habitat board: \n";
            print_board(anchor_status());

            cout  << "Press Enter to return to the Main Menu.\n";
            state = Menu::MainMenu;
            string input;
            getline(cin,input);
            clear_terminal();
            print_title();
            print_main_menu();
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
    }
    return 0;
}