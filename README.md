# Kalah
Kalah console game in C. Min-Max algorithm with Alpha-Beta pruning is applied in the program. Course project.

Environment. The program is completed in C programming language using Dev C++  integrated development environment (IDE).
The main reasons for choosing the environment are:
-	simplicity of Dev C++ usage for creating complete console projects;
-	it is bundled with, and uses, the MinGW or TDM-GCC 64bit port of the GCC as its compiler which fully satisfies the goal of the project.

Project structure. The project consists of two files:
-	main.c, which contains executable program code;
-	Rules.h, with the rules of the Kalah game for printing on user’s screen when requested.

The game starts in main function. It contains an endless loop and calls responsible for game settings and gameplay functions.

Once the game is launched, it calls setGame function, where user chooses one of the available options to start a game: Player VS Player or Player VS Computer. Besides these, there are options to read the Kalah Game rules or quit app. If user chooses to play VS Computer, he has to choose level of difficulty, which is EASY (e), MEDIUM (m) and HARD (h). Difficulty level is set to maxDepth variable (the only global variable in the app).

setGame function returns 1 (Player VS Player) or 2 (Player VS Computer) value and assigns it to the gameType variable. Once new game is set, main function calls printDesk, which prints game board and the number of stones in console.

Before user A or B makes a turn, isEmptySide function checks whether terminal position (when either side has no stones at any of 1 to 6 houses) has been reached. If so, function calculates and compares the number of stones in Kalahs of Player A and Player B. Depending on the comparison, it prints the message with game results and proposes to start new game.

If a position is not terminal, getUserTurn function gets called, which is responsible for user move based on the value of side variable. If side = B and the opponent is a Computer, then the getUserTurn function calls bestTurn function, which is responsible for calculating the best turn for a computer based on the selected depth (opponent level) using Alpha-Beta pruning (alphaBeta function).
The alphaBeta function calls itself in a recursive way maxDepth-1 times. On the last recursive call of the function (when the search tree reaches the last leaf in a branch),  alphaBeta calls evaluate function which returns the difference in players’ Kalahs from the perspective of a user who is making a turn. The value goes up in a branch of Mini-Max algorithm and gets compared with the values on the previous levels using Alpha-Beta pruning principal.

In case it is a user turn (Player A or Player B if it is a Player VS Player game) to make a move, getUserTurn function validates entered from the keyboard value and returns error in case the move is invalid.

The result of the getUserTurn function (a value from 1 to 6) is returned to the main function and gets transmitted to the makeTurn function which drops stones according to the move. If the last stone in a move is dropped into Player’s (who made a move) Kalah, function returns 1 value and user repeats his move by returning to the start of the cycle without changing the value of side variable.
Otherwise, program starts new iteration of the cycle changing value of the variable from A to B or B to A.

The cycle in the main function ends once user chooses to quit app by pressing ESC button twice or not confirming start of a new game after a finished game.
