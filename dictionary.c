/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "dictionary.h"

/**
 * Returns true if word is in dictionary else false.
 */
bool FreeTrie();
 
typedef struct node
{
    bool is_word;
    struct node *children[27];
}
node;
    
node *root; 
node *cursor;
int letter_index;
int count;
    
bool check(const char *word)
{
    cursor = root;
    for(int i = 0; i < strlen(word); i++)
    {
        if(isalpha(word[i]))
        {
            letter_index = tolower(word[i]) - 'a';
        }
        else if (word[i] == '\'')
        {
            letter_index = 26;
        }
        
        if(cursor->children[letter_index] == NULL)
        {
            return false;
        }
        else
        {
            cursor = cursor->children[letter_index];
        }
        
        if((i + 1) == strlen(word) && cursor->is_word == true)
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
    //open dictionary
    FILE *file = fopen(dictionary, "r");
    if(file == NULL)
    {
        return false;
    }
    
    //load dictionary's words
    char word[LENGTH + 1];
    
    //prepare the root of Trie
    root = calloc(1, sizeof(node));;
    if(root == NULL)
    {
        return false;
    }

    while(fscanf(file, "%s", word) != EOF)
    {
        cursor = root;
        for(int i = 0; i < strlen(word); i++)
        {
            // check that the char is not a '\'' and convert to ASCII value for letter_index
            if(isalpha(word[i]))
            {
                letter_index = tolower(word[i]) - 'a';
            }
            else if (word[i] == '\'')
            {
                letter_index = 26;
            }
            
            if (cursor->children[letter_index] == NULL)
            {
                node *child = calloc(1, sizeof(node));
                if(child == NULL)
                {
                    return false;
                }
                cursor->children[letter_index] = child;
                cursor = child;
            }
            else
            {
                cursor = cursor->children[letter_index];
            }
                
            //change bool value if at end of word
            if((i + 1) == strlen(word))
            {
                cursor->is_word = true;
                count++;
            }
        }
    }
    fclose(file);
    
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    if (load() == false) 
    {
        return 0;
    }
    
    return count;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    return FreeTrie(root);
}

bool FreeTrie(node *cursor)
{
    for(int i = 0; i < 27; i++)
    {
        if(cursor->children[i] != NULL)
        {
            FreeTrie(cursor->children[i]);
        }  
    }
    free(cursor);
    
    return true;
}