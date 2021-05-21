/*
 * > H. Berghel, D. Roach. "An Extension of Ukkonen's Enhanced 
 * > Dynamic Programming ASM Algorithm". ACM Transactions on 
 * > Information Systems, Vol. 14, No. 1, pp. 94-106. 1996.
 *
 * > Inspired by: https://github.com/sunesimonsen/ukkonen
 *
 */

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <climits>
using namespace std;

int maximum(int a, int b, int c) {
    return a > b ? (a > c ? a : c) : (b > c ? b : c);
}

int edit_distance_enhanced_ukkonen (string s1, string s2, int s1len, int s2len, int threshold) {
    int i, t = 0;

    if (threshold == -1)
        threshold = INT_MAX;

    threshold = s2len < threshold ? s2len : threshold;

    int dLen = s2len - s1len;

    if(threshold < dLen) {
        return threshold;
    }

    int ZERO_K = ((s1len < threshold ? s1len : threshold) >> 1) + 2;

    int arrayLength = dLen + ZERO_K * 2 + 2;
    vector<int> currentRow (arrayLength, -1);
    vector<int> nextRow (arrayLength, -1);

    i = 0;
    int conditionRow = dLen + ZERO_K;
    int endMax = conditionRow << 1;
    do {
        i++;

        currentRow.swap(nextRow);

        int start;
        int previousCell;
        int currentCell = -1;
        int nextCell;

        if (i <= ZERO_K) {
            start = -i + 1;
            nextCell = i - 2;
        }
        else {
           start = i - (ZERO_K << 1) + 1;
           nextCell = currentRow[ZERO_K + start];
        }

        int iend;
        if (i <= conditionRow) {
            iend = i;
            nextRow[ZERO_K + i] = -1;
        } 
        else {
            iend = endMax - i;
        }

        for (int k = start, rowIndex = start + ZERO_K; k < iend; k++, rowIndex++) {
            previousCell = currentCell;
            currentCell = nextCell;
            nextCell = currentRow[rowIndex + 1];

            int t = currentCell + 1;
            t = maximum(t, previousCell, nextCell+1);

            while (t < s1len && t + k < s2len && s1.at(t) == s2.at(t + k)) {
                t++;
            }

            nextRow[rowIndex] = t;
        }
    } while (nextRow[conditionRow] < s1len && i <= threshold);

    return i - 1;
}

int main(int argc, char** argv) {
    string s1, s2, line;
    string txt = argv[1] != NULL ? argv[1] : "10000";
    string prefix = "../input/InputEditDistance";
    string suffix = ".txt";
    ifstream inputFile(prefix+txt+suffix);
    if (inputFile) {
        getline(inputFile, line);
        s1 = line;
        getline(inputFile, line);
        s2 = line;
    	
        inputFile.close();
    }
    else {
        cout << "Cannot open input file" << endl;
        return 0;
    }
    if(s1.length() < s2.length())
        swap(s1, s2);
    
    int s1len, s2len;
    int threshold = 6000;
    s1len = s1.length();
    s2len = s2.length();
    cout << "String length 1 = " << s1len << endl;
    cout << "String length 2 = " << s2len << endl << endl;
    
    struct timespec start, end; 
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
    
    int result = edit_distance_enhanced_ukkonen(s1, s2, s1len, s2len, threshold);
    
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
    
    cout << "Edit distance - Enhanced Ukkonen:\t" << result << endl;
    
    long seconds = end.tv_sec - start.tv_sec;
    long nanoseconds = end.tv_nsec - start.tv_nsec;
    double elapsed = (seconds + nanoseconds*1e-9)*1000;
       
    cout << "Time (milliseconds) = " << elapsed << endl;
    
    return 0;
}
