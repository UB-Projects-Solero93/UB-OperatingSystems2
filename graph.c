#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

//Sort descending
int cmpFunc(const void* a, const void* b){
    ListData* elem1 = (ListData*) a;
    ListData* elem2 = (ListData*) b;
    if (elem1->numTimes < elem2->numTimes){
        return 1;
    }
    if (elem1->numTimes > elem2->numTimes){
        return -1;
    }
    return 0;
}

ListData* convertToIterable(List* list){
    ListData* finalList = (ListData*)malloc(list->numItems * sizeof(ListData));
    ListItem* currentNode = list->first;
    for (int i=0; i<list->numItems; i++){
        finalList[i] = *currentNode->data;
        currentNode = currentNode->next;
    }
    return finalList;
}

int drawGraph(char* word, RBTree* tree){
    RBData* treeNode = findNode(tree, word);
    if (treeNode == NULL){
        printf("Word not found");
        return -1;
    }
    ListData* occurrences = convertToIterable(treeNode->occurrences);
    int numItems = treeNode->occurrences->numItems;
    if (treeNode->occurrences->first == NULL){
        printf("xorra");
    }
    
    qsort(occurrences, numItems, sizeof(ListData), cmpFunc);

    FILE *data = fopen("appearances.data", "w+");
    if (!data){
        printf("Error saving data");
        return -1;
    }

    for (int i=0; i<numItems; i++){
        fprintf(data, "%s %f\n", occurrences[i].key, occurrences[i].numTimes * 1.0 / (treeNode->num));
    }
    fclose(data);

    FILE* gnuplot = popen("gnuplot -p", "w");
    if (!gnuplot){
        printf("Error creating pipes\n");
        return -1;
    }
    fprintf(gnuplot, "set title \'Probability graph\'\n");
    fprintf(gnuplot, "set yrange [0:%f]\n", occurrences[0].numTimes * 1.0 / (treeNode->num));
    fprintf(gnuplot, "set xrange [0:%d]\n", numItems);
    fprintf(gnuplot, "plot \"appearances.data\" with lines\n");
    fflush(gnuplot);
    if (pclose(gnuplot) == -1){
        printf("Error closing pipes\n");
        return -1;
    }
    return 0;
}