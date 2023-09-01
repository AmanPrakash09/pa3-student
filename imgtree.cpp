/**
 * @file imgtree.cpp
 * @description implementation of ImgTree class used for storing image data
 *              CPSC 221 2023S PA3
 *
 *              SUBMIT THIS FILE WITH YOUR MODIFICATIONS
 */

#include "imgtree.h"
#include "imgtree_private.h"

#include <cfloat>  // For DBL_MAX

/* ImgTree constructor */
ImgTree::ImgTree(const PNG& imIn){ 
	// complete your implementation below

    Stats stat = Stats(imIn);
    
    // create the root node covering the whole image
	height = imIn.height();
	width = imIn.width();

    // std::cout << width << " " << height << endl;
    pair<unsigned int, unsigned int> ul = make_pair(0, 0);
    pair<unsigned int, unsigned int> lr = make_pair(width - 1, height - 1);
    
    // std::cout << "pre-build: " << ul.first << ", " << ul.second << "|" << lr.first << ", " << lr.second << endl;
	root = BuildTree(stat, ul, lr);
}

/* BuildTree helper for ImgTree constructor */
Node* ImgTree::BuildTree(Stats& s, pair<unsigned int, unsigned int> ul, pair<unsigned int, unsigned int> lr) {
	// // replace the stub below with your own implementation

    // base case: single pixel/no area
    // std::cout << "A" << endl;
    // std::cout << "building: " << ul.first << ", " << ul.second << "|" << lr.first << ", " << lr.second << endl;
    if ((lr.first <= ul.first) && (lr.second <= ul.second)) {
        Node* leaf = new Node(ul, lr, s.GetAvg(ul, lr));
        leaf->LT = NULL;
        leaf->RB = NULL;
        return leaf;
    }

    double minEntropy = DBL_MAX;
    // pair<unsigned int, unsigned int> lrLT;
    // pair<unsigned int, unsigned int> ulRB;

    Node* node = new Node(ul, lr, s.GetAvg(ul, lr));

    // wider --> a vertical split
    if ((lr.first - ul.first) >= (lr.second - ul.second)) {

        // Node* node = new Node(ul, lr, s.GetAvg(ul, lr));
        unsigned int firstCopy = ul.first;
        unsigned int hDiff     = (lr.second - ul.second);

        // std::cout << "B" << endl;
        // std::cout << ul.first << " " << lr.first << endl;
        for (unsigned int x = 1; x <= (lr.first - ul.first); x++) {
            // std::cout << "C" << endl;
            // pair<unsigned int, unsigned int> temp_lrLT = make_pair(x - 1, lr.second);
            // pair<unsigned int, unsigned int> temp_ulRB = make_pair(x, ul.second);
            pair<unsigned int, unsigned int> temp_lr = make_pair(ul.first + x - 1, lr.second);
            pair<unsigned int, unsigned int> temp_ul = make_pair(ul.first + x, ul.second);

            // double entropyLT = s.Entropy(ul, temp_lrLT);
            // double entropyRB = s.Entropy(temp_ulRB, lr);
            // double areaLT = s.RectArea(ul, temp_lrLT);
            // double areaRB = s.RectArea(temp_ulRB, lr);

            // double weightedEntropy = (entropyLT * areaLT) + (entropyRB * areaRB);

            // double area1 = s.RectArea(ul, temp_lr) / (s.RectArea(ul, temp_lr) + s.RectArea(temp_ul, lr));
            // double area2 = s.RectArea(temp_ul, lr) / (s.RectArea(ul, temp_lr) + s.RectArea(temp_ul, lr));

            // double weightedEntropy = (area1 * s.Entropy(ul, temp_lr)) + (area2 * s.Entropy(temp_ul, lr));

            double area_temp_lr = s.RectArea(ul, temp_lr);
            double area_temp_ul = s.RectArea(temp_ul, lr);
            double totalArea = area_temp_lr + area_temp_ul;

            double weightedEntropy = ((area_temp_lr / totalArea) * s.Entropy(ul, temp_lr)) + (area_temp_ul / totalArea) * s.Entropy(temp_ul, lr);

            if (weightedEntropy < minEntropy) {
                minEntropy = weightedEntropy;
                // lrLT = temp_lr;
                // ulRB = temp_ul;
                firstCopy = temp_lr.first;
                hDiff     = temp_lr.second;
            }
        }

        node->LT = BuildTree(s, ul, make_pair(firstCopy, hDiff));
        node->RB = BuildTree(s, make_pair(firstCopy + 1, ul.second), lr);
        return node;
    }
    // taller --> horizontal split
    else {
        unsigned int secondCopy = ul.second;
        unsigned int wDiff     = (lr.first - ul.first);
        
        for (unsigned int y = 1; y <= (lr.second - ul.second); y++) {
            pair<unsigned int, unsigned int> temp_lr = make_pair(lr.first, ul.second + y - 1);
            pair<unsigned int, unsigned int> temp_ul = make_pair(ul.first, ul.second + y);

            // double entropyLT = s.Entropy(ul, temp_lrLT);
            // double entropyRB = s.Entropy(temp_ulRB, lr);
            // double areaLT = s.RectArea(ul, temp_lrLT);
            // double areaRB = s.RectArea(temp_ulRB, lr);

            // double weightedEntropy = (entropyLT * areaLT) + (entropyRB * areaRB);

            // double area1 = s.RectArea(ul, temp_lr) / (s.RectArea(ul, temp_lr) + s.RectArea(temp_ul, lr));
            // double area2 = s.RectArea(temp_ul, lr) / (s.RectArea(ul, temp_lr) + s.RectArea(temp_ul, lr));

            // double weightedEntropy = (area1 * s.Entropy(ul, temp_lr)) + (area2 * s.Entropy(temp_ul, lr));

            double area_temp_lr = s.RectArea(ul, temp_lr);
            double area_temp_ul = s.RectArea(temp_ul, lr);
            double totalArea = area_temp_lr + area_temp_ul;

            double weightedEntropy = ((area_temp_lr / totalArea) * s.Entropy(ul, temp_lr)) + (area_temp_ul / totalArea) * s.Entropy(temp_ul, lr);

            if (weightedEntropy < minEntropy) {
                minEntropy = weightedEntropy;
                wDiff = temp_lr.first;
                secondCopy = temp_lr.second;
            }
        }

        node->LT = BuildTree(s, ul, make_pair(wDiff, secondCopy));
        node->RB = BuildTree(s, make_pair(ul.first, secondCopy + 1), lr);
        return node;

    }
}

