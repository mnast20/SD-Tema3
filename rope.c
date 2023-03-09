// Copyright 2021 Calin Coman & Nastase Maria
// Grupa 311CA
#include "./rope.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define EMPTY ""

char* strdup(const char*);

/**
 * Error handling when allocating memory
 * @param ptr pointer to be checked
 */
void check_malloc(const void *ptr)
{
    if (ptr == NULL) {
        fprintf(stderr, "malloc failed\n");
        exit(-1);
    }
}

/**
 * Create rope node
 * @param str string that will be attributed to node
 */
RopeNode* makeRopeNode(const char* str) {
    // Allocate memory for the node
    RopeNode *node = (RopeNode *)malloc(sizeof(RopeNode));
    check_malloc(node);

    // Attribute string to node
    node->str = str;
    // Node's weight becomes length of string
    int len = strlen(str);
    node->weight = len;
    // Initially, the node has no children
    node->left = NULL;
    node->right = NULL;

    return node;
}

/**
 * Create rope tree whose root is the node sent as parameter
 * @param root the node that will become the root of tree
 */
RopeTree* makeRopeTree(RopeNode* root) {
    // Allocate memory for the rope tree
    RopeTree *rope_tree = (RopeTree *)malloc(sizeof(RopeTree));
    check_malloc(rope_tree);

    // Attribute the root of the tree
    rope_tree->root = root;

    return rope_tree;
}

void printRopeNode(RopeNode* rn) {
    if (!rn)
        return;

    if (!(rn->left) && !(rn->right)) {
        printf("%s", rn->str);
        return;
    }

    printRopeNode(rn->left);
    printRopeNode(rn->right);
}

void printRopeTree(RopeTree* rt) {
    if (rt && rt->root) {
        printRopeNode(rt->root);
        printf("%s", "\n");
    }
}

void debugRopeNode(RopeNode* rn, int indent) {
    if (!rn)
        return;

    for (int i = 0; i < indent; ++i)
        printf("%s", " ");

    if (!strcmp(rn->str, EMPTY))
        printf("# %d\n", rn->weight);
    else
        printf("%s %d\n", rn->str, rn->weight);

    debugRopeNode(rn->left, indent + 2);
    debugRopeNode(rn->right, indent + 2);
}

/**
 * Recursively free all the memory allocated to node and all its descendants
 * @param node root of the subtree that will be freed
 */
void free_rope_node(RopeNode *node)
{
    if (node == NULL)
        return;

    // Free left and right subtree of node
    free_rope_node(node->left);
    free_rope_node(node->right);

    // Free node and its data
    free((void *)node->str);
    free(node);
}

/**
 * Free all the memory allocated to tree
 * @param tree the tree that will be freed
 */
void free_rope_tree(RopeTree *rope_tree)
{
    if (rope_tree == NULL)
        return;

    // Free every node of the tree
    free_rope_node(rope_tree->root);
    // Free the tree itself
    free(rope_tree);
}

/**
 * Return the sum of the leaf weights from the tree with node as root
 * @param node node whose total weight will be returned
 */
int getTotalWeight(RopeNode* node) {
    if (!node)
        return 0;

    return node->weight + getTotalWeight(node->right);
}

/**
 * Concatenate two trees
 * @param rt1 first tree, that will be on the left side of concatenation
 * @param rt2 second tree, that will be on the right side of concatenation
 */
RopeTree* concat(RopeTree* rt1, RopeTree* rt2) {
    // Create new tree with new root
    RopeNode *new_root = makeRopeNode(strdup(EMPTY));

    // attach the root of each tree to the new root
    new_root->left = rt1->root;
    new_root->right = rt2->root;

    // Readjust the weight of the new root
    new_root->weight = getTotalWeight(new_root->left);

    return makeRopeTree(new_root);
}

/**
 * Check is node is a leaf
 * @param node node to be checked
 */
int is_leaf(RopeNode *node)
{
    // If a node doesn't have children, it is a leaf
    if (node->left == NULL && node->right == NULL)
        return 1;
    return 0;
}

