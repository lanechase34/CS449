/* 
 * csim.c - A cache simulator that can replay traces from Valgrind
 *     and output statistics such as number of hits, misses, and
 *     evictions.  The replacement policy is LRU.
 * 
 * Implementation and assumptions:
 * 
 *  1. Each load/store can cause at most one cache miss. (I examined the trace,
 *  the largest request I saw was for 8 bytes).
 *
 *  2. Instruction loads (I) are ignored, since we are interested in evaluating
 *  data cache performance.
 *
 *  3. data modify (M) is treated as a load followed by a store to the same
 *  address. Hence, an M operation can result in two cache hits, or a miss and a
 *  hit plus an possible eviction.
 *
 * The function printSummary() is given to print output.
 * Please use this function to print the number of hits, misses and evictions.
 * IMPORTANT: This is crucial for the driver to evaluate your work. 
 *
 */

#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <limits.h>
#include <string.h>
#include <errno.h>

//#define DEBUG_ON 
#define ADDRESS_LENGTH 64

/* Type: Memory address */
typedef unsigned long long int mem_addr_t;

/*
 * Data structures to represent the cache we are simulating
 *
 * TODO: Define your own!
 *
 * E.g., Types: cache, cache line, cache set
 * (you can use a counter to implement LRU replacement policy)
 */

typedef struct { //each line of the cache must contain: 1 Valid Bit, t tag bits, B=2^b bytes per cache block
                 //also must include counter to keep track of Least Recently Used blockline -- defining as an int 1 being most recently accessed
    int valid; //valid bit
    mem_addr_t tag; //tag bits
    int LRUtracker; //LRU tracker 0==unitiated
    char *block; //block array for data
} setLine;

typedef setLine *cacheSets; //lines make up a cache set
typedef cacheSets *cache; //sets make up the cache

cache Cache; //the cache
/* Picture for understanding
         Number of Sets      Each line in each set
cache   -> cacheSets[]    -> [][][][][][]

*/



/* Globals set by command line args */
int verbosity = 0; /* print trace if set */
int s = 0; /* set index bits */
int b = 0; /* block offset bits */
int E = 0; /* associativity */
char* trace_file = NULL;

/* Derived from command line args */
int S; /* number of sets */
int B; /* block size (bytes) */

/* Counters used to record cache statistics */
int miss_count = 0;
int hit_count = 0;
int eviction_count = 0;
unsigned long long int lru_counter = 1;



/* 
 * initCache - Allocate memory (with malloc) for cache data structures (i.e., for each of the sets and lines per set),
 * writing 0's for valid and tag and LRU
 *
 * TODO: Implement
 *
 */
void initCache()
{
    Cache = malloc(sizeof(cacheSets) * S); //allocating space for total number of sets

    int i;
    int j;
    for(i = 0; i < S; i++) {
        Cache[i] = malloc(sizeof(setLine) * E); //allocating space for current line

        for(j = 0; j < E; j++) {//filling in each line with 0s
            Cache[i][j].valid = 0;
            Cache[i][j].tag = 0;
            Cache[i][j].LRUtracker = 0;
        } 
    }

}


/* 
 * freeCache - free allocated memory
 *
 * This function deallocates (with free) the cache data structures of each
 * set and line.
 *
 * TODO: Implement
 */
void freeCache()
{
    int i;
    for(i = 0; i < S; i++){ //free each individual line first
        if(Cache[i] != NULL){
            free(Cache[i]); //free the set now
        }
    }
    free(Cache); //finally free the cache
    
}


/*
 * accessData - Access data at memory address addr
 *   If it is already in cache, increase hit_count
 *   If it is not in cache, bring it in cache, increase miss count.
 *   Also increase eviction_count if a line is evicted.
 *
 * TODO: Implement
 */
