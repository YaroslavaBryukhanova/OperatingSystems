#include<stdio.h>
#include <stdlib.h>
#include <math.h>


typedef struct Point {
    float x;
    float y;
}Point;
float distance(Point A, Point B);

float area(Point A, Point B, Point C);

int main() {
    struct Point A = {.x = 2.5, .y = 6};
    struct Point B = {1, 2.2};
    struct Point C = {10, 6};
    printf("%f\n", distance(A,B));
    printf("%f\n", area(A,B,C));
    return 0;
}

float distance(Point A,Point B){
	return sqrt(pow(A.x - B.x, 2) + pow(A.y - B.y, 2));
}

float area(Point A,Point B,Point C){
	return 0.5 * fabs(A.x * B.y - B.x * A.y + B.x * C.y - C.x * B.y + C.x * A.y - A.x * C.y);
}