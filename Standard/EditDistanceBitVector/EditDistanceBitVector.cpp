/*
 * > G. Myers. "A fast bit-vector algorithm for approximate string
 * > matching based on dynamic programming". Journal of the ACM, 1999.
 * 
 * > Inspired by: https://github.com/fujimotos/polyleven
 * 
 */

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <vector>
using namespace std;

#define CDIV(a,b) ((a) / (b) + ((a) % (b) > 0))
#define BIT(i,n) (((i) >> (n)) & 1)
#define FLIP(i,n) ((i) ^ ((uint64_t) 1 << (n)))

uint64_t *create_map(string s, long long int len) {
    long long int i;
    uint64_t *Peq;
    uint64_t *map;
    long long int maplen = 256 * CDIV(len, 64);

    map = (uint64_t*)calloc(maplen, sizeof(uint64_t));
    if (map == NULL)
        return NULL;

    for (i = 0; i < len; i++) {
        Peq = map + (256 * (i / 64));
        Peq[s[i]] |= (uint64_t) 1 << (i % 64);
    }
    return map;
}

long long int edit_distance_myers_bit_parallel(string s1, string s2, long long int s1len, long long int s2len) {
    uint64_t Eq, Xv, Xh, Ph, Mh, Pv, Mv, Last;
    uint64_t *Mhc, *Phc;
    long long int i, b, hsize, vsize, Score;
    uint8_t Pb, Mb;
    uint64_t *map;
    uint64_t *Peq;

    hsize = CDIV(s1len, 64);
    vsize = CDIV(s2len, 64);
    Score = s2len;

    if (s2len == 0) {
        return s1len;
    }

    map = create_map(s2, s2len);
    if (map == NULL)
        return -1;

    Phc = (uint64_t*)malloc(hsize * 2 * sizeof(uint64_t));
    if (Phc == NULL) {
        free(map);
        return -1;
    }

    Mhc = Phc + hsize;
    memset(Phc, -1, hsize * sizeof(uint64_t));
    memset(Mhc, 0, hsize * sizeof(uint64_t));
    Last = (uint64_t) 1 << ((s2len - 1) % 64);

    for (b = 0; b < vsize; b++) {
        Peq = map + (256 * b);
        Mv = 0;
        Pv = (uint64_t) -1;
        Score = s2len;

        for (i = 0; i < s1len; i++) {
            Eq = Peq[s1[i]];

            Pb = BIT(Phc[i / 64], i % 64);
            Mb = BIT(Mhc[i / 64], i % 64);

            Xv = Eq | Mv;
            Xh = ((((Eq | Mb) & Pv) + Pv) ^ Pv) | Eq | Mb;

            Ph = Mv | ~ (Xh | Pv);
            Mh = Pv & Xh;

            if (Ph & Last) Score++;
            if (Mh & Last) Score--;

            if ((Ph >> 63) ^ Pb)
                Phc[i / 64] = FLIP(Phc[i / 64], i % 64);

            if ((Mh >> 63) ^ Mb)
                Mhc[i / 64] = FLIP(Mhc[i / 64], i % 64);

            Ph = (Ph << 1) | Pb;
            Mh = (Mh << 1) | Mb;

            Pv = Mh | ~ (Xv | Ph);
            Mv = Ph & Xv;
        }
    }
    free(Phc);
    free(map);
    return Score;
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

    long long int s1len, s2len;
    s1len = s1.length();
    s2len = s2.length();
    cout << "String length 1 = " << s1len << endl;
    cout << "String length 2 = " << s2len << endl << endl;

    struct timespec start, end; 
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);

    long long int result = edit_distance_myers_bit_parallel(s1, s2, s1len, s2len);

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
    
    cout << "Edit distance:\t" << result << endl;
    
    long seconds = end.tv_sec - start.tv_sec;
    long nanoseconds = end.tv_nsec - start.tv_nsec;
    double elapsed = (seconds + nanoseconds*1e-9)*1000;
    
    cout << "Time (milliseconds) = " << elapsed << endl;
    
    return 0;
}
