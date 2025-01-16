

/* *
* 
* Solution to course project # 2
* Introduction of programming course
* Faculty of Mathematics and Informatics of Sofia University 
* Winter semester 2024/2025
* 
* @ Tsvetinka Hadzhieva
* @idnumber 9MI0600414* @compiler VC
* 
*/



#include <iostream>
#include <cstdlib> 
#include <ctime> 
#include <fstream> 

#define RESET "\033[0m"
#define BLUE_BG "\033[44m"
#define GREEN_BG "\033[42m"
#define YELLOW_TEXT "\033[43m"


const int Max_Size = 50;   
const int Min_Size = 4; 
const int number_of_operations = 4; 
const int number_of_positions = 3;
char grid[Max_Size][Max_Size][number_of_positions]; 


void clearConsole() {
    system("cls"); 
}

void saveGame(const char grid[][Max_Size][number_of_positions], int rows, int cols, int player1Row, 
    int player1Col, int player2Row, int player2Col, double player1Score, double player2Score, const bool visitedByPlayer1[Max_Size][Max_Size], const bool visitedByPlayer2[Max_Size][Max_Size], int currentPlayer) {
   
    std::ofstream outFile("game_save.txt"); 

    if (!outFile) {
        std::cerr << "Error opening file for saving!" << std::endl; 
        return; 
    }

    outFile << rows << " " << cols << std::endl; 

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            outFile << grid[i][j][0] << grid[i][j][1] << " "; 
        }
        outFile << std::endl; 
    }
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            outFile << visitedByPlayer1[i][j] << " ";
        }
        outFile << std::endl; 
    }
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            outFile << visitedByPlayer2[i][j] << " "; 
        }
        outFile << std::endl; 
    }

    outFile << player1Row << " " << player1Col << std::endl; 
    outFile << player2Row << " " << player2Col << std::endl; 

    outFile << player1Score << " " << player2Score << " " << currentPlayer << std::endl;
    

    outFile.close();
}

bool loadGame(char grid[][Max_Size][number_of_positions], int& rows, int& cols, int& player1Row, 
    int& player1Col, int& player2Row, int& player2Col, double& player1Score, double& player2Score, bool  visitedByPlayer1[Max_Size][Max_Size], bool visitedByPlayer2[Max_Size][Max_Size], int& currentPlayer){

    std::ifstream inFile("game_save.txt");

    if (!inFile) {
        std::cerr << "Error opening file for loading!" << std::endl; 
        return false;
    }

    inFile >> rows >> cols; 

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            inFile >> grid[i][j][0] >> grid[i][j][1]; 
        }
    }
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            inFile >> visitedByPlayer1[i][j];
        }
    }
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            inFile >> visitedByPlayer2[i][j];
        }
    }

    inFile >> player1Row >> player1Col; 
    inFile >> player2Row >> player2Col; 

    inFile >> player1Score >> player2Score; 
    inFile >> currentPlayer; 

    inFile.close(); 
    return true; 
}


void generateRandomCell(char cell[], int MaxNumber) {
    const char operations[] = { '+', '-', 'x', '/'};

    int Random_Operation_Symbol = rand() % number_of_operations; 
    int RandomNumber = 1 + rand() % MaxNumber; 

    cell[0] = operations[Random_Operation_Symbol];

    int index = 1; 

    if (RandomNumber >= 10) {

        cell[index++] = (RandomNumber / 10) + '0'; 
    }

    cell[index++] = (RandomNumber % 10) + '0'; 
    cell[index] = '\0'; 
}


bool hasDividedByTwo(int rows, int cols) {

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (grid[i][j][0] == '/' && grid[i][j][1] == '2') {
                return true;
                break;
            }
        }
    }
    return false;
}

bool hasPositiveNumber(int rows, int cols) {

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (grid[i][j][0] == '+' && (grid[i][j][1] - '0') > 0)
                return true;
                break; 
        }
    }
    return false; 
}

bool hasNegativeNumber(int rows, int cols) {

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (grid[i][j][0] == '-' && (grid[i][j][1] - '0') > 0)
                return true;
                break; 
        }
    }
    return false; 
}

