/*
 * > R. Wagner, M. Fischer, "The string-to-string correction problem".
 * > 1974.
 *
 * > Optimized version using just two rows.
 */

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <vector>
using namespace std;

int minimum(int a, int b, int c) {
    return a < b ? (a < c ? a : c) : (b < c ? b : c);
}

int edit_distance(string s1, string s2, int s1len, int s2len) {
    int x, y;
    vector<int> row1 (s2len+1);
    vector<int> row2 (s2len+1);

    for (x = 1; x <= s2len; x++)
        row1[x] = x;

    for (y = 1; y <= s1len; y++){
        row2[0] = x;
        for (x = 1; x <= s2len; x++)
            row2[x] = minimum(row1[x] + 1, row2[x-1] + 1, row1[x-1] + (s1.at(y-1) == s2.at(x-1) ? 0 : 1));
        row1.swap(row2);
    }

    return row1[s2len];
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
    s1len = s1.length();
    s2len = s2.length();
    cout << "String length 1 = " << s1len << endl;
    cout << "String length 2 = " << s2len << endl << endl;
    
    struct timespec start, end; 
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
    
    int result = edit_distance(s1, s2, s1len, s2len);
    
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
    
    cout << "Edit distance:\t" << result << endl;
    
    long seconds = end.tv_sec - start.tv_sec;
    long nanoseconds = end.tv_nsec - start.tv_nsec;
    double elapsed = (seconds + nanoseconds*1e-9)*1000;
    
    cout << "Time (milliseconds) = " << elapsed << endl;
    
    return 0;
}
