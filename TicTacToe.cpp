#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <cstring>
#include <sstream>

using std::vector;

class Grid {
public:
    int width;
    int height;
    vector<int> dataVec;
    Grid(int givenWidth, int givenHeight) : width(givenWidth), height(givenHeight) {
        for (int i = 0; i < width*height; i++)
        {
            dataVec.push_back(0); //0 = blank, 1 = X, 2 = O;
        }
    }

    static char convertToGraphic(int givenInt)
    {
        //Converts 0 to -, 1 to X, and 2 to O.
        char newChar = 45; //"-"
        if (givenInt == 1)
        {
            newChar = 88; //"X"
        }
        if (givenInt == 2)
        {
            newChar = 79; //"O"
        }
        return newChar;
    }

    int convertTupleToInt (std::tuple<int,int> givenTuple) const
    {
        //Converts a tuple to an integer, allowing for it to be used as an input for the Grid.
        int returnInt = 0;
        returnInt += std::get<0>(givenTuple);
        returnInt += std::get<1>(givenTuple) * width;
        returnInt -= 1;
        returnInt -= width;
        return returnInt;
    }
    std::tuple<int,int> convertIntToTuple(int givenInt) const
    {
        //Converts an integer to a tuple, allowing for it to be shown as a 2D coordinate.
        int xCoord = givenInt%width + 1;
        int yCoord = 1;
        for (int i = givenInt; i >= width; i -= height)
        {
            yCoord += 1;
        }
        std::tuple <int,int> returnTuple(xCoord,yCoord);
        return returnTuple;
    }

    void printGrid()
    {
        //Displays the grid in the command prompt, along with bordering numbers
        //to help the player know what the coordinates of certain spaces are.
        std::cout << "  ";
        for (int i = 1; i <= width; i++)
        {
            std::cout << i << " ";
        }
        int heightTracker = 0;
        int yDisplayTracker = 0;
        for (int i : dataVec)
        {
            if (heightTracker == 0)
            {
                std::cout << std::endl;
                yDisplayTracker++;
                std::cout << yDisplayTracker << " ";
                heightTracker = width;
            }
            else
            {
                std::cout << " ";
            }
            std::cout << convertToGraphic(i);
            heightTracker--;
        }
        std::cout << std::endl;
    }

    bool playerAction(std::string givenString)
    {
        //Takes an input string, and tries to create an X and Y coordinate out of it.
        //It then will try to change the value of the corresponding place in the grid.
        //If the space is occupied, or if an error is otherwise thrown, returns false.
        int xCoord;
        int yCoord;
        long unsigned int trueCoord;
        std::stringstream playerInputStream(givenString);
        std::string temp;
        try {
            getline(playerInputStream, temp, ',');
            xCoord = stoi(temp);
            getline(playerInputStream, temp, ',');
            yCoord = stoi(temp);
            trueCoord = convertTupleToInt(std::tuple<int, int>(xCoord, yCoord));
            if (trueCoord >= dataVec.size()) {
                return false;
            }
            if (dataVec[trueCoord] == 0) {
                dataVec[trueCoord] = 1;
                return true;
            } else {
                return false;
            }
        }
        catch(...)
        {
            return false;
        }
    }

    void computerAction()
    {
        //Computer makes a move in a random unoccupied space. Given more time, a smarter algorithm would have been implemented.
        vector<int> availableSpaces;
        for (long unsigned int i = 0; i < dataVec.size(); i++)
        {
            if (dataVec[i] == 0)
            {
                availableSpaces.push_back(i);
            }
        }
        srand((unsigned) time(NULL));
        int randomInt = rand();
        randomInt = randomInt%availableSpaces.size();
        dataVec[availableSpaces[randomInt]] = 2;
    }
    std::tuple<int,int> exampleLocation()
    {
        //Used when the player makes an invalid move. Looks through every available space, and returns a tuple giving the location.
        vector<int> availableSpaces;
        for (long unsigned int i = 0; i < dataVec.size(); i++)
        {
            if (dataVec[i] == 0)
            {
                availableSpaces.push_back(i);
            }
        }
        srand((unsigned) time(NULL));
        int randomInt = rand();
        randomInt = randomInt%availableSpaces.size();
        return convertIntToTuple(availableSpaces[randomInt]);
    }
    int checkForVictory()
    {
        //Checks every possible way of winning. If the board is not a square, diagonal victories are not possible.
        bool playerVictory;
        bool computerVictory;
        bool boardIsFull = true;
        //Horizontal Victory
        for (long unsigned int i = 0; i < dataVec.size(); i += width)
        {
            playerVictory = true;
            computerVictory = true;
            for (long unsigned int j = i; j < i+width; j++)
            {
                if (dataVec[j] == 0)
                {
                    playerVictory = false;
                    computerVictory = false;
                }
                if (dataVec[j] == 1)
                {
                    computerVictory = false;
                }
                if (dataVec[j] == 2)
                {
                    playerVictory = false;
                }
            }
            if (playerVictory)
            {
                //std::cout << "Horizontal";
                return 1;
            }
            if (computerVictory)
            {
                //std::cout << "Horizontal";
                return 2;
            }
        }
        //Vertical Victory
        for (long unsigned int i = 0; i < width; i++)
        {
            playerVictory = true;
            computerVictory = true;
            for (long unsigned int j = i; j < dataVec.size(); j+= width)
            {
                if (dataVec[j] == 0)
                {
                    playerVictory = false;
                    computerVictory = false;
                }
                if (dataVec[j] == 1)
                {
                    computerVictory = false;
                }
                if (dataVec[j] == 2)
                {
                    playerVictory = false;
                }
            }
            if (playerVictory)
            {
                //std::cout << "Vertical";
                return 1;
            }
            if (computerVictory)
            {
                //std::cout << "Vertical";
                return 2;
            }
        }
        if (width == height) {
            //Diagonal Victory 1
            playerVictory = true;
            computerVictory = true;
            for (long unsigned int i = 0; i < dataVec.size(); i += width + 1) {
                if (dataVec[i] == 0) {
                    playerVictory = false;
                    computerVictory = false;
                }
                if (dataVec[i] == 1) {
                    computerVictory = false;
                }
                if (dataVec[i] == 2) {
                    playerVictory = false;
                }
            }
            if (playerVictory) {
                //std::cout << "Diagonal 1";
                return 1;
            }
            if (computerVictory) {
                //std::cout << "Diagonal 1";
                return 2;
            }
            //Diagonal Victory 2
            playerVictory = true;
            computerVictory = true;
            for (long unsigned int i = width - 1; i <= dataVec.size() - width; i += width - 1) {
                if (dataVec[i] == 0) {
                    playerVictory = false;
                    computerVictory = false;
                }
                if (dataVec[i] == 1) {
                    computerVictory = false;
                }
                if (dataVec[i] == 2) {
                    playerVictory = false;
                }
            }
            if (playerVictory) {
                //std::cout << "Diagonal 2";
                return 1;
            }
            if (computerVictory) {
                //std::cout << "Diagonal 2";
                return 2;
            }
        }
        //Check to see if board is full
        for (int i : dataVec)
        {
            if (i == 0)
            {
                boardIsFull = false;
            }
        }
        if (boardIsFull)
        {
            return 3;
        }
        return 0;
    }
};



