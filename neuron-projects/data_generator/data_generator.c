#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <math.h>

int main() {
    FILE *fp = fopen("data.txt", "w");
    srand(time(NULL));
    int i;
    int categories[] = {1, 2, 3, 4}; 
    double x1, x2;
    double p;
    for(i = 0; i < 4000; i++) {
        
        p = (double)fabs(sin(rand()));
        x1 = (double)(sin(rand()));
        x2 = (double)(sin(rand()));

        if( p <= 0.1 ) {
            fprintf(fp, "%lf,%lf,%d\n",x1,x2,categories[rand()%4]);
        } 
        else {
            if( pow(x1-0.5, 2) + pow(x2-0.5, 2) < 0.16 || pow(x1+0.5, 2) + pow(x2+0.5, 2) < 0.16 ) {
                fprintf(fp, "%lf,%lf,%d\n",x1,x2,1);
            } 
            else if ( pow(x1-0.5, 2) + pow(x2+0.5, 2) < 0.16 || pow(x1+0.5, 2) + pow(x2-0.5, 2) < 0.16 ) {
                fprintf(fp, "%lf,%lf,%d\n",x1,x2,2);
            } else {
                if( (x1 >= 0 && x2 >= 0) || (x1 <= 0 && x2 <= 0) ) {
                    fprintf(fp, "%lf,%lf,%d\n",x1,x2,3);
                }
                else if ( (x1 <= 0 && x2 >= 0) || (x1 >= 0 && x2 <= 0) ) {
                    fprintf(fp, "%lf,%lf,%d\n",x1,x2,4);
                }
            }
        }
    }

    for(i = 0; i < 4000; i++) {

        x1 = (double)(sin(rand()));
        x2 = (double)(sin(rand()));

        if( pow(x1-0.5, 2) + pow(x2-0.5, 2) < 0.16 || pow(x1+0.5, 2) + pow(x2+0.5, 2) < 0.16 ) {
            fprintf(fp, "%lf,%lf,%d\n",x1,x2,1);
        } 
        else if ( pow(x1-0.5, 2) + pow(x2+0.5, 2) < 0.16 || pow(x1+0.5, 2) + pow(x2-0.5, 2) < 0.16 ) {
            fprintf(fp, "%lf,%lf,%d\n",x1,x2,2);
        } 
        else {
            if( (x1 >= 0 && x2 >= 0) || (x1 <= 0 && x2 <= 0) ) {
                fprintf(fp, "%lf,%lf,%d\n",x1,x2,3);
            }
            else if ( (x1 <= 0 && x2 >= 0) || (x1 >= 0 && x2 <= 0) ) {
                fprintf(fp, "%lf,%lf,%d\n",x1,x2,4);
            }
        }
    }
}