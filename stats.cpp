/**
 * @file stats.cpp
 * @description implementation of Stats class used for fast computation of
 *              colour information from rectangular image regions
 *              CPSC 221 2023S PA3
 *
 *              SUBMIT THIS FILE WITH YOUR MODIFICATIONS
 */

#include "stats.h"

#define NUMBINS 36 // number of histogram bins

Stats::Stats(const PNG& im){
    // complete your implementation below

    int width = im.width();
    int height = im.height();
    
    sumHueX.resize(width, vector<double>(height, 0));
    sumHueY.resize(width, vector<double>(height, 0));
    sumSat.resize(width, vector<double>(height, 0));
    sumLum.resize(width, vector<double>(height, 0));
    hist.resize(width, vector<vector<unsigned int>>(height, vector<unsigned int>(NUMBINS, 0)));

    for(int i = 0; i < width; i++) {
        for(int j = 0; j < height; j++) {
            HSLAPixel* pixel = im.getPixel(i, j);

            sumHueX[i][j] = cos(pixel->h * PI / 180.0);
            sumHueY[i][j] = sin(pixel->h * PI / 180.0);
            sumSat[i][j] = pixel->s;
            sumLum[i][j] = pixel->l;

            if(i > 0) {
                sumHueX[i][j] += sumHueX[i-1][j];
                sumHueY[i][j] += sumHueY[i-1][j];
                sumSat[i][j] += sumSat[i-1][j];
                sumLum[i][j] += sumLum[i-1][j];
            }

            if(j > 0) {
                sumHueX[i][j] += sumHueX[i][j-1];
                sumHueY[i][j] += sumHueY[i][j-1];
                sumSat[i][j] += sumSat[i][j-1];
                sumLum[i][j] += sumLum[i][j-1];
            }

            if(i > 0 && j > 0) {
                sumHueX[i][j] -= sumHueX[i-1][j-1];
                sumHueY[i][j] -= sumHueY[i-1][j-1];
                sumSat[i][j] -= sumSat[i-1][j-1];
                sumLum[i][j] -= sumLum[i-1][j-1];
            }

            for(int k = 0; k < NUMBINS; k++){
                hist[i][j][k] = 0;
                if(i > 0){
                    hist[i][j][k] += hist[i-1][j][k];
                }
                if(j > 0){
                    hist[i][j][k] += hist[i][j-1][k];
                }
                if(i > 0 && j > 0){
                    hist[i][j][k] -= hist[i-1][j-1][k];
                }
                
                if(k * 10 <= (int)im.getPixel(i, j)->h && (int)im.getPixel(i, j)->h < (k + 1) * 10){
                    hist[i][j][k] += 1;
                }
            }
        }
    }

    // int width = im.width();
    // int height = im.height();
    
    // sumHueX.resize(height, vector<double>(width, 0));
    // sumHueY.resize(height, vector<double>(width, 0));
    // sumSat.resize(height, vector<double>(width, 0));
    // sumLum.resize(height, vector<double>(width, 0));
    // hist.resize(height, vector<vector<unsigned int>>(width, vector<unsigned int>(NUMBINS, 0)));

    // for(int i = 0; i < height; i++) {
    //     for(int j = 0; j < width; j++) {
    //         HSLAPixel* pixel = im.getPixel(j, i);
    //         // int bin = pixel->h / 10; // histogram bin for the hue

    //         sumHueX[i][j] = cos(pixel->h * PI / 180.0);
    //         sumHueY[i][j] = sin(pixel->h * PI / 180.0);
    //         sumSat[i][j] = pixel->s;
    //         sumLum[i][j] = pixel->l;
    //         // hist[i][j][bin]++;

    //         if(i > 0) {
    //             sumHueX[i][j] += sumHueX[i-1][j];
    //             sumHueY[i][j] += sumHueY[i-1][j];
    //             sumSat[i][j] += sumSat[i-1][j];
    //             sumLum[i][j] += sumLum[i-1][j];
    //             // for (int k = 0; k < NUMBINS; k++) {
    //             //     hist[i][j][k] += hist[i-1][j][k];
    //             // }
    //         }

    //         if(j > 0) {
    //             sumHueX[i][j] += sumHueX[i][j-1];
    //             sumHueY[i][j] += sumHueY[i][j-1];
    //             sumSat[i][j] += sumSat[i][j-1];
    //             sumLum[i][j] += sumLum[i][j-1];
    //             // for (int k = 0; k < NUMBINS; k++) {
    //             //     hist[i][j][k] += hist[i][j-1][k];
    //             // }
    //         }

    //         if(i > 0 && j > 0) {
    //             sumHueX[i][j] -= sumHueX[i-1][j-1];
    //             sumHueY[i][j] -= sumHueY[i-1][j-1];
    //             sumSat[i][j] -= sumSat[i-1][j-1];
    //             sumLum[i][j] -= sumLum[i-1][j-1];
    //             // for (int k = 0; k < NUMBINS; k++) {
    //             //     hist[i][j][k] -= hist[i-1][j-1][k];
    //             // }
    //         }

    //         for(int k = 0; k < NUMBINS; k++){
    //             hist[i][j][k] = 0;
    //             if(i > 0){
    //                 hist[i][j][k] += hist[i-1][j][k];
    //             }
    //             if(j > 0){
    //                 hist[i][j][k] += hist[i][j-1][k];
    //             }
    //             if(i > 0 && j > 0){
    //                 hist[i][j][k] -= hist[i-1][j-1][k];
    //             }
                
    //             if(k*10 <= (int)im.getPixel(j, i)->h && (int)im.getPixel(j, i)->h < (k+1)*10){
    //                 hist[i][j][k] += 1;
    //             }
    //         }
    //     }
    // }
    
}

