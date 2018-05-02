enum menuSelection
{
	SEARCHANDDESTROY = 0,
	SURVIVAL,
	HIGHSCORES,
	EXIT
};

int maxChoices = 4;
int currentlySelectedMenuItem = 0;
char currentlyPressedKey = 'n';
bool showHighScores = false;
bool escapeHasBeenPressed = false;
bool spaceHasBeenPressed = false;