bool hasPoweredByZero(int rows, int cols) {

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (grid[i][j][0] == 'x' && grid[i][j][1] == '0') {
                return true; 
                break; 
            }
        }
    }
    return false; 
}

bool hasPoweredByTwo(int rows, int cols) {

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (grid[i][j][0] == 'x' && grid[i][j][1] == '2') {
                return true; 
                break; 
            }
        }
    }
    return false; 
}

void shuffleGrid(int rows, int cols) {
    int randomRow, randomCol;
    char temp;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {

            randomRow = rand() % rows; 
            randomCol = rand() % cols; 

            for (int k = 0; k < number_of_positions; k++) {
                temp = grid[i][j][k];
                grid[i][j][k] = grid[randomRow][randomCol][k]; 
                grid[randomRow][randomCol][k] = temp; 
            }
        }
    }
}


void generateBalanceGrid(int rows, int cols) {

    int totalCells = rows * cols; 
    int MaxNumber = rows * cols < 25 ? 5 : 10; 

    int positiveCount = totalCells / number_of_operations; // 25%
    int negativeCount = totalCells / number_of_operations; // 25%
    int multiplicationCount = totalCells / number_of_operations; // 25%
    int divisionCount = totalCells -(positiveCount + negativeCount + multiplicationCount); 
    int RandomNumber2;

 
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                RandomNumber2 = 1 + rand() % MaxNumber;
                while (RandomNumber2 >= 10) {
                    RandomNumber2 = 1 + rand() % MaxNumber;
                }
                if (positiveCount > 0) {
                    grid[i][j][0] = '+';
                    grid[i][j][1] = '0' + RandomNumber2;
                    grid[i][j][2] = '\0';
                    positiveCount--;
                }
                else if (negativeCount > 0) {
                    grid[i][j][0] = '-';
                    grid[i][j][1] = '0' + RandomNumber2;
                    grid[i][j][2] = '\0';
                    negativeCount--;
                }
                else if (multiplicationCount > 0) {
                    grid[i][j][0] = 'x';
                    grid[i][j][1] = '0' + RandomNumber2;
                    grid[i][j][2] = '\0';
                    multiplicationCount--;
                }
                else if (divisionCount > 0) {
                    grid[i][j][0] = '/';

                    while (RandomNumber2 == 0) {
                        RandomNumber2 = 1 + rand() % MaxNumber;
                    }
                    grid[i][j][1] = '0' + RandomNumber2;
                    grid[i][j][2] = '\0';
                    divisionCount--;
                }
            }
        }



        if (!hasDividedByTwo(rows, cols)) {
            int randomRow;
            int randomCol;

            do {
                randomRow = rand() % rows;
                randomCol = rand() % cols;
            } while (randomRow < 0 || randomRow >= rows || randomCol < 0 || randomCol >= cols);


            grid[randomRow][randomCol][0] = '/';
            grid[randomRow][randomCol][1] = '2';
            grid[randomRow][randomCol][2] = '\0';
        }

        if (!hasPositiveNumber(rows, cols)) {
            int randomRow;
            int randomCol;

            do {
                randomRow = rand() % rows;
                randomCol = rand() % cols;
            } while (randomRow < 0 || randomRow >= rows || randomCol < 0 || randomCol >= cols);

            int RandomNumber1 = 1 + rand() % MaxNumber;
            while (RandomNumber1 >= 10) {
                RandomNumber1 = 1 + rand() % MaxNumber;
            }

            grid[randomRow][randomCol][0] = '+';
            grid[randomRow][randomCol][1] = '0' + RandomNumber1;
            grid[randomRow][randomCol][2] = '\0';

        }

        if (!hasNegativeNumber(rows, cols)) {
            int randomRow;
            int randomCol;

            do {
                randomRow = rand() % rows;
                randomCol = rand() % cols;
            } while (randomRow < 0 || randomRow >= rows || randomCol < 0 || randomCol >= cols);

            int RandomNumber1 = 1 + rand() % MaxNumber;
            while (RandomNumber1 >= 10) {
                RandomNumber1 = 1 + rand() % MaxNumber;
            }

            grid[randomRow][randomCol][0] = '-';
            grid[randomRow][randomCol][1] = '0' + RandomNumber1;
            grid[randomRow][randomCol][2] = '\0';
        }

        if (!hasPoweredByZero(rows, cols)) {
            int randomRow;
            int randomCol;

            do {
                randomRow = rand() % rows;
                randomCol = rand() % cols;
            } while (randomRow < 0 || randomRow >= rows || randomCol < 0 || randomCol >= cols);


            grid[randomRow][randomCol][0] = 'x';
            grid[randomRow][randomCol][1] = '0';
            grid[randomRow][randomCol][2] = '\0';
        }

        if (!hasPoweredByTwo(rows, cols)) {
            int randomRow;
            int randomCol;

            do {
                randomRow = rand() % rows;
                randomCol = rand() % cols;
            } while (randomRow < 0 || randomRow >= rows || randomCol < 0 || randomCol >= cols);


            grid[randomRow][randomCol][0] = 'x';
            grid[randomRow][randomCol][1] = '2';
            grid[randomRow][randomCol][2] = '\0';
        }

        shuffleGrid(rows, cols);

        grid[0][0][0] = '0';
        grid[0][0][1] = '\0';

        if (Max_Size >= rows && Max_Size >= cols) {
            if (rows > 0 && cols > 0) {
                grid[rows - 1][cols - 1][0] = '0';
                grid[rows - 1][cols - 1][1] = '\0';
            }
        }
   
}

