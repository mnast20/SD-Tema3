# SD-Tema3

Copyright 2021 Calin Coman & Nastase Maria
Grupa 311CA

## DATA STRUCTURES:
- RopeNode: node of a Rope Tree.
    - contains pointers to left and right child, a pointer to a string, and its weight.
- RopeTree: a rope tree
    - contains pointer to its root.
- SplitPair: contains pointer to roots of 2 parts of a splitted tree.

## SEARCH ALGORITHM:
This principle will be used in the next operations. During the traversal of the rope tree, if the index of the searched character is smaller than the weight of the current node, it means that the character is in the left subtree, so we continue the search there. Otherwise, we continue on the right side of the node and the index will be updated to the correct index in the right subtree.

## OPERATIONS:
### Concat
Creates a new node that will be the root of the newly created tree. The children of the new root will be the roots of the two trees that need to be concatenated. 

### Index
Makes a recursive search of the character at the given index. It makes use of the search algorithm presented above.

### Search
It uses the search algorithm to find the starting point of the substring. Then, if it hasn't reached the ending point of the substring, the function traverses the right subtrees that weren't visited before.
        
### Split
It uses the search algorithm to find the splitting point. Then, when it goes back in the recursive stack, if the direction during the traversal was left, it splits the right subtree.
        
### Insert
First, the tree will be splitted in two parts. Then, we create a node with the given string and we concatenate the 3 parts (the 2 trees and the node inbetween).
        
### Delete
We split the tree twice (once at the end index and once at the start index) and we get 3 trees, with the one in the middle containing the substring we want to delete. Now, we free the middle tree and concatenate the other 2.


# Link to Project:
https://ocw.cs.pub.ro/courses/sd-ca/teme/tema3-2021
