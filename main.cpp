#include <iostream>
#include <conio.h>
#include <windows.h>
#include <iomanip>
#include <cmath>
// Variables to change field size and the pase of the game
int frames = 1;
const int terrain_height = 8, terrain_width = 25;
float pase = 500;

class Player {
private:
    // Player's barrier starting position. It's made to start at the center
    int height = round(terrain_height / 2) - 1; 
    int player_lenght = round(terrain_height / 3) + 1;
    int wins = 0;
public:
    int getHeight(){
        return height;
    }
    int getPlayerLenght(){
        return player_lenght;
    }

    // Resets the player positions after a point
    void resetPlayerHeight(){
        height = round(terrain_height / 2) - 1;
    }

    // Player movement
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

    // Checks and gives player wins
    int getWins(){
        return wins;
    }
    void giveWin(){
        wins += 1;
    }
};

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

    void resetPosition(){
        posX = round(terrain_width / 2);
        posY = round(terrain_height / 2);
        accelerateX = -accelerateX;
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

bool mainMenu();
void credits();
void updateGame(Player* player1, Player* player2, PingPong* ping_pong, char terrain[][terrain_width]);
void playerMovement(Player* player1, Player* player2);
void draw(Player* player1, Player* player2, PingPong* ping_pong, char terrain[][terrain_width]);
bool checkWinner(Player* player1, Player* player2, PingPong* ping_pong, char terrain[][terrain_width]);
void showScore(Player* player1, Player* player2);

int main(){
    // Starts the main menu
    if(mainMenu() != true){
        return 0;
    }

    // Creates necessary variables
    Player player1, player2;
    PingPong ping_pong(round(terrain_width / 2), round(terrain_height / 2), 1, 1);
    char terrain[terrain_height][terrain_width]{};
    bool running = true;

    while(player1.getWins() < 3 && player2.getWins() < 3){
        // Checks for collisions with the player barriers for a winner
        if(checkWinner(&player1, &player2, &ping_pong, terrain)){
            Sleep(3000);
            player1.resetPlayerHeight();
            player2.resetPlayerHeight();
            ping_pong.resetPosition();
            pase = 500;
        }

        // Updates the game
        updateGame(&player1, &player2, &ping_pong, terrain);
    }
    // Clears the screen
    std::cout << "\033[2J\033[01;1H";

    // Shows the final results
    showScore(&player1, &player2);
    return 0;
}

bool mainMenu(){    
    while(true){
        std::cout << "\033[2J\033[01;1H"; // works for vscode (CLS)
        std::cout << " -------------------------------------------------------  " << std::endl;
        std::cout << "|                                                       | " << std::endl;
        std::cout << "|   ***** * *   * *****    ***** ***** *   * *****      | " << std::endl;
        std::cout << "|   *   * * **  * *        *   * *   * **  * *          | " << std::endl;
        std::cout << "|   ***** * * * * * ***    ***** *   * * * * * ***      | " << std::endl;
        std::cout << "|   *     * *  ** *   *    *     *   * *  ** *   *      | " << std::endl;
        std::cout << "|   *     * *   * *****    *     ***** *   * *****      | " << std::endl;
        std::cout << "|                                                  v1.1 | " << std::endl;
        std::cout << " -------------------------------------------------------  " << std::endl;
        std::cout << std::endl;

        std::cout << "*** MENU: ***" << std::endl;
        std::cout << "Press 1 to start the game" << std::endl;
        std::cout << "Press 2 to see the credits" << std::endl;
        std::cout << "Press 3 to exit the game" << std::endl;

        // When the player presses the a specific key a certain function will get called
        char key = _getch();
        switch(key){
            case '1':
                return true;
                break;
            case '2':
                credits();
                break;
            case '3':
                return false;
                break;
        }
    }
}

void credits(){
    std::cout << "\033[2J\033[01;1H"; // works for vscode (CLS)
    std::cout << std::endl;
    std::cout << "Lead programmer: Dzonex" << std::endl;
    std::cout << "Designer: Dzonex" << std::endl;
    std::cout << "Testers: Dzonex" << std::endl;
    std::cout << "Version: 1.1" << std::endl;
    std::cout << std::endl;

    std::cout << "Press any key to go back" << std::endl;
    while(true){
        if(_getch()){
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

    // Makes the game have a pase
    Sleep(pase);
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
        std::cout << "Player 2 gets the point!" << std::endl;
        player2->giveWin();
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
        std::cout << "Player 1 gets the point!" << std::endl;
        player1->giveWin();
        return true;
    }

    // No collisions detected
    pase -= 10;
    return false;
}

void showScore(Player* player1, Player* player2){
    std::cout << " -----------------------------------------------------------  " << std::endl;
    std::cout << "|                                                           | " << std::endl;
    std::cout << "|   ***** ***** *   * *****    ***** *   * ***** *****      | " << std::endl;
    std::cout << "|   *     *   * ** ** *        *   * *   * *     *   *      | " << std::endl;
    std::cout << "|   * *** ***** * * * *****    *   * *   * ***** *****      | " << std::endl;
    std::cout << "|   *   * *   * *   * *        *   *  * *  *     * *        | " << std::endl;
    std::cout << "|   ***** *   * *   * *****    *****   *   ***** *   *      | " << std::endl;
    std::cout << "|                                                      v1.1 | " << std::endl;
    std::cout << " -----------------------------------------------------------  " << std::endl;
    std::cout << std::endl;
    
    std::cout << "Final score:" << std::endl;
    std::cout << player1->getWins() << " | " << player2->getWins() << std::endl;

    if(player1->getWins() > player2->getWins()){
        std::cout << "Player 1 wins!" << std::endl;
    }
    else{
        std::cout << "Player 2 wins!" << std::endl;
    }
}