bool visitedByPlayer1[Max_Size][Max_Size] = { false };
bool visitedByPlayer2[Max_Size][Max_Size] = { false };

void Borders_of_Grid(int rows, int cols,double player1Score, double player2Score, int player1Row, int player1Col, 
    int player2Row, int player2Col, bool selected[Max_Size][Max_Size], char grid[Max_Size][Max_Size][number_of_positions], bool visitedByPlayer1[Max_Size][Max_Size], bool visitedByPlayer2[Max_Size][Max_Size]) {

  
    for (int j = 0; j < cols; j++) {
        std::cout << "   " << j << " "; 
    }
    std::cout << std::endl; 


    std::cout << " ";
    for (int j = 0; j < cols; j++) {
        std::cout << "+----";
    }
    std::cout << "+" << std::endl; 


    for (int i = 0; i < rows; i++) {
        std::cout << i; 
        for (int j = 0; j < cols; j++) {
            std::cout << "| ";
            if (visitedByPlayer1[i][j]) {
                std::cout << BLUE_BG;
            }
            else if (visitedByPlayer2[i][j]) {
                std::cout << GREEN_BG; 
            }
            else if (i == player1Row && j == player1Col) {
                std::cout << BLUE_BG; 
            }
            else if (i == player2Row && j == player2Col) {
                std::cout << GREEN_BG; 
            }

            std::cout << grid[i][j][0] << grid[i][j][1];
            std::cout << RESET; 

            if (grid[i][j][1] == '\0') {
                std::cout << "  ";
            }
            else {
                std::cout << " ";
            }
        }
        std::cout << "|"; 
        std::cout << std::endl; 
        
        std::cout << " "; 
        for (int j = 0; j < cols; j++) {
              std::cout << "+----";
        }
             std::cout << "+" << std::endl; 
    }

    std::cout << "Player 1 (BLUE): " << player1Score << " "; 
    std::cout << "Player 2 (GREEN): " << player2Score << std::endl; 
}
   



int abs1(int number) {
    return(number < 0) ? -number : number;
}

bool isPlayer1(const char playerName[]) {
    if (playerName[0] == 'P' && 
        playerName[1] == 'l' && 
        playerName[2] == 'a' && 
        playerName[3] == 'y' && 
        playerName[4] == 'e' && 
        playerName[5] == 'r' && 
        playerName[6] == ' ' && 
        playerName[7] == '1' && 
        playerName[8] == '\0') {

        return true; 
    }
    return false;

}

bool selected[Max_Size][Max_Size] = { false }; 