/* Render your ImgTree into a png */
PNG ImgTree::Render() const{
	// replace the stub below with your own implementation
    // std::cout << "render" << endl;

    PNG img(width, height);
    vector<Node*> nodes;
    nodes.push_back(root);

    // need to get leaf nodes --> set pixels
    for(size_t i = 0; i < nodes.size(); i++) {
        Node* current = nodes[i];
        if(current->LT == nullptr && current->RB == nullptr) {
            for (unsigned int x = current->upLeft.first; x <= current->lowRight.first; x++) {
                for (unsigned int y = current->upLeft.second; y <= current->lowRight.second; y++) {
                    // std::cout << "yo: " << x << " " << y << endl;
                    // std::cout << "current->lowRight.first: " << current->lowRight.first << "current->lowRight.second " << current->lowRight.second << endl;
                    HSLAPixel* pixel = img.getPixel(x, y);
                    *pixel = current->avg;
                }
            }
        } else {
            if(current->LT != nullptr) {
                nodes.push_back(current->LT);
            }
            if(current->RB != nullptr) {
                nodes.push_back(current->RB);
            }
        }
    }

    return img;
}

/* Prune function modifies tree by cutting off
 * subtrees whose leaves are all within tol of 
 * the average pixel value contained in the root
 * of the subtree
 */
void ImgTree::Prune(double tol){
	// complete your implementation below

    // need to get to leaf nodes --> check if within tolerance --> delete them
    // need to keep track of root's average
    // delete entire subtree if all the children are leaf nodes and within tolerance

    // HSLAPixel rootAve = root->avg;

    // std::cout << "Before --> Nodes: " << CountNodes(root) << " , Leaves: " << CountLeaves(root) << endl;

    PruneHelper(root, tol);
    // PruneHelper(root, rootAve, tol);
    // PrunePrime(root, rootAve, tol);

    // std::cout << "After --> Nodes: " << CountNodes(root) << " , Leaves: " << CountLeaves(root) << endl;
}

// void ImgTree::PrunePrime(Node*& subRoot, HSLAPixel rootAve, double tol) {

//     // if tree has no nodes
//     if (subRoot == NULL) {
//         return;
//     }

