#include <stdio.h>

void metric(int hit,int miss)
{
double hit_ratio=0.0;
double miss_ratio=0.0;
hit_ratio=(double)hit/500*100;
miss_ratio=(100.0-hit_ratio);
printf("\nCache Metrics:\n");
printf("-------------------------------------------------\n");
printf("| %-30s | %d                   |\n", "Total number of cache hits", hit);
printf("| %-30s | %d                   |\n","Total number of cache miss", miss);
printf("| %-30s | %.2f%%                |\n", "Hit ratio", hit_ratio);
printf("| %-30s | %.2f%%                |\n", "Miss ratio", miss_ratio);
    
}
