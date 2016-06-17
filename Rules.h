void printRules (void) {
	system("cls");
	printf("\tPlay is counterclockwise. The seeds are distributed one by one in the\n pits and the players own kalah, but not into the opponent's store.\n\n");
	printf("\tNew game cannot be started from the first pit.\n\n");
	printf("\tIf the last seed is dropped into an opponent's pit or a non-empty pit\n of the player, the move ends without anything being captured.\n\n");
	printf("\tIf the last seed falls into the player's kalah, he must move again.\n\n");
	printf("\tIf the last seed is put into an empty pit owned by the player,\n he captures all contents of the opposite pit together with the capturing\n piece and puts them in his kalah. If the opposite pit is empty,\n nothing is captured. A capture ends the move.\n\n");
	printf("\tThe game ends when one player no longer has any seeds in any\n of his holes. The remaining pieces are captured by his adversary. The player\n who has captured most pieces is declared the winner.");
	printf("\n\n\t\t\tPress any key to continue");
	getch();
	system("cls");
}


