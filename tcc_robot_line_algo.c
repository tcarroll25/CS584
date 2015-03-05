/* Tyler Carroll - CS584 - Robot arm algorithm */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

/* prototypes */
int algo(int *lines, int size);

/* test arrays */
int testA[3] = {4,6,10};
int testB[13] = {16,4,2,18,4,5,9,20,12,8,18,2,19};
int testC[5] = {20,6,4,10,20};
int testD[5] = {20,4,6,10,20};

int main(int argc, char *argv[])
{
    int compact_seg_size;
    printf("Test Cases:\n");

    printf("\nTest Case A: This is a simple test, it shows why starting with the largest segment helps eliminate some optimization problems. A greedy algorithm that doesn't start with the largest segment would return a compacted segment size of 12\n");
    printf("Running test case A\n");
    compact_seg_size = algo(testA,3);
    printf("Minimum compacted segment size is %d\n", compact_seg_size);

    printf("\nTest Case B: This is a more complicated test. This shows that it is more efficient to try to get as close as possible to an edge. If the algorithm tried to stay in the middle or just work its way towards a single edge the there wouldn't be enough room within the boundaries for larger segments when they appear.\n");
    printf("Running test case B\n");
    compact_seg_size = algo(testB,13);
    printf("Minimum compacted segment size is %d\n", compact_seg_size);
   
    printf("\nTest Case C: This test case shows when the algorithm can be inefficient. Occasionally it would be better to not try to get as close to an edge as possible, if a future edge would allow you to get even closer to an edge than you already are. The minimum compacted size of test case C is 20, however this algorithm returns 28. It would be better for the segment of size 4 to go right, so the segment of size 10 can go left and closer to an edge, which would leave more room for the segment of size 20\n"); 
    printf("Running test case C\n");
    compact_seg_size = algo(testC,5);
    printf("Minimum compacted segment size is %d\n", compact_seg_size);
    
    printf("\nTest Case D: This test case shows that the scenario in test case C is a rare occurrence because the segments have to be in a specific order of sizes for it to occur. If I switch the segments of size 4 and 6 then the algorithm calculates the correct minimum compacted size\n");
    printf("Running test case D\n");
    compact_seg_size = algo(testD,5);
    printf("Minimum compacted segment size is %d\n", compact_seg_size);
}