//     // Post-order traversal: First visit left and right subtrees 
//     PrunePrime(subRoot->LT, rootAve, tol);
//     PrunePrime(subRoot->RB, rootAve, tol);

//     // Then check the node itself
//     if (rootAve.distanceTo(subRoot->avg) <= tol) {
//         ClearHelper(subRoot);
//         subRoot = NULL;
//         return;
//     }
// }

// void ImgTree::deleteNodeAndChildren(Node*& subRoot) {
//     if (subRoot == NULL) {
//         return;
//     }

//     deleteNodeAndChildren(subRoot->LT);
//     deleteNodeAndChildren(subRoot->RB);

//     delete subRoot;
//     subRoot = NULL;
// }

// bool ImgTree::PruneHelper(Node*& subRoot, HSLAPixel rootAve, double tol) {
//     if (subRoot == NULL) {
//         return false;
//     }

//     // Post-order traversal to prune children first
//     bool prunedLeft = PruneHelper(subRoot->LT, subRoot->avg, tol);
//     bool prunedRight = PruneHelper(subRoot->RB, subRoot->avg, tol);

//     // If either child was pruned, check if we can prune this node
//     if ((prunedLeft && prunedRight) || rootAve.distanceTo(subRoot->avg) <= tol) {
//         if(subRoot->LT == NULL && subRoot->RB == NULL){
//             delete(subRoot);
//             subRoot = NULL;
//             return true;
//         }
//     }

//     return false;
// }

void ImgTree::PruneHelper(Node* subRoot, double tol) {

    // if tree has no nodes
    if (subRoot == NULL) {
        return;
    }

    if (subRoot->LT != NULL && subRoot->RB != NULL) {
        if (checkTol(subRoot, subRoot->avg, tol)) {
            // if (checkTol(subRoot, subRoot, tol)) {
            // delete subroot children
            ClearHelper(subRoot->LT);
            // subRoot->LT = NULL;
            ClearHelper(subRoot->RB);
            subRoot->LT = NULL;
            subRoot->RB = NULL;
        }
        else {
        PruneHelper(subRoot->LT, tol);
        PruneHelper(subRoot->RB, tol);
        }
    }

    // if (!subRoot->LT && !subRoot->RB) {
    //     return;
    // }
    // if (checkTol(subRoot, subRoot, tol)) {
    //     ClearPrime(subRoot->LT);
    //     ClearPrime(subRoot->RB);
    // } else {
    //     PruneHelper(subRoot->LT, tol);
    //     PruneHelper(subRoot->RB, tol);
    // }
}

bool ImgTree::checkTol(Node* subRoot, HSLAPixel rootAve, double tol) {
// bool ImgTree::checkTol(Node* subRoot, Node* parent, double tol) {

    // // accounted for tree without nodes
    // // base case: have reached leaf node within tolerance
    if (subRoot == nullptr) {
        return true;
    }
    
    // if (rootAve.distanceTo(subRoot->avg) <= tol) {
    //     return checkTol(subRoot->LT, rootAve, tol) && checkTol(subRoot->RB, rootAve, tol);
    // }

    // return false;

    // accounted for tree without nodes
    // base case: have reached leaf node within tolerance
    // if (subRoot == nullptr) {
    //     return false;
    // }
    
    if (subRoot->LT == NULL && subRoot->RB == NULL) {
        if (rootAve.distanceTo(subRoot->avg) <= tol) {
            return true;
        }
        else {
            return false;
        }
    }
    
    return checkTol(subRoot->LT, rootAve, tol) && checkTol(subRoot->RB, rootAve, tol);

    // return false;

    // // accounted for tree without nodes
    // if (subRoot == nullptr) {
    //     return true;
    // }
    
    // if (rootAve.distanceTo(subRoot->avg) <= tol) {
    //     return true;
    // }

    // return false;

    //if nd is null, just returns true
    // if (!subRoot) {
    //     return true;
    // }
    // //nd has no children, just compare  nd's  color with target's
    // if (!subRoot->RB && !subRoot->LT) {
    //     return parent->avg.distanceTo(subRoot->avg) <= tol;
    // //if nd has children (one or more), check if the nd;s children is within the distance of tol of the target
    // } else {
    //     return checkTol(subRoot->RB, parent, tol) && checkTol(subRoot->LT, parent, tol);

    // }
}