/**
 * Returns the leaf which contains the character at given index in the rope
 * tree string
 * @param node current node
 * @param idx index in the subtree string
 * @param str_idx index of the character in node's string
 */
RopeNode *findNode(RopeNode *node, int idx, int *str_idx)
{
    if (node == NULL) {
        // Node not found
        printf("The %d position does not exist in the Rope Tree\n", idx);
        return NULL;
    }
    // Update the index value in node's string
    *str_idx = idx;

    if (is_leaf(node)) {
        // If leaf is found, we return it
        return node;
    }

    if (idx < node->weight) {
        // If index is smaller than the weight, it is located in the left
        // subtree's string
        return findNode(node->left, idx, str_idx);
    } else {
        // We go to the right, so the current index will be updated as the
        // local index in the right subtree
        idx = idx - node->weight;
        // If index is greater than the weight, it is located in the right
        // subtree's string
        return findNode(node->right, idx, str_idx);
    }
}

/**
 * Find a character at given index in rope tree
 * @param rt the rope tree
 * @param idx index of the character
 */
char indexRope(RopeTree* rt, int idx) {
    if (rt == NULL) {
        printf("The Rope Tree is not created\n");
        return '\0';
    }

    int str_idx;
    // Find the node that contains the character at the given index
    // str_idx will be index of the character in node's string
    RopeNode *node = findNode(rt->root, idx, &str_idx);
    if (node != NULL)
        return node->str[str_idx];

    return '\0';
}

/**
 * Copy a number of characters from the source string into the destination
 * string
 * @param source the source string
 * @param destination the destination string
 * @param start starting point
 * @param end ending point
 */
void copy_string(const char *source, char *destination, int start, int end) {
    int len = strlen(destination);
    int cnt = start;
    // Copy char by char
    for (int i = len; i < len + end - start; ++i) {
        destination[i] = source[cnt++];
    }
    // Place the null terminator at the end of the destination string
    destination[len + end - start] = '\0';
}

/**
 * Recursively search the substring
 * @param node current node
 * @param start the starting index of the substring
 * @param int_len the length of the searched substring
 * @param str the returned substring
 * @param found 0 if first character in substring is not found, 1 if found
 * @param go_right 0 if current direction in traversal is left, 1 otherwise
 * @param traverse_all 0 if the whole right subtree will not be traversed, 1
 *                     otherwise
 */
void search_rec(RopeNode* node, int start, int int_len, char* str, int *found,
                int go_right, int *traverse_all)
{
    if (node == NULL) {
        return;
    }
    if (strlen(str) == (size_t)int_len) {
        return;
    }
    // Check if the first character in substring was found or not
    if (!(*found)) {
        // Starting index wasn't reached
        if (is_leaf(node)) {
            // We found the leaf that contains the character with index = start
            int len_str = int_len;
            // Copy as many characters as possible from leaf string
            if ((size_t)int_len > strlen(node->str) - start) {
                len_str = strlen(node->str) - start;
            }
            copy_string(node->str, str, start, start + len_str);

            // We mark that the leaf with the starting index was found
            *found = 1;
            return;
        }
        // If the starting index wasn't reached yet, we keep searching for it
        // The search is exactly the same as in findNode function
        if (node->weight > start) {
            // The direction in traversal is left
            go_right = 0;
            search_rec(node->left, start, int_len, str, found,
                    go_right, traverse_all);
        } else {
            // The start index is updated for the new subtree
            start = start - node->weight;
            // The direction in traversal is right
            go_right = 1;
            search_rec(node->right, start, int_len, str, found,
                        go_right, traverse_all);
        }
    }
    if ((*found) == 1) {
        // Starting index was already reached
        if (is_leaf(node)) {
            // Found leaf after starting point
            // Copy as many characters as possible from leaf string
            int len_str = int_len - strlen(str);
            if ((size_t)len_str > strlen(node->str)) {
                len_str = strlen(node->str);
            }
            copy_string(node->str, str, 0, len_str);
        }
        if (*traverse_all == 1) {
            // The whole subtree should be traversed
            search_rec(node->left, start, int_len, str, found, 0,
                        traverse_all);
            search_rec(node->right, start, int_len, str, found, 0,
                        traverse_all);
        }
        // If the traversal direction was left and the right was not traversed,
        // it should be traversed fully
        if (go_right == 0 && *traverse_all == 0) {
            // the whole right subtree will be traversed
            *traverse_all = 1;
            // Traverse the whole right subtree of node
            search_rec(node->right, start, int_len, str, found, 0,
                        traverse_all);
            *traverse_all = 0;
        }
    }
}

