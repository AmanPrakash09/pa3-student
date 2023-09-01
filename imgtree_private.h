/**
 * @file imgtree_private.h
 * @description student declarations of private functions
 *              for ImgTree, CPSC 221 2023S PA3
 *
 *              THIS FILE WILL BE SUBMITTED.
 *
 *              Simply declare your function prototypes here.
 *              No other scaffolding is necessary.
 */

// begin your declarations below

void mirror(Node* subRoot);

Node* CopyHelper(Node* subRoot);

void ClearHelper(Node* subRoot);

void PruneHelper(Node* subRoot, double tol);
// bool PruneHelper(Node*& subRoot, HSLAPixel rootAve, double tol);
bool checkTol(Node* subRoot, HSLAPixel rootAve, double tol);
// bool checkTol(Node* subRoot, Node* parent, double tol);

// void ClearPrime(Node* node);

// void PrunePrime(Node*& subRoot, HSLAPixel rootAve, double tol);
// void deleteNodeAndChildren(Node*& subRoot);