int main(int argc, char const* argv[]) {

    std::string playerInput;
    bool widthSelected = false;
    bool heightSelected = false;
    bool orderSelected = false;
    int desiredWidth;
    int desiredHeight;
    int victor = 0;

    while (true)
    {
        std::cout << "Welcome to Tic-Tac-Toe!" << std::endl;
        std::cout << "How wide do you want the board?" << std::endl;
        while (!widthSelected) {
            try
            {
                std::cin >> playerInput;
                desiredWidth = std::stoi(playerInput);
                if (desiredWidth == 0)
                {
                    throw std::invalid_argument("");
                }
                widthSelected = true;
            }
            catch(...) {
                std::cout << "Invalid Input. Just type a number. And make sure it isn't 0." << std:: endl;
            }
        }
        std::cout << "How tall do you want the board?" << std::endl;
        while (!heightSelected) {
            try
            {
                std::cin >> playerInput;
                desiredHeight = std::stoi(playerInput);
                if (desiredHeight == 0)
                {
                    throw std::invalid_argument("");
                }
                heightSelected = true;
            }
            catch(...) {
                std::cout << "Invalid Input. Just type a number. And make sure it isn't 0." << std:: endl;
            }
        }
        Grid playGrid = Grid(desiredWidth, desiredHeight);
        std::cout << "Would you like to go first?" << std:: endl;
        while (!orderSelected) {
            std::cin >> playerInput;
            if (playerInput == "yes" || playerInput == "Yes" || playerInput == "y" || playerInput == "Y") {
                orderSelected = true;
            }
            if (playerInput == "no" || playerInput == "No" || playerInput == "n" || playerInput == "N") {
                orderSelected = true;
                playGrid.computerAction();
                victor = playGrid.checkForVictory();
            }
            if (!orderSelected)
            {
                std::cout << "Invalid response. Just say yes or no." << std::endl;
            }
        }
        while (victor == 0)
        {
            try {
                std::cout << "Make your move. Type 2 numbers, seperated by a comma." << std::endl;
                playGrid.printGrid();
                std::cin >> playerInput;
                if (!playGrid.playerAction(playerInput))
                {
                    throw std::invalid_argument("");
                }
                victor = playGrid.checkForVictory();
                if (victor == 0) {
                    playGrid.computerAction();
                    victor = playGrid.checkForVictory();
                }
            }
            catch(...)
            {
                std::tuple<int,int> exampleTuple = playGrid.exampleLocation();
                int exampleX = std::get<0>(exampleTuple);
                int exampleY = std::get<1>(exampleTuple);
                std::cout << "Invalid response. Choose an unoccupied spot, such as \"" << exampleX << ", " << exampleY << "\".";
            }
        }
        playGrid.printGrid();
        if (victor == 1)
        {
            std::cout << "You win!" << std::endl;
        }
        if (victor == 2)
        {
            std::cout << "You Lose!" << std::endl;
        }
        if (victor == 3)
        {
            std::cout << "It is a draw!" << std::endl;
        }
        std::cout << "Would you like to quit?" << std::endl;
        std::cin >> playerInput;
        if (playerInput == "yes" || playerInput == "Yes" || playerInput == "y" || playerInput == "Y") {
            std::cout << "Thank you for playing!";
            return 0;
        }
        std::cout<< "That wasn't a yes, so let's go again!" << std::endl;
        widthSelected = false;
        heightSelected = false;
        orderSelected = false;
        victor = 0;
    }
}