long Stats::RectArea(pair<unsigned int, unsigned int> ul, pair<unsigned int, unsigned int> lr){
    // replace the stub below with your completed implementation

    // add 1 since values go from 0 to dimension - 1
    unsigned int width = (lr.first - ul.first) + 1;
    unsigned int height = (lr.second - ul.second) + 1;

    return (long)width * height;
}

HSLAPixel Stats::GetAvg(pair<unsigned int, unsigned int> ul, pair<unsigned int, unsigned int> lr){

    double area = double(RectArea(ul, lr));
    
    double aveHueX = sumHueX[lr.first][lr.second];
    double aveHueY = sumHueY[lr.first][lr.second];
    double aveSat = sumSat[lr.first][lr.second];
    double aveLum = sumLum[lr.first][lr.second];

    if (ul.first == 0 && ul.second == 0) {
        aveHueX = sumHueX[lr.first][lr.second];
        aveHueY = sumHueY[lr.first][lr.second];
        aveSat = sumSat[lr.first][lr.second];
        aveLum = sumLum[lr.first][lr.second];
    }
    else if(ul.first == 0){
        aveHueX = sumHueX[lr.first][lr.second] -  sumHueX[lr.first][ul.second-1];
        aveHueY = sumHueY[lr.first][lr.second] -  sumHueY[lr.first][ul.second-1];
        aveSat  = sumSat[lr.first][lr.second]  -  sumSat[lr.first][ul.second-1];
        aveLum  = sumLum[lr.first][lr.second]  -  sumLum[lr.first][ul.second-1];
    }
    else if(ul.second == 0){
        aveHueX = sumHueX[lr.first][lr.second] - sumHueX[ul.first-1][lr.second];
        aveHueY = sumHueY[lr.first][lr.second] - sumHueY[ul.first-1][lr.second];
        aveSat  = sumSat[lr.first][lr.second]  - sumSat[ul.first-1][lr.second];
        aveLum  = sumLum[lr.first][lr.second]  - sumLum[ul.first-1][lr.second];
    }
    else if(ul.second > 0 && ul.first > 0){
        aveHueX = sumHueX[lr.first][lr.second] + sumHueX[ul.first-1][ul.second-1] - sumHueX[ul.first-1][lr.second] - sumHueX[lr.first][ul.second-1];
        aveHueY = sumHueY[lr.first][lr.second] + sumHueY[ul.first-1][ul.second-1] - sumHueY[ul.first-1][lr.second] - sumHueY[lr.first][ul.second-1];
        aveSat  = sumSat[lr.first][lr.second]  + sumSat[ul.first-1][ul.second-1]  - sumSat[ul.first-1][lr.second]  - sumSat[lr.first][ul.second-1];
        aveLum  = sumLum[lr.first][lr.second]  + sumLum[ul.first-1][ul.second-1]  - sumLum[ul.first-1][lr.second]  - sumLum[lr.first][ul.second-1];
    }

    // aveHueX /= area;
    // aveHueY /= area;
    aveSat /= area;
    aveLum /= area;

    double avgHue = atan2(aveHueY, aveHueX) * 180 / PI;
    if (avgHue < 0) {
        avgHue += 360;
    }

    return HSLAPixel(avgHue, aveSat, aveLum, 1.0);

    // replace the stub below with your completed implementation
    // double area = double(RectArea(ul, lr));
    
    // double aveHueX = sumHueX[lr.second][lr.first];
    // double aveHueY = sumHueY[lr.second][lr.first];
    // double aveSat = sumSat[lr.second][lr.first];
    // double aveLum = sumLum[lr.second][lr.first];

    // // if(ul.second > 0){
    // //     aveHueX -= sumHueX[ul.second-1][lr.first];
    // //     aveHueY -= sumHueY[ul.second-1][lr.first];
    // //     aveSat -= sumSat[ul.second-1][lr.first];
    // //     aveLum -= sumLum[ul.second-1][lr.first];
    // // }
    // // if(ul.first > 0){
    // //     aveHueX -= sumHueX[lr.second][ul.first-1];
    // //     aveHueY -= sumHueY[lr.second][ul.first-1];
    // //     aveSat -= sumSat[lr.second][ul.first-1];
    // //     aveLum -= sumLum[lr.second][ul.first-1];
    // // }
    // // if(ul.second > 0 && ul.first > 0){
    // //     aveHueX += sumHueX[ul.second-1][ul.first-1];
    // //     aveHueY += sumHueY[ul.second-1][ul.first-1];
    // //     aveSat += sumSat[ul.second-1][ul.first-1];
    // //     aveLum += sumLum[ul.second-1][ul.first-1];
    // // }

    // if (ul.first == 0 && ul.second == 0) {
    //     aveHueX = sumHueX[lr.second][lr.first];
    //     aveHueY = sumHueY[lr.second][lr.first];
    //     aveSat = sumSat[lr.second][lr.first];
    //     aveLum = sumLum[lr.second][lr.first];
    // }
    // else if(ul.first == 0){
    //     aveHueX = sumHueX[lr.second][lr.first] -  sumHueX[ul.second-1][lr.first];
    //     aveHueY = sumHueY[lr.second][lr.first] -  sumHueY[ul.second-1][lr.first];
    //     aveSat  = sumSat[lr.second][lr.first]  -  sumSat[ul.second-1][lr.first];
    //     aveLum  = sumLum[lr.second][lr.first]  -  sumLum[ul.second-1][lr.first];
    // }
    // else if(ul.second == 0){
    //     aveHueX = sumHueX[lr.second][lr.first] - sumHueX[lr.second][ul.first-1];
    //     aveHueY = sumHueY[lr.second][lr.first] - sumHueY[lr.second][ul.first-1];
    //     aveSat  = sumSat[lr.second][lr.first]  - sumSat[lr.second][ul.first-1];
    //     aveLum  = sumLum[lr.second][lr.first]  - sumLum[lr.second][ul.first-1];
    // }
    // else if(ul.second > 0 && ul.first > 0){
    //     aveHueX = sumHueX[lr.second][lr.first] + sumHueX[ul.second-1][ul.first-1] - sumHueX[lr.second][ul.first-1] - sumHueX[ul.second-1][lr.first];
    //     aveHueY = sumHueY[lr.second][lr.first] + sumHueY[ul.second-1][ul.first-1] - sumHueY[lr.second][ul.first-1] - sumHueY[ul.second-1][lr.first];
    //     aveSat  = sumSat[lr.second][lr.first]  + sumSat[ul.second-1][ul.first-1]  - sumSat[lr.second][ul.first-1]  - sumSat[ul.second-1][lr.first];
    //     aveLum  = sumLum[lr.second][lr.first]  + sumLum[ul.second-1][ul.first-1]  - sumLum[lr.second][ul.first-1]  - sumLum[ul.second-1][lr.first];
    // }

    // aveHueX /= area;
    // aveHueY /= area;
    // aveSat /= area;
    // aveLum /= area;

    // double avgHue = atan2(aveHueY, aveHueX) * 180 / PI;
    // if (avgHue < 0) {
    //     avgHue += 360;
    // }

    // return HSLAPixel(avgHue, aveSat, aveLum, 1.0);
}

