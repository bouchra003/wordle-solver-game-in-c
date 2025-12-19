#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <ctype.h>

#define MAX_TRIES 6
#define MAX_WORDS 5758

// ANSI color codes with black text for visibility
#define GREEN "\x1b[30;42m"
#define YELLOW "\x1b[30;43m"
#define GRAY "\x1b[30;47m"
#define RESET "\x1b[0m"

// ==================== MANUAL TARGET WORD INPUT (HIDDEN) ====================
void user_choose_word(char *target) {
    char temp[100];  // temporary buffer for input
    int length;
    char ch;

    while (1) {
        int index = 0;
        printf("Enter a 5-letter target word (hidden):\n");

        // read hidden input until ENTER
        while (1) {
            ch = _getch();
            if (ch == '\r') { // ENTER pressed
                break;
            }

            // accept only letters
            if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) {
                if (index >= 5) {  // typed more than 5 letters
                    // clear the rest of the line until ENTER
                    while ((ch = _getch()) != '\r');
                    index = -1;  // mark invalid
                    break;
                }
                temp[index++] = (char)tolower((unsigned char)ch);
            }
        }

        if (index == -1 || index != 5) {
            printf("Invalid word it must be exactly 5 letters.\n\n");
            continue;  // ask again
        }

        temp[5] = '\0';
        strcpy(target, temp);
        printf("\nWord saved!\n");
        break;
    }
}


// ==================== RANDOM / MANUAL TARGET SELECTION ====================
void get_target_word(char *target, char **wordslist, int wordscount) {
    int choice;
    printf("Choose mode:\n1. Random word\n2. Enter word manually\nYour choice: ");
    scanf("%d", &choice);
    while (getchar() != '\n'); // clear buffer

    if (choice == 1) {
        srand((unsigned int)time(NULL));
        strcpy(target, wordslist[rand() % wordscount]);
        for (int i = 0; i < 5; i++)
            target[i] = (char)tolower((unsigned char)target[i]);
    }
    else if (choice == 2) {
        user_choose_word(target);
    }
    else {
        printf("Invalid choice! Try again.\n");
        get_target_word(target, wordslist, wordscount);
    }
}

// ==================== VALIDATE GUESS (EXACTLY 5 LETTERS) ====================
void get_valid_guess(char *guess) {
    char input[100];

    while (1) {
        scanf("%99s", input);

        if (strlen(input) != 5) {
            printf("Invalid word it must be exactly 5 letters. Try again: ");
            continue;
        }

        int valid = 1;
        for (int i = 0; i < 5; i++) {
            if (!isalpha((unsigned char)input[i])) {
                valid = 0;
                break;
            }
            input[i] = (char)tolower((unsigned char)input[i]);
        }

        if (!valid) {
            printf("Invalid word letters only. Try again: ");
            continue;
        }

        strcpy(guess, input);
        return;
    }
}

// ==================== WORDLE FEEDBACK (GREEN/YELLOW/GRAY) ====================
void print_feedback_colored(const char *guess, const char *target) {
    int used[5] = {0};
    char colors[5];

    // First pass: GREEN
    for (int i = 0; i < 5; i++) {
        if (guess[i] == target[i]) {
            colors[i] = 'G';
            used[i] = 1;
        } else {
            colors[i] = 'X';
        }
    }

    // Second pass: YELLOW / GRAY
    for (int i = 0; i < 5; i++) {
        if (colors[i] == 'G') continue;

        int found = 0;
        for (int j = 0; j < 5; j++) {
            if (!used[j] && guess[i] == target[j]) {
                found = 1;
                used[j] = 1;
                break;
            }
        }

        colors[i] = found ? 'Y' : 'B';
    }

    // Print colored output
    for (int i = 0; i < 5; i++) {
        if (colors[i] == 'G')
            printf(GREEN " %c " RESET, guess[i]);
        else if (colors[i] == 'Y')
            printf(YELLOW " %c " RESET, guess[i]);
        else
            printf(GRAY " %c " RESET, guess[i]);
    }
    printf("\n");
}

// ==================== MAIN GAME ====================
int main() {
    printf("welcome to wordle solver game\n");
    char **wordslist = calloc(MAX_WORDS, sizeof(char *));
    if (!wordslist) { printf("Memory error\n"); return 1; }

    int wordscount = 0;
    char buffer[64];

    FILE *wordsfile = fopen("C:\\Users\\Bouchra\\Downloads\\words.txt", "r");
    if (!wordsfile) {
        printf("Error opening file!\n");
        free(wordslist);
        return 1;
    }

    while (fscanf(wordsfile, "%63s", buffer) != EOF && wordscount < MAX_WORDS) {
        if (strlen(buffer) >= 5) {
            char tmp[6];
            for (int i = 0; i < 5; i++)
                tmp[i] = (char)tolower((unsigned char)buffer[i]);
            tmp[5] = '\0';

            wordslist[wordscount] = malloc(6);
            if (!wordslist[wordscount]) break;

            strcpy(wordslist[wordscount], tmp);
            wordscount++;
        }
    }
    fclose(wordsfile);

    if (wordscount == 0) {
        printf("No words loaded.\n");
        free(wordslist);
        return 1;
    }

    char target[6];
    get_target_word(target, wordslist, wordscount);

    char guesses[MAX_TRIES][6];
    int tries = 0;

    while (tries < MAX_TRIES) {
        printf("Attempt %d/%d: Enter your guess: ", tries + 1, MAX_TRIES);
        get_valid_guess(guesses[tries]);

        printf("\nYour guesse is so far:\n");
        for (int i = 0; i <= tries; i++)
            print_feedback_colored(guesses[i], target);

        printf("\n");

        if (strcmp(guesses[tries], target) == 0) {
            printf(" Congratulations! You guessed the word in %d tries.\n", tries + 1);
            break;
        }

        tries++;
    }

    if (tries == MAX_TRIES && strcmp(guesses[tries - 1], target) != 0) {
        printf(" Sorry! You've used all attempts. The word was: %s\n", target);
    }

    for (int i = 0; i < wordscount; i++)
        free(wordslist[i]);
    free(wordslist);

    return 0;
}
