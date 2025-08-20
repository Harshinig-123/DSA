#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

int main() {
    char s[1000];
    printf("Enter a string: ");
    scanf("%[^\n]", s);
    int n = strlen(s);
    char new[1000];
    int j = 0;
    for (int i = 0; i < n; i++) {
        if (isalnum(s[i])) {
            new[j++] = tolower(s[i]);
        }
    }
    new[j] = '\0';
    bool isPalindrome = true;
    for (int i = 0; i < j / 2; i++) {
        if (new[i] != new[j - 1 - i]) {
            isPalindrome = false;
            break;
        }
    }
    if (isPalindrome)
        printf("Palindrome\n");
    else
        printf("Not Palindrome\n");

    return 0;
}