void playerTurn(int rows, int cols, double& playerScore, int& currentRow, int& currentCol, const char playerName[], 
    bool selected[Max_Size][Max_Size], bool visitedByPlayer1[Max_Size][Max_Size], bool visitedByPlayer2[Max_Size][Max_Size]) {


    int selectedRow, selectedCol; 
    visitedByPlayer1[0][0] = true; 
    visitedByPlayer2[rows - 1][cols - 1] = true; 

    while (true) {
        std::cout << playerName << "'s turn. Enter a row and column: "; 
        std::cin >> selectedRow >> selectedCol; 

        if (selectedRow < 0 || selectedRow >= rows || selectedCol < 0 || selectedCol >= cols) {
            std::cout << "The cell is out of bounds! Please choose a valid cell within the grid!"; 
            continue; 
        }

        if (selected[selectedRow][selectedCol]) {
            std::cout << "This cell has already been chosen! Please choose a different cell"; 
            continue; 
        }

        if (abs1(selectedRow - currentRow) > 1 || abs1(selectedCol - currentCol) > 1) {
            std::cout << "You can only move to a neighboring cell! Try again!" << std::endl; 
            continue; 
        }
        break; 
    }

    selected[selectedRow][selectedCol] = true; 

    if (isPlayer1(playerName)) {

        visitedByPlayer1[selectedRow][selectedCol] = true; 
    }
    else {
        visitedByPlayer2[selectedRow][selectedCol] = true; 
    }

    currentRow = selectedRow; 
    currentCol = selectedCol; 

    char operation = grid[selectedRow][selectedCol][0]; 
    int number = (grid[selectedRow][selectedCol][1] - '0'); 

    switch (operation) {
    case '+':
        playerScore += number; 
        break;
    case '-':
        playerScore -= number;
        break;
    case 'x':
        playerScore *= number; 
        break; 
    case '/':
        playerScore /= number; 
        break; 
    default:
        std::cout << "Invalid operation detected" << std::endl; 
    }

    std::cout << playerName << "'s current score: " << playerScore << std::endl; 
}


bool hasNeighborCell(int currentRow, int currentCol, int rows, int cols, bool selected[Max_Size][Max_Size]) {

    for (int i = (currentRow - 1); i <= (currentRow + 1); i++) {
        for (int j = (currentCol - 1); j <= (currentCol + 1); j++) {
            if (i == currentRow && j == currentCol) {
                continue; 
            }

            if (i >= 0 && i < rows && j >= 0 && j < cols && !selected[i][j]) {
                return true; 
            }
        }
    }
    return false;
}

int rows, cols;
int currentPlayer;
double player1Score = 0;
double player2Score = 0;

int player1Row = 0;
int player1Col = 0;

int player2Row;
int player2Col;



