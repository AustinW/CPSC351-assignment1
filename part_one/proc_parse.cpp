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
#include <string>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <pthread.h>

using namespace std;

struct rate {
    int n_sector_rw;
    int n_context_s;
    int n_process_c;
    int interval;
    int interval2;
};

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

void *retrieveInfo(void *data)
{
    rate *printoutRate = (rate*)data;

    while (true) {
        char line[128];
        FILE *fp;
        int temp, n_read_sectors, n_write_sectors;
        char disk_name[25];

        fp = fopen("/proc/diskstats", "r");

        if (fp == NULL)
            exit(0);

        while (fgets(line, 128, fp)) {
            sscanf(line, "%*d %*d %s %*d %*d %d %*d %*d %*d %d %*d %*d %*d %*d", disk_name, &n_read_sectors, &n_write_sectors);

            if (strncmp(disk_name, "sda", strlen("sda")) == 0) {
                break;
            } else {
                continue;
            }
        }

        fclose(fp);

        if (printoutRate->n_sector_rw == -1) {
            printoutRate->n_sector_rw = n_read_sectors + n_write_sectors;
        } else {
            printoutRate->n_sector_rw = n_read_sectors + n_write_sectors - printoutRate->n_sector_rw;
        }


        fp = fopen("/proc/stat", "r");
        char str_name[30];
        int n_number;

        if (fp == NULL) exit(0);

        while (fgets(line, 128, fp)) {
            sscanf(line, "%s %d", str_name, &n_number);

            if (strncmp(str_name, "ctxt", strlen("ctxt")) == 0) {
                if (printoutRate->n_context_s == -1) {
                    printoutRate->n_context_s = n_number;
                } else {
                    printoutRate->n_context_s = n_number - printoutRate->n_context_s;
                }
            } else if (strncmp(str_name, "processes", strlen("processes")) == 0) {
                if (printoutRate->n_process_c == -1) {
                    printoutRate->n_process_c = n_number;
                } else {
                    printoutRate->n_process_c = n_number - printoutRate->n_process_c;
                }
            } else {
                continue;
            }
        }

        fclose(fp);

        usleep(printoutRate->interval);
    }
}

void *printInfo(void *data)
{
    rate *printoutRate = (rate*)data;

    int count = 0;

    while (true) {
        usleep(printoutRate->interval2);

        FILE *fp;
        char line[128];
        float t_user, t_sys, t_idle;
        float sum, time_percent;

        char name[30];
        float number;
        float mem_total, mem_free;

        fp = fopen("/proc/stat", "r");

        while (fgets(line, 128, fp)) {
            sscanf(line, "%s %f %f %f", name, &t_user, &t_sys, &t_idle);

            if (strncmp(name, "cpu", strlen("cpu")) == 0) {
                break;
            }
        }

        fclose(fp);

        sum = t_user + t_sys + t_idle;

        printf("#---- System Info ----#\n");

        printf("User mode: %f\n", t_user / sum);
        printf("Kernel mode: %f\n", t_sys / sum);
        printf("Idle mode: %f\n", t_idle / sum);


        fp = fopen("/proc/meminfo", "r");

        while (fgets(line, 128, fp)) {
            sscanf(line, "%s %f", name, &number);

            if (strncmp(name, "MemTotal:", strlen("MemTotal:")) == 0) {
                mem_total = number;
            }

            if (strncmp(name, "MemFree:", strlen("MemFree:")) == 0) {
                mem_free = number;
                break;
            }
        }

        printf("The percentage of free memory: %f\n", mem_free / mem_total);
        fclose(fp);
        //printf("Read and write rate:%d\nContext switch rate:%d\nProcess creation rate:%d\n\n",prate->n_sector_rw,prate->n_contex_s,prate->n_process_c)
        printf("Read and write rate: %d\n", printoutRate->n_sector_rw);
        printf("Context switch rate: %d\n", printoutRate->n_context_s);
        printf("Process creation rate: %d\n", printoutRate->n_process_c);

        printf("#---------------------#\n");
    }
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
        
        int read_rate = stoi(argv[1]);
        int printout_rate = stoi(argv[2]);
        int seconds = 0;
        int calculations;

        rate rateTracker;
        rateTracker.n_sector_rw = -1;
        rateTracker.n_context_s = -1;
        rateTracker.n_process_c = -1;
        rateTracker.interval = read_rate * 1000000;
        rateTracker.interval2 = printout_rate * 1000000;

        pthread_t thread;
        pthread_t thread2;

        if (pthread_create(&thread, NULL, retrieveInfo, &rateTracker) != 0) {
            printf("Error creating thread\n");
        }

        if (pthread_create(&thread2, NULL, printInfo, &rateTracker) != 0) {
            printf("Error creating thread\n");
        }

        while(true);

    }
    
    return 0;
}