/**
 * Return the substring searched between 2 indexes
 * @param rt the tree where the search is conducted
 * @param start the starting index of searched substring
 * @param end the ending index of searched substring
 */
char* search(RopeTree* rt, int start, int end)
{
    // Length of searched substring
    int int_len = end - start;
    // Allocate the substring to be searched
    char *str = (char *)malloc((int_len + 1) * sizeof(char));
    check_malloc(str);
    strcpy(str, EMPTY);

    int found = 0;
    int traverse_all = 0;
    // Recursive search for the substring
    search_rec(rt->root, start, int_len, str, &found, 0, &traverse_all);
    // Check if substring was found
    if (strlen(str) < (size_t)int_len) {
        return NULL;
    }

    return str;
}

/**
 * Recursively split the tree
 * @param node the current node
 * @param parent the current node's parent
 * @param start the starting index of split
 * @param go_left retains the direction in traversal
 * @param rt_add the tree in which the removed/split nodes/subtrees are added
 */
void split_rec(RopeNode* node, RopeNode *parent, int start, int go_left,
               RopeTree **rt_add)
{
    if (node == NULL) {
        return;
    }
    if (is_leaf(node)) {
        int len_str = strlen(node->str);
        if (start != 0) {
            // First Case : Searched Node's string has to be split
            // Create the left string
            char *str_l = malloc((start + 1) * sizeof(char));
            check_malloc(str_l);
            // Create the right string
            char *str_r = malloc((len_str - start + 1) * sizeof(char));
            check_malloc(str_r);
            strcpy(str_l, "");
            strcpy(str_r, "");

            // Copy into right and left string
            copy_string(node->str, str_l, 0, start);
            copy_string(node->str, str_r, start, len_str);

            // Create right and left child of the current node
            RopeNode *node_l = makeRopeNode(str_l);
            RopeNode *node_r = makeRopeNode(str_r);

            // Make current node string empty
            free((void *)node->str);
            node->str = strdup(EMPTY);

            // Create links between the current node and its children
            node->left = node_l;
            node->right = node_r;
            // Readjust weight of current node
            node->weight = strlen(str_l);

            // Create tree with right child of current node as root
            *rt_add = makeRopeTree(node->right);
            node->right = NULL;
        }
        return;
    }
    if (node->weight > start) {
        go_left = 1;
        if (start == 0 && is_leaf(node->left)) {
            // Second Case : Searched Node is the left child of current node
            // Create tree with current node as root
            *rt_add = makeRopeTree(node);
            // Cut the link between parent and current node
            // Find on what side of the parent the current node is
            if (parent->left == node) {
                parent->left = NULL;
            } else {
                parent->right = NULL;
            }
            return;
        } else {
            // Split index wasn't reached, continue search
            split_rec(node->left, node, start, go_left, rt_add);
        }
    } else {
        go_left = 0;
        start = start - node->weight;
        if (start == 0 && is_leaf(node->right)) {
            // Third Case : Searched node is the right child of current node
            // Create tree with right child of current node as root
            *rt_add = makeRopeTree(node->right);
            // Cut the link between current node and right child
            node->right = NULL;
            return;
        } else {
            // Split index wasn't reached, continue search
            split_rec(node->right, node, start, go_left, rt_add);
        }
    }
    // If the traversal direction was left, the whole right subtree will be
    // splitted from the original tree
    if (go_left == 1) {
        // Create new rope tree with the right child as root
        RopeTree *curr_rt = makeRopeTree(node->right);
        // Concatenate the right subtree with the tree where the splitted
        // nodes/subtrees are added
        RopeTree *new_rt = concat(*rt_add, curr_rt);
        free(*rt_add);
        *rt_add = new_rt;
        free(curr_rt);
        // Readjust weight of node
        node->weight = getTotalWeight(node->left);
        node->right = NULL;
    }
}

