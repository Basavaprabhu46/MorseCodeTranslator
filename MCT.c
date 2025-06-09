#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#define MAX_LEN 1000

const char *morse[] = {
    ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", 
    ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", 
    "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.."
};

char* englishToMorse(const char* input) {
    char *result = malloc(MAX_LEN * 5);
    if (result == NULL) {
        printf("Memory error!\n");
        exit(1);
    }

    int i, j, pos = 0;
    for (i = 0; input[i] != '\0'; i++) {
        char ch = input[i];
        if (isalpha(ch)) {
            int index = toupper(ch) - 'A';
            const char* code = morse[index];
            for (j = 0; code[j] != '\0'; j++) {
                result[pos++] = code[j];
            }
            result[pos++] = ' ';
        } else if (ch == ' ') {
            result[pos++] = '/';
            result[pos++] = ' ';
        }
    }
    result[pos] = '\0';
    return result;
}

char* morseToEnglish(const char* input) {
    char *result = malloc(MAX_LEN);
    if (result == NULL) {
        printf("Memory error!\n");
        exit(1);
    }

    result[0] = '\0';

    char buffer[MAX_LEN]; 
    strncpy(buffer, input, MAX_LEN - 1);
    buffer[MAX_LEN - 1] = '\0';

    char *token = strtok(buffer, " ");
    while (token != NULL) {
        if (strcmp(token, "/") == 0) {
            strcat(result, " ");
        } else {
            for (int i = 0; i < 26; i++) {
                if (strcmp(token, morse[i]) == 0) {
                    char ch[2];
                    ch[0] = 'A' + i;
                    ch[1] = '\0';
                    strcat(result, ch);
                    break;
                }
            }
        }
        token = strtok(NULL, " ");
    }

    return result;
}

char* readFromFile(const char* filename) {
    static char buffer[MAX_LEN];
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        printf("Can't open file: %s\n", filename);
        return NULL;
    }
    
    fread(buffer, sizeof(char), MAX_LEN - 1, file);
    buffer[MAX_LEN - 1] = '\0';
    fclose(file);
    return buffer;
}

int writeToFile(const char* filename, const char* content) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Can't write to file: %s\n", filename);
        return 0;
    }

    fprintf(file, "%s", content);
    fclose(file);
    return 1;
}
void convertMorseFileToEnglish() {
    char filename[100];
    printf("Enter filename to read Morse code: ");
    scanf("%s", filename);
    getchar();

    char *fileContent = readFromFile(filename);
    if (fileContent != NULL) {
        char *engText = morseToEnglish(fileContent);
        printf("From file (English): %s\n", engText);
        free(engText);
    }
}
void playMorseSound(const char* morseCode) {
    int i;
    for (i = 0; morseCode[i] != '\0'; i++) {
        if (morseCode[i] == '.') {
            system("afplay dot.wav &");
            usleep(300000);
        } else if (morseCode[i] == '-') {
            system("afplay dash.wav &");
            usleep(400000);
        } else if (morseCode[i] == ' ') {
            usleep(400000);
        } else if (morseCode[i] == '/') {
            usleep(500000);
        }
    }
}



void showMenu() {
    printf("\n\n----- Morse Code Translator -----\n");
    printf("1. Convert English to Morse\n");
    printf("2. Convert Morse to English\n");
    printf("3. Convert English File to Morse\n");
    printf("4. Convert Morse File to English\n");
    printf("5. Play Morse Audio\n");
    printf("6. Exit\n");
    printf("---------------------------------\n");
}

void handleConversion() {
    int choice;
    char input[MAX_LEN];
    char filename[100];

    while (1) {
        showMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        if (choice == 1) {
            printf("Enter English text: ");
            fgets(input, MAX_LEN, stdin);
            input[strcspn(input, "\n")] = '\0';

            char *morseResult = englishToMorse(input);
            printf("Morse Code: %s\n", morseResult);
            free(morseResult);
        } else if (choice == 2) {
            printf("Enter Morse code: ");
            fgets(input, MAX_LEN, stdin);
            input[strcspn(input, "\n")] = '\0';

            char *eng = morseToEnglish(input);
            printf("English Text: %s\n", eng);
            free(eng);
        } else if (choice == 3) {
            printf("Enter filename to read: ");
            scanf("%s", filename);
            getchar();

            char *fileContent = readFromFile(filename);
            if (fileContent != NULL) {
                char *morseFromFile = englishToMorse(fileContent);
                printf("From file: %s\n", morseFromFile);
                free(morseFromFile);
            }
        } else if (choice == 4) {
            convertMorseFileToEnglish();
        } else if (choice == 5) {
            printf("Enter Morse to play: ");
            fgets(input, MAX_LEN, stdin);
            input[strcspn(input, "\n")] = '\0';
            playMorseSound(input);
        } else if (choice == 6) {
            printf("Exiting...\n");
            break;
        } else {
            printf("Invalid choice. Try again.\n");
        }
    }
}

int main() {
    printf("Welcome to Morse Code Project!\n");
    handleConversion();
    return 0;
}