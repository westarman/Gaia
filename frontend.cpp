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
    PickPool, PickCard, PlaceToken, PlaceAnchor, ViewToken, ViewAnchor
};

int state = Menu::MainMenu;

//GAME MEMORY
//one only
vector<int> tbag(0);
vector<vector<int>> tboard(0);
vector<Animal> card_deck(0);
int max_card_board = 2;

//per player
vector<Tile> board_status(0);
vector<Animal> player_cards(0);
int max_player_cards = 3;
vector<int> player_tokens(0);


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
        cout << "\33[38;5;226m" << "  Field w  " << "\33[0m";
        break;
    
        case Token::Grass:
        cout << "\33[38;5;46m" << " Grass ;,; " << "\33[0m";
        break;
        
        case Token::Mountain:
        cout << "\33[38;5;242m" << "Mountain /\\" << "\33[0m";
        break;
        
        case Token::Trunk:
        cout << "\33[38;5;94m" << "  Trunk @  " << "\33[0m";
        break;
    
        case Token::Water:
        cout << "\33[38;5;33m" << "  Water ~  " << "\33[0m";
        break;
    }
}

void print_token_pools() {
    //TODO make board print out in columns instead of rows
    cout << "Token pools: \n";
    for(int i = 0; i < pool_count; i++) {
        cout << i << ": | ";
        for(int node : tboard[i]){
            print_token(node);
            cout << " | ";
        }
        cout << "\n";
    }
    cout << "\nSelect a token pool.\n";
}

void print_player_tokens() {
    //TODO
    cout << "printed player tokens\n";
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

string stack(int n) {
    string s = "\33[1m";
    for(int x : board_status[n].stack) {
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

void print_board(vector<int> mask) {
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
    for(int i = 0; i < cols; i++) cout << "| " << stack((2*colsize-1)*i) << " |     "; 
    cout << '\n';

    //2
    cout << " \\ " << (mask[0] ? "o" : " ")  << " /";
    for(int i = 1; i < cols; i++) cout << " /" << triple0(colsize*i+(colsize-1)*(i-1)) << "\\ \\ " << (mask[(2*colsize-1)*i] ? "o" : " ") << " /";
    cout << '\n';

    //3452
    for(int i = 1; i < colsize-1; i++) {
        //3
        cout << "   |  ";
        for(int j = 1; j < cols; j++) cout << "| " << stack(colsize*j+(colsize-1)*(j-1)+(i-1)) << " |  |  ";
        cout << '\n';

        //4
        cout << " /" << triple0(i) << "\\";
        for(int j = 1; j < cols; j++) cout << " \\ " << (mask[colsize*j+(colsize-1)*(j-1)+(i-1)] ? "o" : " ") << " / /" << triple0((2*colsize-1)*j+i) << "\\";
        cout << '\n';
        
        //5
        cout << "| " << stack(i) << " |";
        for(int j = 1; j < cols; j++) cout << "  |  | " << stack((2*colsize-1)*j+i) << " |";
        cout << '\n';

        //2
        cout << " \\ " << (mask[i] ? "o" : " ")  << " /";
        for(int j = 1; j < cols; j++) cout << " /" << triple0(colsize*j+(colsize-1)*(j-1)+i) << "\\ \\ " << (mask[(2*colsize-1)*j+i] ? "o" : " ") << " /";
        cout << '\n';
    }

    //3
    cout << "   |  ";
    for(int i = 1; i < cols; i++) cout << "| " << stack(colsize*i+(colsize-1)*(i-1)+(colsize-2)) << " |  |  ";
    cout << '\n';

    //4
    cout << " /" << triple0(colsize-1) << "\\";
    for(int i = 1; i < cols; i++) cout << " \\ " << (mask[colsize*i+(colsize-1)*(i-1)+(colsize-2)] ? "o" : " ") << " / /" << triple0((2*colsize-1)*i+(colsize-1)) << "\\";
    cout << '\n';
    
    //1
    for(int i = 0; i < cols; i++) cout << "| " << stack((2*colsize-1)*i+(colsize-1)) << " |     "; 
    cout << '\n';

    //6
    for(int i = 0; i < cols; i++) cout << " \\ " << (mask[(2*colsize-1)*i+(colsize-1)] ? "o" : " ") << " /      ";

    cout << "\n\n";
}

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
            break;    
        case 2:
            cout << "not implemented yet...\n\n";
            break;
        case 3:
            cout << "not implemented yet...\n\n";
            break;
        case 4:
            cout << "\033[38;5;1mWARNING: Nuclear launch detected! ETA: 3s\n";
            this_thread::sleep_for(chrono::milliseconds(500));
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

// gameloop substate
void mainturn(int order, vector<string> &in) {
    if(player_tokens.empty() && order != Order::PickPool) {
        cout << "Please select a token pool first.\n";
        return;
    }
        //pick pool <- 'pick id'
        //place token <- 'place t id'
        //place animal <- 'place name id'
        //view valid tokens <- 'view t'
        //view valid anchors <- 'view name'
        //pick card <- 'take name'
        //end turn <- 'end'
        //help

    switch(order) {
    case Order::PickPool:
        if(in[1][0] < '1' || in[1][0] > (pool_size+1+'0')) {
            cout << "\033[38;5;1m" << "! - invalid token pool, a value in range 1-" << pool_count << " is required\n\n" << "\033[0m";
            return;
        }
        player_tokens = select_token_pool(tboard,tbag,stoi(in[1])-1);
        print_board(anchor_status());
        print_player_tokens();
        break;
    
    case Order::PlaceToken:

        break;

    default:
        break;
    }
}

// only determines which order is given (validity is checked later)
int give_order(vector<string> &in) {
    string o = in[0];
    if(o == "pick") {
        return Order::PickPool;
    } else if(o == "place") {
        if(in[1].size() > 1) {
            return Order::PlaceAnchor;
        } else {
            return Order::PlaceToken;
        }
    } else if(o == "view") {
        //TODO
    } else if(o == "end") {
        //TODO
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
            if(in[0][0] < '1' || in[0][0] > '4' || in.size()>1 || in[0].size()>1) {
                cout << "\033[38;5;1m" << "! - invalid input\n\n" << "\033[0m";
                return;
            }
            int n = stoi(in[0]);
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
            
            const Tile t;
            for(int i = 0; i < nodes; i++) board_status.push_back(t);

            print_token_pools();
            state = MainTurn;
            break; }
        
        case Menu::Settings:
            break;
        

        case Menu::MainTurn: {
            // picks pool(special check)
            // user can pick cards, place tokens, place anchors as well as view valid placements
            // end turn option
            // MainTurn -> TokenSelection OR MainTurn -> EndGame

            int order = give_order(in);
            if(order<0) return;
            mainturn(order,in);
            break; }
        case Menu::EndGame:
            //post game screen
            //scoring, clearing memory etc.
            // EndGame -> MainMenu
            break;
        default:
            cout << "wtf\n";
            break;
    }
} 

int main() {
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

