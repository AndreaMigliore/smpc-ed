/*
 * > E. Ukkonen. "Algorithms for approximate string matching".
 * > Information and control. 1985.
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

int minimum(int a, int b, int c) {
    return a < b ? (a < c ? a : c) : (b < c ? b : c);
}

int minimum_two(int a, int b) {
    return a < b ? a : b;
}

int maximum_two(int a, int b) {
    return a > b ? a : b;
}

int edit_distance_threshold(string s1, string s2, int s1len, int s2len, int threshold) {
    if (threshold == -1)
        threshold = INT_MAX;

    vector<int> p (s2len + 1);
    vector<int> d (s2len + 1, INT_MAX);

    int i, row, col, min, max;
    int boundary = minimum_two(s2len, threshold) + 1;
    for(i = 0; i < boundary; i++)
        p[i] = i;

    for( ; i <= s2len; i++)
        p[i] = INT_MAX;

    for(row = 1; row <= s1len; row++) {
        d[0] = row;

        min = maximum_two(1, row - threshold);
        max = minimum_two(s2len, row + threshold);

        if (min > max)
            return -1;

        if(min > 1)
            d[min-1] = INT_MAX;

        for(col = min; col <= max; col++) {
            if(s1.at(row-1) == s2.at(col-1))
                d[col] = p[col-1];
            else
                d[col] = minimum(p[col-1], d[col-1], p[col]) + 1;
        }
        p.swap(d);
    }

    if (p[s2len] <= threshold)
    	return p[s2len];
    else
        return -1;
}

int main(int argc, char** argv) {
    string s1, s2, line;
    string txt = argv[1] != NULL ? argv[1] : "1000";
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

    int threshold = s1.length()*6/10;
    if (argc == 3 && argv[2] != NULL)
        threshold = atoi(argv[2]);

    s1len = s1.length();
    s2len = s2.length();
    cout << "String length 1 = " << s1len << endl;
    cout << "String length 2 = " << s2len << endl;
    cout << "Threshold = " << threshold << endl << endl;

    s1len = s1.length();
    s2len = s2.length();
    cout << "String length 1 = " << s1len << endl;
    cout << "String length 2 = " << s2len << endl << endl;

    struct timespec start, end; 
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);

    int result = edit_distance_threshold(s1, s2, s1len, s2len, threshold);

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);

    cout << "Edit distance - Threshold:\t" << result << endl;

    long seconds = end.tv_sec - start.tv_sec;
    long nanoseconds = end.tv_nsec - start.tv_nsec;
    double elapsed = (seconds + nanoseconds*1e-9)*1000;
    
    cout << "Time (milliseconds) = " << elapsed << endl;

    return 0;
}
