#include <stdio.h>
#include <stdlib.h>

void printFrames(int frames[], int frameSize) {
for (int i = 0; i < frameSize; i++) {
if (frames[i] == -1)
printf("- ");
else
printf("%d ", frames[i]);
}
printf("\n");
}

void lru(int refString[], int refSize, int frameSize) {
int *frames = (int *)malloc(frameSize * sizeof(int));
int *time = (int *)malloc(frameSize * sizeof(int));
for (int i = 0; i < frameSize; i++) {
frames[i] = -1;
time[i] = 0;
}
int pageFaults = 0;
printf("\nLRU Page Replacement:\n");
for (int i = 0; i < refSize; i++) {
int found = 0;
for (int j = 0; j < frameSize; j++) {
if (frames[j] == refString[i]) {
found = 1;
time[j] = i;
break;
}
}
if (!found) {
int lruIndex = 0;
for (int j = 1; j < frameSize; j++) {
if (time[j] < time[lruIndex])
lruIndex = j;
}
frames[lruIndex] = refString[i];
time[lruIndex] = i;
pageFaults++;
}
printFrames(frames, frameSize);
}
printf("Total Page Faults: %d\n", pageFaults);
free(frames);
free(time);
}

int main() {
int refSize, frameSize;
printf("Enter the number of pages in the reference string: ");
scanf("%d", &refSize);
int *refString = (int *)malloc(refSize * sizeof(int));
printf("Enter the reference string:\n");
for (int i = 0; i < refSize; i++) {
scanf("%d", &refString[i]);
}
printf("Enter the number of frames (minimum 3): ");
scanf("%d", &frameSize);
if (frameSize < 3) {
printf("Frame size should be at least 3.\n");
free(refString);
return 1;
}
lru(refString, refSize, frameSize);
free(refString);
return 0;
}