int algo(int *lines, int size){
    int left_boundary = 0;
    int right_boundary = 0;
    int left_index = 0;
    int right_index = 0;
    int left_temp = 0;
    int right_temp = 0;
    int left_edge = 0;
    int right_edge = 0;
    int left_edge_dist_from_left = 0;
    int right_edge_dist_from_left = 0;
    int left_edge_dist_from_right = 0;
    int right_edge_dist_from_right = 0;
    int max_size = 0;
    int max_index = 0;
    int i,j;

    /* Find the maximum size line segment */
    printf("Line segments: ");
    for(i = 0; i < size; i++){
        printf("%d ", lines[i]);
        if(lines[i] > max_size){
            max_size = lines[i];
            max_index = i;
        }
    }
    printf("\n");
    printf("\tStarting with biggest segment of size %d located at index %d\n", max_size, max_index);
   
    /* Finding the minimum size compacted line segment */
    
    /* Start with current max */ 
    right_boundary = max_size;
    right_index = max_size;
    /* Check left */
    printf("\tIterating all line segments to the left of biggest segment\n");
    for(i = max_index - 1; i >= 0; i--){
        printf("\tLeft boundary: %d, Right boundary: %d, left_index: %d\n",
               left_boundary,right_boundary,left_index);
        /* Calculate index when folding line left and right */
        left_temp = left_index - lines[i];
        right_temp = left_index + lines[i];
        /* Calculate distance from each current edge */
        left_edge_dist_from_left = left_temp - left_boundary;
        right_edge_dist_from_left = right_temp - left_boundary;
        left_edge_dist_from_right = right_boundary - left_temp;
        right_edge_dist_from_right = right_boundary - right_temp;
        /* Calculate shortest distance from edge or shortest overlap */
        if(abs(left_edge_dist_from_left) < abs(left_edge_dist_from_right)){
            left_edge = left_edge_dist_from_left;
        }else{
            left_edge = left_edge_dist_from_right;
        } 
        if(abs(right_edge_dist_from_left) < abs(right_edge_dist_from_right)){
            right_edge = right_edge_dist_from_left;
        }else{
            right_edge = right_edge_dist_from_right;
        }
        /* Pick choice that is closest to edge or least amount over edge */
        if( (left_edge < right_edge) && (left_edge >= 0) ){
            /* no overlaps, left edge is closer to an edge */
            left_index -= lines[i];
            printf("\tSegment %d of size %d going left, left_index at %d, left boundary at %d\n",
                   i,lines[i],left_index,left_boundary);
        }else if( (right_edge < left_edge) && (right_edge >= 0) ){
            /* no overlaps, right edge is closer to an edge */
            left_index += lines[i];
            printf("\tSegment %d of size %d going right, left_index at %d, right boundary at %d\n",
                   i,lines[i],left_index,right_boundary);
        }else if(right_edge < left_edge){
            /* at least left edge goes over edge,
               pick positive edge or least negative */
            left_index -= lines[i];
            if(left_index < left_boundary){
                left_boundary = left_index;
            }
            printf("\tSegment %d of size %d going left, left_index at %d, left boundary at %d\n",
                   i,lines[i],left_index,left_boundary);
        }else{
            /* at least right edge goes over edge,
               pick positive edge or least negative */
            left_index += lines[i];
            if(left_index > right_boundary){
                right_boundary = left_index;
            }
            printf("\tSegment %d of size %d going right, left_index at %d, right boundary at %d\n",
                   i,lines[i],left_index,right_boundary);
        }
    }
    /* Check right */
    printf("\tIterating all line segments to the right of biggest segment\n");
    for(i = max_index + 1; i < size; i++){
        printf("\tLeft boundary: %d, Right boundary: %d, right_index: %d\n",
               left_boundary,right_boundary,right_index);
        /* Calculate index when folding line left and right */
        left_temp = right_index - lines[i];
        right_temp = right_index + lines[i];
        /* Calculate distance from each current edge */
        left_edge_dist_from_left = left_temp - left_boundary;
        right_edge_dist_from_left = right_temp - left_boundary;
        left_edge_dist_from_right = right_boundary - left_temp;
        right_edge_dist_from_right = right_boundary - right_temp;
        /* Calculate shortest distance from edge or shortest overlap */
        if(abs(left_edge_dist_from_left) < abs(left_edge_dist_from_right)){
            left_edge = left_edge_dist_from_left;
        }else{
            left_edge = left_edge_dist_from_right;
        } 
        if(abs(right_edge_dist_from_left) < abs(right_edge_dist_from_right)){
            right_edge = right_edge_dist_from_left;
        }else{
            right_edge = right_edge_dist_from_right;
        }
        /* Pick choice that is closest to edge or least amount over edge */
        if( (left_edge < right_edge) && (left_edge >= 0) ){
            /* no overlaps, left edge is closer to an edge */
            right_index -= lines[i];
            printf("\tSegment %d of size %d going left, right_index at %d, left boundary at %d\n",
                   i,lines[i],right_index,left_boundary);
        }else if( (right_edge < left_edge) && (right_edge >= 0) ){
            /* no overlaps, right edge is closer to an edge */
            right_index += lines[i];
            printf("\tSegment %d of size %d going right, right_index at %d, right boundary at %d\n",
                   i,lines[i],right_index,right_boundary);
        }else if(right_edge < left_edge){
            /* at least left edge goes over edge,
               pick positive edge or least negative */
            right_index -= lines[i];
            if(right_index < left_boundary){
                left_boundary = right_index;
            }
            printf("\tSegment %d of size %d going left, right_index at %d, left boundary at %d\n",
                   i,lines[i],right_index,left_boundary);
        }else{
            /* at least right edge goes over edge,
               pick positive edge or least negative */
            right_index += lines[i];
            if(right_index > right_boundary){
                right_boundary = right_index;
            }
            printf("\tSegment %d of size %d going right, right_index at %d, right boundary at %d\n",
                   i,lines[i],right_index,right_boundary);
        }

    }

    return (right_boundary-left_boundary);   
}
