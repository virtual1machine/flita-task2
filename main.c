#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// The sizes for each "memory aallocation increase" of both the arrays of
// chars storing each line (MORE_CHARS), and the array of pointers to chars
// pointing to the strings/lines (MORE_LINES).
#define MORE_LINES 1024
#define MORE_CHARS 1024

void find_another_end(int i, int j, char **matrix, int total_lines, int total_chars);

int main(void) {
    //reading matrix into 2d array
    FILE *file = fopen("C://Users//natal//Desktop//flita//newT2//incidence-matrix.txt", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    char **imatrix;
    imatrix = malloc(sizeof(char *) * MORE_LINES);

    size_t total_lines = 0;
    size_t total_chars = 0;
    char c;

    do {
        c = fgetc(file);
        if (ferror(file)) {
            printf("Error reading from file.\n");
            return 1;
        }
        if (feof(file)) {
            if (total_chars != 0) {
                imatrix[total_lines] = realloc(imatrix[total_lines], total_chars + 1);
                imatrix[total_lines][total_chars] = '\0';
                total_lines++;
            }
            break;
        }
        if (total_chars == 0) imatrix[total_lines] = malloc(MORE_CHARS);
        if (c != ' ') {
            imatrix[total_lines][total_chars] = c;
            total_chars++;
        }

        if (c == '\n') {
            imatrix[total_lines] = realloc(imatrix[total_lines], total_chars + 1);
            imatrix[total_lines][total_chars] = '\0';
            total_lines++;
            total_chars = 0;

            if (total_lines % MORE_LINES == 0) {
                size_t new_size = total_lines + MORE_LINES;
                imatrix = realloc(imatrix, sizeof(char *) * new_size);
            }
        } else if (total_chars % MORE_CHARS == 0) {
            size_t new_size = total_chars + MORE_CHARS;
            imatrix[total_lines] =
                    realloc(imatrix[total_lines], new_size);
        }
    } while (true);

    imatrix = realloc(imatrix, sizeof(char *) * total_lines);
    fclose(file);

    //printing matrix into a file in dot-format
    FILE *gfile = fopen("C://Users//natal//Desktop//flita//newT2//graph.dot", "w");
    if (gfile == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    //incidence matrix to .dot
    fputs("graph {\n\n", gfile);
    int edges_num = (int) strlen(imatrix[0]) - 1;
    int vertex_num = (int) total_lines;

    bool *isConnected;
    isConnected = (bool *) calloc(vertex_num, sizeof(bool));
    for (int i = 0; i < edges_num; i++) {
        int buff[2];
        int counter = 0;
        for (int j = 0; j < vertex_num; j++) {
            if (imatrix[j][i] == '1') {
                isConnected[j] = true;
                buff[counter] = j;
                counter++;
            }
        }
        if (counter > 0) {
            if (counter == 1)
                buff[1] = buff[0];
            fprintf(gfile, "%d -- %d\n", buff[0], buff[1]);
        }
    }
    for (int k = 0; k < vertex_num; k++) {
        if (!isConnected[k])
            fprintf(gfile, "%d\n", k);
    }
    fprintf(gfile, "\n}");



    fclose(gfile);
    system("dot -Tjpg C://Users//natal//Desktop//flita//newT2//graph.dot -o cool_graph.png");
    system("C://Users//natal//Desktop//flita//newT2//cmake-build-debug//cool_graph.png");
}