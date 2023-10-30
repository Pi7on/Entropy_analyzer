#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 8192

unsigned long hash(const unsigned char *data, int byte_count) {
    unsigned long h = 0;
    for (int i = 0; i < byte_count; i++) {
        h = (h << 5) ^ data[i];  // Simple hash function. Can be replaced with a more sophisticated one.
    }
    return h;
}

double calculate_entropy(const char *filename, int byte_count) {
    unsigned long *hash_table = calloc(1 << (8 * byte_count), sizeof(unsigned long));  // Allocate hash table based on max possible sequences
    unsigned long total_sequences = 0;
    double entropy = 0.0;

    if (!hash_table) {
        perror("Failed to allocate memory for hash table");
        exit(1);
    }

    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Failed to open file");
        exit(1);
    }

    unsigned char buffer[BUFFER_SIZE + byte_count - 1];  // To handle sequences overlapping buffer boundaries
    memset(buffer, 0, sizeof(buffer));

    size_t bytesRead;
    while ((bytesRead = fread(buffer + byte_count - 1, 1, BUFFER_SIZE, file)) > 0) {
        for (size_t i = 0; i < bytesRead; i++) {
            if (i + byte_count <= bytesRead + byte_count - 1) {
                unsigned long h = hash(buffer + i, byte_count);
                hash_table[h]++;
                total_sequences++;
            }
        }
        memmove(buffer, buffer + bytesRead, byte_count - 1);  // Move the last bytes that might be part of the next sequence to the beginning
    }

    fclose(file);

    // Calculate entropy
    unsigned long max_index = 1UL << (8 * byte_count);
    for (unsigned long i = 0; i < max_index; i++) {
        if (hash_table[i]) {
            double probability = (double)hash_table[i] / total_sequences;
            entropy += probability * log2(probability);
        }
    }

    free(hash_table);
    return entropy;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {  // Checking if we have the correct number of arguments (program name, file path, byte count)
        printf("Usage:\n  %s <path_to_file>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    unsigned int byte_count = 1;  // currently we force byte-count to 1. cmd flag to be supported

    double entropy = calculate_entropy(filename, 1);
    double entropy_normalized = -entropy / (8.0 * byte_count);  // Normalize

    printf("Entropy for %d-byte sequences: %.15f\n", 1, entropy_normalized);

    return 0;
}