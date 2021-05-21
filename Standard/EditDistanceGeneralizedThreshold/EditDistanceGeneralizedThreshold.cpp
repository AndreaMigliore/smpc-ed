/*
 * > E. Ukkonen. "Algorithms for approximate string matching".
 * > Information and control. 1985.
 * 
 * > Inspired by: https://ceptord.net/wagner-fischer/index.html
 * 
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

int minimum_two(int a, int b) {
    return a < b ? a : b;
}

int maximum_two(int a, int b) {
    return a > b ? a : b;
}

int edit_distance_generalized_threshold(string s1, string s2, int s1len, int s2len) {
    int i, j, min, max, gmax, gmin, dia, top, left;

    vector<int> row (s2len+1);

    gmax = (s2len - 1) / 2;
    gmin = 1 - gmax - (s1len - s2len);

    for (j = 0; j <= gmax; j++)
        row[j] = j;

    for (i = 1; i <= s1len; i++) {
        row[0] = i - 1;

        min = maximum_two(gmin, 1);
        max = minimum_two(gmax, s2len);
        gmin++;
        gmax++;

        dia = row[min - 1];
        top = row[min];

        if (s1[i - 1] != s2[min - 1])
            dia = minimum_two(dia, top) + 1;

        row[min] = dia;
        left = dia;
        dia = top;

        for (j = min + 1; j <= max; j++) {
            top = row[j];

            if (s1[i - 1] != s2[j - 1])
                dia = minimum(dia, top, left) + 1;
            row[j] = dia;
            left = dia;
            dia = top;
        }

        if (s2len == max)
            continue;

        if (s1[i - 1] != s2[max])
            dia = minimum_two(dia, left) + 1;
        row[max + 1] = dia;
    }
    dia = row[s2len];
    return dia;
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

    int result = edit_distance_generalized_threshold(s1, s2, s1len, s2len);

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);

    cout << "Edit distance - Generalized Threshold:\t" << result << endl;

    long seconds = end.tv_sec - start.tv_sec;
    long nanoseconds = end.tv_nsec - start.tv_nsec;
    double elapsed = (seconds + nanoseconds*1e-9)*1000;
    
    cout << "Time (milliseconds) = " << elapsed << endl;

    return 0;
}
