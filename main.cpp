#include <iostream>
#include <conio.h>
#include <windows.h>
#include <iomanip>
#include <cmath>
// Variables to change field size and the pase of the game
int frames = 1;
const int terrain_height = 20, terrain_width = 70;
float pase = 8.33;

// Player class
class Player {
private:
    // Player's barrier starting position. It's made to start at the center
    int height = round(terrain_height / 2) - 2; 
    int player_lenght = round(terrain_height / 3);
public:
    int getHeight(){
        return height;
    }
    int getPlayerLenght(){
        return player_lenght;
    }
    void moveUp(){
        if (height > 0){
            height--;
        }
    }
    void moveDown(){
        if(height < terrain_height - player_lenght){
            height++;
        }
    }
};

// Ping pong class
class PingPong {
private:
    int posX, posY;
    int accelerateX, accelerateY;
public:
    // Constructor for the ping_pong ball
    PingPong(int posX = 0, int posY = 0, int accelerateX = 1, int accelerateY = 1) {
        this->posX = posX;
        this->posY = posY;
        this->accelerateX = accelerateX;
        this->accelerateY = accelerateY;
    }

    // Gets positions for the ping_pong ball
    int getPosX(){
        return posX;
    }
    int getPosY(){
        return posY;
    }

    // Functions to move the ping_pong balls location
    void movePosX(){
        // Checks if the next movement will exceed the right boundary
        if(posX + accelerateX <= terrain_width - 1 && posX + accelerateX >= 0){
            posX += accelerateX;
        }
        // Reverses the direction if the boundary is reached
        else{
            accelerateX = -accelerateX;
            posX += accelerateX;
        }
    }
    void movePosY(){
        // Checks if the next movement will exceed the bottom boundary
        if(posY + accelerateY < terrain_height && posY + accelerateY >= 0){
            posY += accelerateY;
        }
        else{
            accelerateY = -accelerateY;
            posY += accelerateY;
        }
        
    }
};

void mainMenu();
void updateGame(Player* player1, Player* player2, PingPong* ping_pong, char terrain[][terrain_width]);
void playerMovement(Player* player1, Player* player2);
void draw(Player* player1, Player* player2, PingPong* ping_pong, char terrain[][terrain_width]);
bool checkWinner(Player* player1, Player* player2, PingPong* ping_pong, char terrain[][terrain_width]);

int main(){
    // Starts the main menu
    mainMenu();

    // Creates necessary variables
    Player player1, player2;
    PingPong ping_pong(10, 4, 1, 1);
    char terrain[terrain_height][terrain_width]{};
    bool running = true;

    while(running){
        // Checks for collisions with the player barriers for a winner
        if(checkWinner(&player1, &player2, &ping_pong, terrain)){
            running = false;
            break;
        }

        // Updates the game
        updateGame(&player1, &player2, &ping_pong, terrain);
    }

    return 0;
}

void mainMenu(){
    std::cout << "***Press space to start the game***" << std::endl;

    // When the player presses the spacebar start() function will get called
    while(true){
        if(_getch() == ' '){
            return;
        }
    }
}

void updateGame(Player* player1, Player* player2, PingPong* ping_pong, char terrain[][terrain_width]){
    // Calls the move function to move the ping_pong ball
    ping_pong->movePosX();
    ping_pong->movePosY();
      
    // Check for player movement
    playerMovement(player1, player2);

    // Calls the draw funtion
    draw(player1, player2, ping_pong, terrain);

    // Shows how many frames have passes
    std::cout << frames << std::endl;
    frames++;

    Sleep(pase); //125
    // pase -= 5;
}

void playerMovement(Player* player1, Player* player2){
    if(_kbhit()){
        char key = _getch();
        switch(key){
            case 'w':
                player1->moveUp();
                break;
            case 's':
                player1->moveDown();
                break;
            case 'i':
                player2->moveUp();
                break;
            case 'k':
                player2->moveDown();
                break;
        }
    }
}

void draw(Player* player1, Player* player2, PingPong* ping_pong, char terrain[][terrain_width]){
    // Clears the screen
    // system("CLS"); works for codeblocks
    std::cout << "\033[2J\033[01;1H"; // works for vscode

    // Clears the terrain
    for(size_t i = 0; i < terrain_height; i++){
        for(size_t j = 0; j < terrain_width; j++) {
            terrain[i][j] = ' ';
        }
    }
    // Places the ping_pong inside the matrix
    terrain[ping_pong->getPosY()][ping_pong->getPosX()] = '*';

    // Places the barriers for both players
    for(size_t counter = 0; counter < player1->getPlayerLenght(); counter++){
        terrain[player1->getHeight() + counter][0] = '|';
    }
    for(size_t counter = 0; counter < player2->getPlayerLenght(); counter++){
        terrain[player2->getHeight() + counter][terrain_width - 1] = '|';
    }
    
    // Prints out the terrain
    std::string terrain_lenght(terrain_width, '.');
    std::cout << terrain_lenght << std::endl;
    for(size_t i = 0; i < terrain_height; i++){
        for(size_t j = 0; j < terrain_width; j++){
            std::cout << terrain[i][j];
        }
        std::cout << std::endl;
    }
    std::cout << terrain_lenght << std::endl;
}

bool checkWinner(Player* player1, Player* player2, PingPong* ping_pong, char terrain[][terrain_width]){
    // Gets the ball location
    int ballX = ping_pong->getPosX();
    int ballY = ping_pong->getPosY();

    // Checks if the ball is at the edges of the terrain
    if(ballX != 0 && ballX != terrain_width - 1){
        return false;
    }
    
    // Checks collisions with player1
    int collision_counter = 0;
    for(int i = player1->getHeight(); i < player1->getHeight() + player1->getPlayerLenght(); i++){
        if(ballX == 0 && ballY != i){
            collision_counter += 1;
        }
    }
    if(collision_counter == player1->getPlayerLenght()){
        std::cout << "Player 2 wins!" << std::endl;
        return true;
    }

    // Check collisions with player2
    collision_counter = 0;
    for(int i = player2->getHeight(); i < player2->getHeight() + player2->getPlayerLenght(); i++){
        if(ballX == terrain_width - 1 && ballY != i){
            collision_counter += 1;
        }
    }
    if(collision_counter == player2->getPlayerLenght()){
        std::cout << "Player 1 wins!" << std::endl;
        return true;
    }

    // No collisions detected
    return false;
}