void accessData(mem_addr_t addr)
{
    //address [t bits][s bits]  [b bits]
    //        tag     set index block offset
    int tagsize = 64 - (s + b); //64 bit machine
    unsigned long long tag = addr >> (s + b); //shift the address to the right by the (set index + block offset) to get the tag
    
    //calculate set index
    unsigned long long temp = addr  << (tagsize); //cut off tag bits by shifting it to the left
    unsigned long long setIndex = temp >> (tagsize + b); //get just the setIndex by shifting it to the left by tag + b

    //we are currently working on simCache->sets[setIndex]
    int i;
    int hit = 0;
    int full = 1; //if set is full == 1 else 0 by default set is full
    int index = -1;
    for(i = 0; i < E; i++){
        //checking for a hit
        if(Cache[setIndex][i].valid == 1){ //line is valid
            if(Cache[setIndex][i].tag == tag) { //tag matches
                //we have a hit
                hit_count+=1; //incrementing hit counter
                hit = 1;
                index = i;

                //this code keeps track of least recently used
                for(i = 0; i < E; i++){ 
                    if(Cache[setIndex][i].LRUtracker != 0){ //as long as the memory is initialized we keep track
                        Cache[setIndex][i].LRUtracker += 1; //if the line wasn't accessed, we add 1 to its tracker
                    }
                }
                Cache[setIndex][index].LRUtracker = 1; //set the tracker of the line just accessed to 1. this signifies we just accessed this memory
                //the least recently used line will have the highest tracker number in turn

            }
        }
    }

    //now we must check if the set is full
    for(i = 0; i < E; i++){
        if(Cache[setIndex][i].valid == 0){ //if there is just 1 not valid line, meaning unitialized, we will say the cache is not full
            full = 0; //cache is not full
            break; 
        }
    }

    //if there is not a hit and the set is not full we will update the set
    if(hit == 0 && full == 0){
        miss_count += 1;
        //now we must find the next available line
        for(i = 0; i < E; i++){
            if(Cache[setIndex][i].valid == 0){
                index = i;
                break;                
            }
        }
        Cache[setIndex][index].tag = tag;
        Cache[setIndex][index].valid = 1;

        //this code keeps track of least recently used
        for(i = 0; i < E; i++){ 
            if(Cache[setIndex][i].LRUtracker != 0){ //as long as the memory is initialized we keep track
                Cache[setIndex][i].LRUtracker += 1; //if the line wasn't accessed, we add 1 to its tracker
            }
        }
        Cache[setIndex][index].LRUtracker = 1; //set the tracker of the line just accessed to 1. this signifies we just accessed this memory
         //the least recently used line will have the highest tracker number in turn
  
    }

    //if we reach this point w  e dont have a hit and the cache is full so we must free space
    else if(hit == 0){
        miss_count += 1;
        eviction_count += 1;
        int evictionNumber = 0;
        int evictionIndex = 0;
        //now we must find which line to evict
        for(i = 0; i < E; i++){
            if(Cache[setIndex][i].LRUtracker > evictionNumber){
                evictionNumber = Cache[setIndex][i].LRUtracker;
                evictionIndex = i;
            }
        }
        Cache[setIndex][evictionIndex].tag = tag; //updating tag
        Cache[setIndex][evictionIndex].valid = 1;

        //this code keeps track of least recently used
        for(i = 0; i < E; i++){ 
            if(Cache[setIndex][i].LRUtracker != 0){ //as long as the memory is initialized we keep track
                Cache[setIndex][i].LRUtracker += 1; //if the line wasn't accessed, we add 1 to its tracker
            }
        }
        Cache[setIndex][evictionIndex].LRUtracker = 1; //set the tracker of the line just accessed to 1. this signifies we just accessed this memory
         //the least recently used line will have the highest tracker number in turn
    }
}

/*
 * replayTrace - replays the given trace file against the cache
 *
 * This function:
 * - opens file trace_fn for reading (using fopen)
 * - reads lines (e.g., using fgets) from the file handle (may name `trace_fp` variable)
 * - skips lines not starting with ` S`, ` L` or ` M`
 * - parses the memory address (unsigned long, in hex) and len (unsigned int, in decimal)
 *   from each input line
 * - calls `access_data(address)` for each access to a cache line
 *
 * TODO: Implement
 *
 */
