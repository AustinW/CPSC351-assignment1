//
//  main.c
//  
//
//  Created by Austin White on 2/21/14.
//
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>

struct Reader {
    char *line;
    size_t length;
    ssize_t read;
};

struct Reader* reset_reader(struct Reader *reader) {
    reader->line = NULL;
    reader->length = 0;
    
    return reader;
}

int processor_info(const char *str) {
    
    return strstr(str, "processor") ||
        strstr(str, "vendor_id") ||
        strstr(str, "cpu family") ||
        strstr(str, "model") ||
        strstr(str, "model name");
}

void print_processor_info(FILE *fp, struct Reader *r) {
    
    printf("##################\n");
    printf("# Processor Info #\n");
    printf("##################\n");
    
        
    while ((r->read = getline(&r->line, &r->length, fp)) != -1) {
        if (processor_info(r->line))
            printf("%s", r->line);
    }
    
    printf("\n");
}

void print_kernel_info(FILE *fp, struct Reader *r) {
    
    printf("###############\n");
    printf("# Kernel Info #\n");
    printf("###############\n");
    
    while ((r->read = getline(&r->line, &r->length, fp)) != -1) {
        printf("%s", r->line);
    }
    
    printf("\n");
}

const char* memory_info(const char *str) {
    return strstr(str, "MemTotal");
}

void print_memory_info(FILE *fp, struct Reader *r) {
    
    printf("###############\n");
    printf("# Memory Info #\n");
    printf("###############\n");
    
    while ((r->read = getline(&r->line, &r->length, fp)) != -1) {
        if (memory_info(r->line))
            printf("%s", r->line);
    }
    
    printf("\n");
}

/**
 * print_uptime_info
 * @params:      FILE *fp
                 struct Reader *r
 * @return:      void
 * @description: Prints the contents of /proc/uptime. Format is
 *               as follows: 13212593.73 25774848.54. First part
 *               is seconds since system boot
 */
void print_uptime_info(FILE *fp) {
    printf("################\n");
    printf("# Up-Time Info #\n");
    printf("################\n");
    
    char uptime_seconds[15];
    
    fscanf(fp, "%s", uptime_seconds);
    
    printf("Time since last system boot: %ss\n", uptime_seconds);
    
    printf("\n");
}

int main(int argc, char *argv[]) {
    
    // Version 1 of program
    if (argc == 1) {
        
        FILE *processor_file_pointer = fopen("/proc/cpuinfo", "r");
        // FILE *processor_file_pointer = fopen("cpuinfo", "r");
        FILE *kernel_file_pointer = fopen("/proc/version", "r");
        // FILE *kernel_file_pointer = fopen("version", "r");
        FILE *memory_file_pointer = fopen("/proc/meminfo", "r");
        // FILE *memory_file_pointer = fopen("meminfo", "r");
        FILE *uptime_file_pointer = fopen("/proc/uptime", "r");
        // FILE *uptime_file_pointer = fopen("uptime", "r");
        
        Reader *r = new Reader();
        
        if (processor_file_pointer)
            print_processor_info(processor_file_pointer, reset_reader(r));
        else
            fprintf(stderr, "ERROR: Could not access CPU information\n");
        
        if (kernel_file_pointer)
            print_kernel_info(kernel_file_pointer, reset_reader(r));
        else
            fprintf(stderr, "ERROR: Could not access kernel information\n");
        
        if (memory_file_pointer)
            print_memory_info(memory_file_pointer, reset_reader(r));
        else
            fprintf(stderr, "ERROR: Could not access memory information\n");
        
        if (uptime_file_pointer)
            print_uptime_info(uptime_file_pointer);
        else
            fprintf(stderr, "ERROR: Could not access up-time information\n");
        
        delete(r);
        fclose(processor_file_pointer);
        fclose(kernel_file_pointer);
    // Version 2 of program
    } else if (argc == 3) {
        
    }
    
    return 0;
    
}