/**
 * Recursively create a copy of the rope node and its descendants
 * @param node the node about to be copied
 */
RopeNode *copy_rope_node(RopeNode *node)
{
    if (node == NULL)
        return NULL;

    // Create the copy
    RopeNode *new_node = makeRopeNode(strdup(node->str));
    new_node->weight = node->weight;

    // Create copies of the node's children and attach them to the copied node
    new_node->left = copy_rope_node(node->left);
    new_node->right = copy_rope_node(node->right);

    return new_node;
}

/**
 * Create a copy of the rope tree
 * @param rope_tree the tree about to be copied
 */
RopeTree *copy_rope_tree(RopeTree *rope_tree)
{
    if (rope_tree == NULL)
        return NULL;

    // Make copies of all the nodes
    RopeTree *new_rope_tree = makeRopeTree(copy_rope_node(rope_tree->root));

    return new_rope_tree;
}

/**
 * Split tree in two parts
 * @param rt the tree about to be split
 * @param idx index where the split will be conducted
 */
SplitPair split(RopeTree* rt, int idx)
{
    RopeTree *rt_2 = NULL;

    SplitPair res;

    // If the index is out of bounds or 0, it is a special case
    if (idx <= 0) {
        RopeNode *copy_root = copy_rope_node(rt->root);

        res.left = makeRopeNode(strdup(EMPTY));
        res.right = copy_root;
        return res;
    } else if (idx > getTotalWeight(rt->root)) {
        RopeNode *copy_root = copy_rope_node(rt->root);

        res.left = copy_root;
        res.right = makeRopeNode(strdup(EMPTY));
        return res;
    }

    // Create a copy of the original tree in order to not modify it in the
    // split_rec function
    RopeTree *copy_rt = copy_rope_tree(rt);

    split_rec(copy_rt->root, copy_rt->root, idx, 1, &rt_2);
    res.left = copy_rt->root;
    res.right = rt_2->root;

    free(copy_rt);
    free(rt_2);

    return res;
}

/**
 * Insert string in rope tree
 * @param rt the tree where string is inserted
 * @param idx index where the string will be inserted
 * @param str string that will be inserted
 */
RopeTree* insert(RopeTree* rt, int idx, const char* str)
{
    // create tree with the root containing new string
    RopeNode *new_node = makeRopeNode(str);
    RopeTree *new_tree = makeRopeTree(new_node);

    // split tree at given index
    SplitPair splitted_tree = split(rt, idx);

    RopeTree *left_tree = makeRopeTree(splitted_tree.left);

    // concat leftmost tree with the newly created tree
    RopeTree *rt_1 = concat(left_tree, new_tree);

    RopeTree *right_tree = makeRopeTree(splitted_tree.right);
    // concat the last two remaining trees
    RopeTree *rt_final = concat(rt_1, right_tree);

    free(new_tree);
    free(rt_1);
    free(left_tree);
    free(right_tree);

    return rt_final;
}

/**
 * Delete substring from rope tree
 * @param rt the tree where the substring is deleted
 * @param start starting point of substring
 * @param len length of substring
 */
RopeTree* delete(RopeTree* rt, int start, int len)
{
    // split at ending point
    SplitPair splitted_right = split(rt, start + len);

    RopeTree *right_tree = makeRopeTree(splitted_right.right);
    RopeTree *aux_tree = makeRopeTree(splitted_right.left);

    // split at starting point
    SplitPair splitted_left = split(aux_tree, start);

    RopeTree *left_tree = makeRopeTree(splitted_left.left);
    RopeTree *middle_tree = makeRopeTree(splitted_left.right);
    // concat leftmost and rightmost trees
    RopeTree *final_tree = concat(left_tree, right_tree);

    free(right_tree);
    free(left_tree);
    free(aux_tree);
    // delete middle tree (where the substring is)
    free_rope_tree(middle_tree);

    return final_tree;
}
