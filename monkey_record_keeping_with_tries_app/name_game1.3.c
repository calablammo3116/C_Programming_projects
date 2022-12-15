/*  Caleb Gibson
    COP3502
    Assignment 6 -- Name Game
    Due Monday November 21, 2022
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct TrieNode{
    struct TrieNode * children[26];
    int subtreeSum, curNodeValue;
} TrieNode;

TrieNode * createNode()
{
    TrieNode * node = (TrieNode *)malloc(sizeof(TrieNode));
    node->subtreeSum = 0;
    node->curNodeValue = 0;
    for(int i = 0; i < 26; i++){
        node->children[i] = NULL;
    }
    return node;
}

TrieNode* addName(TrieNode * root, int value, char * name)
{
    // base case; have reached an empty node; note that this will NOT end the function, as we have potentially NOT reached the name's end yet
    if (root == NULL){
       root = createNode();
//       printf("Creating new node...\n");
    }
    // check to see if the name is at its last non-null terminator '\0' character
    // if it is, then return
    if (name != NULL && name[0] == '\0'){
        root->subtreeSum += value;
        root->curNodeValue = value;
//        printf("subtreeSum = %d ------- curNodeValue = %d\n", root->subtreeSum, root->curNodeValue);
        return root;
    } else{ // otherwise, update the subtree sum and go to the next root
        // check for errors
        if(name[0] < 'A' || name[0] > 'Z'){
            return NULL;
        }
//        printf("%c subtreeSum = %d\n", name[0], root->subtreeSum);
        root->children[name[0] - 'A'] = addName(root->children[name[0] - 'A'], value, name + 1);
        root->subtreeSum += value;
    }
    return root;
}

TrieNode* reduceSums(TrieNode * root, int value, char * oldName)
{
    // base case; have reached an empty node
    if (root == NULL || oldName == NULL){
        return NULL;
    }
    // otherwise, update the value and go to the next node
    // check to make sure you are not at the end of the word
    if(oldName != NULL && oldName[0] == '\0'){
//        printf("Reducing subtree sum %d and curNodeValue %d at %c by %d\n...", root->subtreeSum, root->curNodeValue, oldName[0], value);
        root->subtreeSum -= value;
        root->curNodeValue -= value;
//        printf("New subtree sum at %c: %d ------- New curNodeValue at %c: %d", oldName[0], root->subtreeSum, oldName[0], root->curNodeValue);
    } else{
        // check for errors
        if(oldName[0] < 'A' || oldName[0] > 'Z'){
            return NULL;
        }
        root->children[oldName[0] - 'A'] = reduceSums(root->children[oldName[0] - 'A'], value, oldName + 1);
        root->subtreeSum -= value;
    }
    return root;
}

int newNameMangoRetrieval(TrieNode * root, char * name)
{
    // base case; if the root is NULL, then we have an empty trie, so there are 0 mangoes, so return 0
    if (root == NULL || name == NULL){
        return 0;
    }
    // non-base case: if we are at the  null terminator, then return the "curNodeValue" stored there. Otherwise, continue navigating down the trie.
    if (name != NULL && name[0] == '\0'){
//        printf("curNodeValue = %d\n", root->curNodeValue);
        return root->curNodeValue;
    } else{
        return newNameMangoRetrieval(root->children[name[0] - 'A'], name + 1);
    }
}

int prefixMangoRetrieval(TrieNode * root, char * pre)
{
    // make sure pre is not NULL to be safe
    if (root == NULL || pre == NULL){
        return 0;
    }
    // base case; have reached last character of the prefix string, return subtree sum at the current trie node
    if (pre != NULL && pre[0] == '\0'){
//        printf("prefix subtree sum at FINAL prefix character: %d\n", root->subtreeSum);
        return root->subtreeSum;
    }
    // non-base case; continue navigating down the trie until at null terminator, '\0'
//    printf("prefix subtree sum at prefix character %c: %d\n", pre[0], root->subtreeSum);
    return prefixMangoRetrieval(root->children[pre[0] - 'A'], pre + 1);
}

void cleanTrie(TrieNode *root) {
    if (root == NULL)
    return;
    for (int i = 0; i < 26; i++){
    cleanTrie(root->children[i]);
    }
    free(root);
}

int main()
{
    int command, numOrangutans, numEvents, numMangoes;
    char * oldName = (char *)malloc(sizeof(char)*21); // 21 instd of 20 because null terminator '\0'
    char * curName = (char *)malloc(sizeof(char)*21);
    // create the root for the orangutans' names trie
    TrieNode* orangutansNames = createNode();

    // read in the number of orangutans followed by the number of events
    scanf("%d%d", &numOrangutans, &numEvents);

    // read in the rest of the input and handle commands
    for(int i = 0; i < numEvents; i++){
        // read in the first part of the input, representing the campaign
        scanf("%d", &command);
        // case 1 -- a feeding, 1 n a
        if (command == 1){
            // read in the name and number after it
            scanf("%s%d", curName, &numMangoes);
            // update the value of the subtree sums and curNodeValues for each node in the subtree corresponding with the orangutan's name
            orangutansNames = addName(orangutansNames, numMangoes, curName);
        }
        // case 2 -- a re-naming, 2 o n
        else if (command == 2){
            // read in the two names
            scanf("%s%s", oldName, curName);
            // update the subtree sums in both orangutans' names' parts of the trie
            // need to store the number of mangoes an orangutan has eaten and easily access it O(1) somehow
            // one way could be an array, based on key-value IDs, but then how would you identify which name goes with which ID?
            // that would require looping through all the characters in each name associated with each ID, making for an O(N^2), technically
            // O(NM) because there are N names and if there are M characters in each name, process.
            // The only way to effectively do this is with a data retrieval going to the end of the trie and fetching the "nodeTotal" (or
            // "curNodeValue") value and then using that as the reference for the number of mangoes to add or subtract from the subtree sum in
            // each trie node in the orangutan's new and old names' subtrees.
            numMangoes = newNameMangoRetrieval(orangutansNames, oldName);
            orangutansNames = reduceSums(orangutansNames, numMangoes, oldName);
            orangutansNames = addName(orangutansNames, numMangoes, curName);
        }
        // case 3 -- a data retrieval based on prefix 'p,' 3 p
        else{
            // read in the prefix
            scanf("%s", curName);
            // store the number of mangoes it finds
            numMangoes = prefixMangoRetrieval(orangutansNames, curName);
            // output the number of mangoes for orangutans with that prefix
            printf("%d\n", numMangoes);
        }
    }

    // clean all dynamically allocated memory
    free(oldName);
    free(curName);
    cleanTrie(orangutansNames);

    return 0;
}
