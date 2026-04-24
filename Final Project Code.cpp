/*
Name: Manuel Dixon
Course: CSC242 Intro to Programming Concepts
Assignment: Assignment 5
Description: The first option runs a spell checker for P8.2 by loading a dictionary file 
and checking an input file for words that are not in the dictionary. The second option runs a 
monoalphabetic cipher for P8.7 that can encrypt or decrypt a file using a keyword.
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cctype>
using namespace std;

// Common function
string cleanWord(string word);

// Spell checker functions
void runSpellChecker();
bool wordFound(const vector<string>& dictionaryWords, string wordToCheck);

// Cipher functions
void runCipher();
string removeDuplicateLetters(string keyword);
string createCipherAlphabet(string keyword);
char encryptLetter(char letter, string cipherAlphabet);
char decryptLetter(char letter, string cipherAlphabet);

int main()
{
    int choice;

    cout << "Assignment 5 Program" << endl;
    cout << "1. Spell Checker" << endl;
    cout << "2. Cipher" << endl;
    cout << "Enter your choice: ";
    cin >> choice;

    if (choice == 1)
    {
        runSpellChecker();
    }
    else if (choice == 2)
    {
        runCipher();
    }
    else
    {
        cout << "Invalid choice." << endl;
    }

    return 0;
}

// Removes punctuation and makes letters lowercase
string cleanWord(string word)
{
    string cleaned = "";

    for (int i = 0; i < word.length(); i++)
    {
        if (isalpha(word[i]))
        {
            cleaned += tolower(word[i]);
        }
    }

    return cleaned;
}

// SPELL CHECKER

void runSpellChecker()
{
    vector<string> dictionaryWords;
    string word;

    // These files are in the main project folder.
    // Visual Studio should have Working Directory set to $(ProjectDir).
    ifstream dictionaryFile("dictionary.txt");

    if (!dictionaryFile)
    {
        cout << "Error: could not open dictionary.txt." << endl;
        cout << "Make sure dictionary.txt is in the project folder." << endl;
        cout << "Also make sure Project Properties > Debugging > Working Directory is set to $(ProjectDir)." << endl;
        return;
    }

    // Loads each dictionary word into the vector
    while (dictionaryFile >> word)
    {
        string cleanedWord = cleanWord(word);

        if (cleanedWord != "")
        {
            dictionaryWords.push_back(cleanedWord);
        }
    }

    dictionaryFile.close();

    cout << "Dictionary words loaded: " << dictionaryWords.size() << endl;

    ifstream inputFile("input.txt");

    if (!inputFile)
    {
        cout << "Error: could not open input.txt." << endl;
        cout << "Make sure input.txt is in the project folder." << endl;
        cout << "Also make sure Project Properties > Debugging > Working Directory is set to $(ProjectDir)." << endl;
        return;
    }

    cout << endl;
    cout << "Words not found in dictionary:" << endl;

    // Checks each word from the input file against the dictionary
    while (inputFile >> word)
    {
        string cleanedWord = cleanWord(word);

        if (cleanedWord != "" && !wordFound(dictionaryWords, cleanedWord))
        {
            cout << cleanedWord << endl;
        }
    }

    inputFile.close();
}

// Searches for the word inside the dictionary vector
bool wordFound(const vector<string>& dictionaryWords, string wordToCheck)
{
    for (int i = 0; i < dictionaryWords.size(); i++)
    {
        if (dictionaryWords[i] == wordToCheck)
        {
            return true;
        }
    }

    return false;
}

// CIPHER

void runCipher()
{
    char mode;
    string keyword;
    string inputFileName;
    string outputFileName;

    cout << "Enter E to encrypt or D to decrypt: ";
    cin >> mode;

    cout << "Enter keyword: ";
    cin >> keyword;

    if (keyword == "")
    {
        cout << "Error: keyword is required." << endl;
        return;
    }

    cout << "Enter input file name: ";
    cin >> inputFileName;

    cout << "Enter output file name: ";
    cin >> outputFileName;

    ifstream inputFile(inputFileName);
    ofstream outputFile(outputFileName);

    if (!inputFile)
    {
        cout << "Error: could not open input file." << endl;
        cout << "Make sure the file is in the project folder." << endl;
        return;
    }

    if (!outputFile)
    {
        cout << "Error: could not open output file." << endl;
        return;
    }

    string cipherAlphabet = createCipherAlphabet(keyword);
    char character;

    while (inputFile.get(character))
    {
        if (mode == 'E' || mode == 'e')
        {
            outputFile << encryptLetter(character, cipherAlphabet);
        }
        else if (mode == 'D' || mode == 'd')
        {
            outputFile << decryptLetter(character, cipherAlphabet);
        }
        else
        {
            cout << "Invalid mode." << endl;
            inputFile.close();
            outputFile.close();
            return;
        }
    }

    inputFile.close();
    outputFile.close();

    cout << "Cipher process complete." << endl;
    cout << "Output saved to " << outputFileName << endl;
}

// Removes repeated letters from the keyword
string removeDuplicateLetters(string keyword)
{
    string result = "";

    for (int i = 0; i < keyword.length(); i++)
    {
        char currentLetter = toupper(keyword[i]);
        bool alreadyUsed = false;

        if (isalpha(currentLetter))
        {
            for (int j = 0; j < result.length(); j++)
            {
                if (result[j] == currentLetter)
                {
                    alreadyUsed = true;
                }
            }

            if (!alreadyUsed)
            {
                result += currentLetter;
            }
        }
    }

    return result;
}

// Creates the cipher alphabet from the keyword and the remaining alphabet in reverse order
string createCipherAlphabet(string keyword)
{
    string cipherAlphabet = removeDuplicateLetters(keyword);
    string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    for (int i = alphabet.length() - 1; i >= 0; i--)
    {
        char currentLetter = alphabet[i];
        bool alreadyUsed = false;

        for (int j = 0; j < cipherAlphabet.length(); j++)
        {
            if (cipherAlphabet[j] == currentLetter)
            {
                alreadyUsed = true;
            }
        }

        if (!alreadyUsed)
        {
            cipherAlphabet += currentLetter;
        }
    }

    return cipherAlphabet;
}

// Encrypts one character using the cipher alphabet
char encryptLetter(char letter, string cipherAlphabet)
{
    string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    bool lowerCase = islower(letter);
    char upperLetter = toupper(letter);

    for (int i = 0; i < alphabet.length(); i++)
    {
        if (alphabet[i] == upperLetter)
        {
            if (lowerCase)
            {
                return tolower(cipherAlphabet[i]);
            }
            else
            {
                return cipherAlphabet[i];
            }
        }
    }

    return letter;
}

// Decrypts one character using the cipher alphabet
char decryptLetter(char letter, string cipherAlphabet)
{
    string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    bool lowerCase = islower(letter);
    char upperLetter = toupper(letter);

    for (int i = 0; i < cipherAlphabet.length(); i++)
    {
        if (cipherAlphabet[i] == upperLetter)
        {
            if (lowerCase)
            {
                return tolower(alphabet[i]);
            }
            else
            {
                return alphabet[i];
            }
        }
    }

    return letter;
}