void replayTrace(char* trace_fn)
{
    FILE *file = fopen(trace_fn, "r"); //opens file for reading -- "r"
    char line[500];
    if(file == NULL){ //check to make sure we opened the file
        printf("%s\n", "Error opening trace file");
    }

    mem_addr_t memAddress;
    unsigned int len;

    while(fgets(line, sizeof(line), file) != NULL){ //reads current line into a string
        if(line[1] == 'S' || line[1] == 'L' || line[1] == 'M'){ //looks at second char of string since the first is either space or I so we can ignore it
            sscanf(line+3, "%llx, %u", &memAddress, &len); //adding +3 because the trace file contains 3 chars befeore actually getting to the data we want to look at
            accessData(memAddress); //passing in the memory address
            if(line[1] == 'M') { //if the cache command is M we must run another accessdata
                accessData(memAddress);
            }
        }
    }

    
    fclose(file); //making sure to close file    
    
}

/*
 * printUsage - Print usage info
 */
void printUsage(char* argv[])
{
    printf("Usage: %s [-hv] -s <num> -E <num> -b <num> -t <file>\n", argv[0]);
    printf("Options:\n");
    printf("  -h         Print this help message.\n");
    printf("  -v         Optional verbose flag.\n");
    printf("  -s <num>   Number of set index bits.\n");
    printf("  -E <num>   Number of lines per set.\n");
    printf("  -b <num>   Number of block offset bits.\n");
    printf("  -t <file>  Trace file.\n");
    printf("\nExamples:\n");
    printf("  linux>  %s -s 4 -E 1 -b 4 -t traces/yi.trace\n", argv[0]);
    printf("  linux>  %s -v -s 8 -E 2 -b 4 -t traces/yi.trace\n", argv[0]);
    exit(0);
}

/*
 *
 * !! DO NOT MODIFY !!
 *
 * printSummary - Summarize the cache simulation statistics. Student cache simulators
 *                must call this function in order to be properly autograded.
 */
void printSummary(int hits, int misses, int evictions)
{
    printf("hits:%d misses:%d evictions:%d\n", hits, misses, evictions);
    FILE* output_fp = fopen(".csim_results", "w");
    assert(output_fp);
    fprintf(output_fp, "%d %d %d\n", hits, misses, evictions);
    fclose(output_fp);
}

/*
 * main - Main routine 
 */
int main(int argc, char* argv[])
{
    char c;

    while( (c=getopt(argc,argv,"s:E:b:t:vh")) != -1){
        switch(c){
        case 's':
            s = atoi(optarg);
            break;
        case 'E':
            E = atoi(optarg);
            break;
        case 'b':
            b = atoi(optarg);
            break;
        case 't':
            trace_file = optarg;
            break;
        case 'v':
            verbosity = 1;
            break;
        case 'h':
            printUsage(argv);
            exit(0);
        default:
            printUsage(argv);
            exit(1);
        }
    }

    /* Make sure that all required command line args were specified */
    if (s == 0 || E == 0 || b == 0 || trace_file == NULL) {
        printf("%s: Missing required command line argument\n", argv[0]);
        printUsage(argv);
        exit(1);
    }

    /* Compute S, E and B from command line args */
    S = (unsigned int) pow(2, s);
    B = (unsigned int) pow(2, b);
 
    /* Initialize cache */
    initCache();

#ifdef DEBUG_ON
    printf("DEBUG: S:%u E:%u B:%u trace:%s\n", S, E, B, trace_file);
#endif
    replayTrace(trace_file);

    /* Free allocated memory */
    
    freeCache();

    /* Output the hit and miss statistics for the autograder */
    printSummary(hit_count, miss_count, eviction_count);
    
    return 0;
}
