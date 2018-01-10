/**
 * Implements a dictionary's functionality.
 */

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "dictionary.h"

typedef struct node 
{
    bool is_word;
    struct node *children[27];
}
node;

node *root;
int counter = 0;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    node *cursor = root;
    int index = 0;
    
    for (int i = 0; i < strlen(word); i++) 
    {
        if (isalpha(word[i])) 
        {
            index = tolower(word[i]) - 'a';
        }
        else if(word[i] == '\'')
        {
            index = 26;
        }
        
        if (cursor->children[index] == NULL) {
            return false;
        }
        else 
        {
            cursor = cursor->children[index];
        }
        
        if (i == strlen(word) - 1 && cursor->is_word == true) 
        {
            return true;
        }
    }
    
    return false;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    // trie data structure implementation
    char letter = 'a';
    int index = 0;
    node *cursor;
    
    // open dictionary
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        return false;
    }
    
    // allocate memory for the root node
    root = calloc(1, sizeof(node));
    if (root == NULL) {
        return false;
    }
    
    cursor = root;
    while (letter != EOF) 
    {
        letter = fgetc(dict);
        
        if (letter == '\n') 
        {
            cursor->is_word = true;
            cursor = root;
            counter++;
            continue;
        }
        
        if (isalpha(letter)) {
            index = tolower(letter) - 'a';
        }
        else if (letter == '\'')
        {
            index = 26;
        }
        
        if (cursor->children[index] == NULL) {
            node *child = calloc(1, sizeof(node));
            if (child == NULL) 
            {
                return false;
            }
            cursor->children[index] = child;
            cursor = child;
        }
        else
        {
            cursor = cursor->children[index];
        }
    }
    
    fclose(dict);
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return counter;
}

bool freeTrie(node *cursor) 
{
    for(int i = 0; i < 27; i++)
    {
        if(cursor->children[i] != NULL) 
        {
            freeTrie(cursor->children[i]);
        }
    }
    free(cursor);
    
    return true;
}

/**
 * Unloads dictionary from memory. Returns true if successful
 */
bool unload(void)
{
    return freeTrie(root);
}