double Stats::Entropy(pair<unsigned int, unsigned int> ul, pair<unsigned int, unsigned int> lr){

    vector<int> distribution;

    double entropy = 0;
    long area = 0;

    /**
    *  Complete your implementation below.
    *  Using private member hist, assemble the distribution over the
    *  given rectangle defined by points ul, and lr into variable distribution.
    *  You will use distribution to compute the entropy over the rectangle.
    *  if any bin in the distribution has frequency 0, then do not add that 
    *  term to the entropy total. see .h file for more details.
    */

    /* your code should include the following lines (or something similar):
        if (distribution[i] > 0 ) 
            entropy += ((double) distribution[i]/(double) area) 
                                    * log2((double) distribution[i]/(double) area);
    */
    area = RectArea(ul, lr);
    distribution.resize(NUMBINS);

    for(int i=0; i<NUMBINS; i++){

        if (ul.first == 0 && ul.second == 0) {
            distribution[i] = hist[lr.first][lr.second][i];
        }
        else if (ul.first == 0){
            distribution[i] = hist[lr.first][lr.second][i] -  hist[lr.first][ul.second-1][i];
        }
        else if (ul.second == 0){
            distribution[i] = hist[lr.first][lr.second][i] - hist[ul.first-1][lr.second][i];
        }
        else if (ul.first > 0 && ul.second > 0){
            distribution[i] = hist[lr.first][lr.second][i] -  hist[lr.first][ul.second-1][i] - hist[ul.first-1][lr.second][i] + hist[ul.first-1][ul.second-1][i];
        }

        if (distribution[i] > 0 ) 
            entropy += ((double) distribution[i]/(double) area) 
                                    * log2((double) distribution[i]/(double) area);
    }
    
    return  -1 * entropy;
    // area = RectArea(ul, lr);
    // distribution.resize(NUMBINS);

    // for(int i=0; i<NUMBINS; i++){
    //     distribution[i] = hist[lr.second][lr.first][i];

    //     if(ul.second > 0){
    //         distribution[i] -= hist[ul.second-1][lr.first][i];
    //     }
    //     if(ul.first > 0){
    //         distribution[i] -= hist[lr.second][ul.first-1][i];
    //     }
    //     if(ul.second > 0 && ul.first > 0){
    //         distribution[i] += hist[ul.second-1][ul.first-1][i];
    //     }

    //     if (distribution[i] > 0 ) 
    //         entropy += ((double) distribution[i]/(double) area) 
    //                                 * log2((double) distribution[i]/(double) area);
    // }
    
    // return  -1 * entropy;
}

/**
 * Add your private Stats function implementations below
 */