/* Modifies node contents so that the rendered tree
 * will appear to be flipped horizontally across
 * a vertical axis
 */
void ImgTree::FlipHorizontal() {
	// complete your implementation below

    mirror(root);
    // std::cout << "DOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOONE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
    
}

void ImgTree::mirror(Node* subRoot) {

    // pre-order
    if (subRoot == NULL) {
      return;
    }

    // std::cout << "mirror: " << subRoot->upLeft.first << ", " << subRoot->upLeft.second << "|" << subRoot->lowRight.first << ", " << subRoot->lowRight.second << endl;

    // flip data in tree (swap width coordinates)
    // if ((subRoot->lowRight.first - subRoot->upLeft.first) >= (subRoot->lowRight.second - subRoot->upLeft.second)) {
    //     unsigned int ulSwap = width - 1 - subRoot->upLeft.first;
    //     unsigned int lrSwap = width - 1 - subRoot->lowRight.first;
    //     subRoot->upLeft.first = ulSwap;
    //     subRoot->lowRight.first = lrSwap;
    // }
    // else {
    //     unsigned int ulSwap = height - 1 - subRoot->upLeft.second;
    //     unsigned int lrSwap = height - 1 - subRoot->lowRight.second;
    //     subRoot->upLeft.second = ulSwap;
    //     subRoot->lowRight.second = lrSwap;
    // }

    // mirror(subRoot->LT);
    // mirror(subRoot->RB);

    if ((subRoot->lowRight.first - subRoot->upLeft.first) >= (subRoot->lowRight.second - subRoot->upLeft.second)) {
        // flip tree (swap children)
        Node* temp = subRoot->LT;
        subRoot->LT = subRoot->RB;
        subRoot->RB = temp;
    }

    unsigned int newULfirst = width - 1 - subRoot->upLeft.first;
    unsigned int newLRfirst = width - 1 - subRoot->lowRight.first;
    // std::cout << newULfirst << ", " << newLRfirst << " | " << subRoot->upLeft.first << ", " << subRoot->lowRight.first << endl;
    subRoot->upLeft.first = newLRfirst;
    subRoot->lowRight.first = newULfirst;
    // std::cout << newULfirst << ", " << newLRfirst << " | " << subRoot->upLeft.first << ", " << subRoot->lowRight.first << " --> AFTER" << endl;
    
    // unsigned int newULfirst = width - 1 - subRoot->lowRight.first;
    // unsigned int newLRfirst = width - 1 - subRoot->upLeft.first;
    // std::cout << newULfirst << ", " << newLRfirst << " | " << subRoot->upLeft.first << ", " << subRoot->lowRight.first << endl;
    // subRoot->upLeft.first = newULfirst;
    // subRoot->lowRight.first = newLRfirst;
    // std::cout << newULfirst << ", " << newLRfirst << " | " << subRoot->upLeft.first << ", " << subRoot->lowRight.first << " --> AFTER" << endl;

    // unsigned int temp1 = subRoot->upLeft.first;
    // unsigned int temp2 = subRoot->lowRight.first;
    // subRoot->upLeft.first = temp2;
    // subRoot->lowRight.first = temp1;

    // flip tree (swap children)
    // Node* temp = subRoot->LT;
    // subRoot->LT = subRoot->RB;
    // subRoot->RB = temp;

    mirror(subRoot->LT);
    mirror(subRoot->RB);
}

/* helper function for destructor and op= */
/* frees dynamic memory associated w the ImgTree */
void ImgTree::Clear() {
	// complete your implementation below
    ClearHelper(root);
    root = NULL;
}

void ImgTree::ClearHelper(Node* subRoot) {
    if (subRoot != NULL) {
        ClearHelper(subRoot->LT);
        ClearHelper(subRoot->RB);
        delete(subRoot);
    }
}

/* helper function for copy constructor and op= */
void ImgTree::Copy(const ImgTree& orig){
	// complete your implementation below
    height = orig.height;
    width = orig.width;

    root = CopyHelper(orig.root);
}

Node* ImgTree::CopyHelper(Node* subRoot) {

    if (subRoot == NULL) {
        return NULL;
    }

    Node* newNode = new Node(subRoot->upLeft, subRoot->lowRight, subRoot->avg);

    newNode->LT = CopyHelper(subRoot->LT);
    newNode->RB = CopyHelper(subRoot->RB);

    return newNode;

}

/**
 * Add your private ImgTree function implementations below
 */