int main()
{
    srand(static_cast<unsigned int>(time(0))); 

    bool gameIsRunning = true; 
    
    std::cout << " 1. New Game" << std::endl; 
    std::cout << " 2. Load Game" << std::endl; 
    std::cout << "Enter your choice: ";
    int choice; 
    std::cin >> choice; 
    bool RightChoice = true; 

    while (RightChoice == true) {

        if (choice == 2) {
            
            if (loadGame(grid, rows, cols, player1Row, player1Col, player2Row, player2Col, player1Score, player2Score, visitedByPlayer1, visitedByPlayer2, currentPlayer)) {
                std::cout << "Game successfully loaded!" << std::endl;
                Borders_of_Grid(rows, cols, player1Score, player2Score, player1Row, player1Col, player2Row, player2Col, selected, grid, visitedByPlayer1, visitedByPlayer2);
                RightChoice = false;
            }
            else {
                std::cout << "Failed to load game. Starting a new game ..." << std::endl;
                choice = 1;
            }
        }
        else if (choice == 1) {
            RightChoice = false; 
            do {
                currentPlayer = 0;
                std::cout << "Enter grid width(minimum 4): ";
                std::cin >> cols;
                std::cout << "Enter grid height(minimum 4): ";
                std::cin >> rows;

                if (rows < Min_Size || cols < Min_Size) {
                    std::cout << "Both rows and colums must be at least 4. Please, try again!" << std::endl;
                }
                if (rows > Max_Size || cols > Max_Size) {
                    std::cout << "Both rows and colums must not be greater than 50. Please, try again!" << std::endl;
                }
            } while (rows < Min_Size || cols < Min_Size || rows > Max_Size || cols > Max_Size);
            player2Row = rows - 1;
            player2Col = cols - 1;

            generateBalanceGrid(rows, cols);
            Borders_of_Grid(rows, cols, player1Score, player2Score, player1Row, player1Col, player2Row, player2Col, selected, grid, visitedByPlayer1, visitedByPlayer2);



        }
        else {
            std::cout << "Invalid entry. Enter your choice again: ";
            std::cin >> choice;
        }
    }
    selected[player1Row][player1Col] = true;
    selected[player2Row][player2Col] = true;

    while (gameIsRunning == true) {

         
            if (currentPlayer == 0) {

                if (!hasNeighborCell(player1Row, player1Col, rows, cols, selected)) {
                    std::cout << "Player 1 has no valid moves! Game over." << std::endl;
                    std::cout << "Player 1's score: " << player1Score << std::endl;
                    std::cout << "Player 2's score: " << player2Score << std::endl;

                    if (player1Score > player2Score) {
                        std::cout << "Player 1 wins!" << std::endl;
                        break;
                    }
                    else if (player2Score > player1Score) {
                        std::cout << "Player 2 wins!" << std::endl;
                        break;
                    }
                    else {
                        std::cout << "It is a tie!" << std::endl;
                        break;
                    }
                }
                playerTurn(rows, cols, player1Score, player1Row, player1Col, "Player 1", selected, visitedByPlayer1, visitedByPlayer2);
                clearConsole(); 

                Borders_of_Grid(rows, cols,player1Score, player2Score, player1Row, player1Col, player2Row, player2Col, selected, grid, visitedByPlayer1, visitedByPlayer2); 
                currentPlayer = 1;


            }

            else {

                if (!hasNeighborCell(player2Row, player2Col, rows, cols, selected)) {
                    std::cout << "Player 2 has no valid moves! Game over." << std::endl;
                    std::cout << "Player 1's score: " << player1Score << std::endl;
                    std::cout << "Player 2's score: " << player2Score << std::endl;

                    if (player1Score > player2Score) {
                        std::cout << "Player 1 wins!" << std::endl;
                        break;
                    }
                    else if (player2Score > player1Score) {
                        std::cout << "Player 2 wins!" << std::endl;
                        break;
                    }
                    else {
                        std::cout << "It is a tie!" << std::endl;
                        break;
                    }
                }
                playerTurn(rows, cols, player2Score, player2Row, player2Col, "Player 2", selected, visitedByPlayer1, visitedByPlayer2);
                clearConsole(); 
                Borders_of_Grid(rows, cols, player1Score, player2Score, player1Row, player1Col, player2Row, player2Col, selected, grid, visitedByPlayer1, visitedByPlayer2);
                currentPlayer = 0; 

            }
            std::cout << " Do you want to save the game?(1 -> YES and 0 -> NO): ";
            char saveChoice;
            bool Is_Rigth_Choice = true; 

            while (Is_Rigth_Choice == true) {
                std::cin >> saveChoice; 


                if (saveChoice == '1') {
                    Is_Rigth_Choice = false; 
                    
                    saveGame(grid, rows, cols, player1Row, player1Col, player2Row, player2Col, player1Score, player2Score, visitedByPlayer1, visitedByPlayer2, currentPlayer);
                    std::cout << "Game saved successfully" << std::endl;

                    gameIsRunning = false; 
                    break; 
                }

                else if (saveChoice == '0') {
                    Is_Rigth_Choice = false; 
                    clearConsole();
                    Borders_of_Grid(rows, cols, player1Score, player2Score, player1Row, player1Col, player2Row, player2Col, selected, grid, visitedByPlayer1, visitedByPlayer2);
                    break; 
                }

                else {
                    std::cout << "Choose only between 1 and 0!" << std::endl;
                }
            }
           
    }


    return 0; 
}



