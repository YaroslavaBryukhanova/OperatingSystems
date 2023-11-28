#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>

// Define constants for event device and key codes
#define EVENT_DEVICE "/dev/input/by-path/platform-i8042-serio-0-event-kbd"
#define E_KEY 0x12
#define X_KEY 0x2d
#define P_KEY 0x19
#define C_KEY 0x2e
#define A_KEY 0x1e
#define B_KEY 0x30

// Function declarations
void print_event(struct input_event *ev);
void is_terminated(struct input_event *ev, int key_e_is_holding);
void handle_shortcut_exam(struct input_event *ev, int key_p_is_holding);
void handle_shortcut_cappuccino(struct input_event *ev, int key_c_is_holding, int key_a_is_holding);
void handle_shortcut_scholarship(struct input_event *ev, int key_a_is_holding);
void print_shortcuts();

// Main function
int main() {
    int fd;
    struct input_event ev;

    // Print available shortcuts
    print_shortcuts();

    // Open the input event device
    fd = open(EVENT_DEVICE, O_RDONLY);
    if (fd == -1) {
        perror("Error opening device");
        exit(EXIT_FAILURE);
    }

    // Main event loop
    while (1) {
        if (read(fd, &ev, sizeof(struct input_event)) < 0) {
            perror("Error reading event");
            break;
        }

        // Process keyboard events
        if (ev.type == EV_KEY && (ev.value == 0 || ev.value == 1 || ev.value == 2)) {
            print_event(&ev);
        }
    }

    // Close the device
    close(fd);

    return 0;
}

// Function to print details of an input event
void print_event(struct input_event *ev) {
    // Event type description
    const char *event_type;

    // Flags indicating whether certain keys are being held
    int key_e_is_holding, key_p_is_holding, key_c_is_holding, key_a_is_holding;

    // Determine event type
    switch (ev->value) {
        case 0:
            event_type = "RELEASED";
            break;
        case 1:
            event_type = "PRESSED";
            break;
        case 2:
            event_type = "REPEATED";
            break;
        default:
            event_type = "UNKNOWN";
            break;
    }

    // Update key holding flags
    // (Note: This might need synchronization if multithreading is used)
    // (Also, it would be beneficial to use an array or a struct to manage these flags)
    if (ev->code == 0x12 && (ev->value == 1 || ev->value == 2)) {
        key_e_is_holding = 1;
    } else if (ev->code == 0x12 && (ev->value == 0)) {
        key_e_is_holding = 0;
    }
    if (ev->code == 0x19 && (ev->value == 1 || ev->value == 2)) {
        key_p_is_holding = 1;
    } else if (ev->code == 0x19 && (ev->value == 0)) {
        key_p_is_holding = 0;
    }
    if (ev->code == 0x2e && (ev->value == 1 || ev->value == 2)) {
        key_c_is_holding = 1;
    } else if (ev->code == 0x2e && (ev->value == 0)) {
        key_c_is_holding = 0;
    }
    if (ev->code == 0x1e && (ev->value == 1 || ev->value == 2)) {
        key_a_is_holding = 1;
    } else if (ev->code == 0x1e && (ev->value == 0)) {
        key_a_is_holding = 0;
    }

    // Log event details to a file
    FILE *output_file = fopen("ex1.txt", "a");
    if (output_file != NULL) {
        fprintf(output_file, "%s 0x%04x (%d)\n", event_type, ev->code, ev->code);
        fclose(output_file);
    } else {
        perror("Error opening output file");
    }

    // Check for specific shortcuts and handle accordingly
    is_terminated(ev, key_e_is_holding);
    handle_shortcut_exam(ev, key_p_is_holding);
    handle_shortcut_cappuccino(ev, key_c_is_holding, key_a_is_holding);
    handle_shortcut_scholarship(ev, key_a_is_holding);

    // Print event details to the console
    printf("%s 0x%04x (%d)\n", event_type, ev->code, ev->code);
}
 

 

// Function to check if the program should be terminated
void is_terminated(struct input_event *ev, int key_e_is_holding) {
    if (ev->code == X_KEY && (ev->value == 1 || ev->value == 2) && key_e_is_holding == 1) {
        FILE *output_file = fopen("ex1.txt", "a");
        if (output_file != NULL) {
            fprintf(output_file, "Terminating the program...\n");
            fclose(output_file);
        } else {
            perror("Error opening output file");
        }
        exit(EXIT_SUCCESS);
    } else if (ev->code == E_KEY && ev->value == 0) {
        key_e_is_holding = 0;
    }
}

// Function to handle the "P + E" shortcut
void handle_shortcut_exam(struct input_event *ev, int key_p_is_holding) {
    if (ev->code == E_KEY && ev->value == 1 && key_p_is_holding == 1) {
        FILE *output_file = fopen("ex1.txt", "a");
        if (output_file != NULL) {
            fprintf(output_file, "I passed the Exam!\n");
            fclose(output_file);
        } else {
            perror("Error opening output file");
        }

    }
}

// Function to handle the "C + A + P" shortcut
void handle_shortcut_cappuccino(struct input_event *ev, int key_c_is_holding, int key_a_is_holding) {
    if (ev->code == P_KEY && ev->value == 1 && key_c_is_holding == 1 && key_a_is_holding == 1) {
        FILE *output_file = fopen("ex1.txt", "a");
        if (output_file != NULL) {
            fprintf(output_file, "Get some cappuccino!\n");
            fclose(output_file);
        } else {
            perror("Error opening output file");
        }
    }
}

// Function to handle the "A + B" shortcut
void handle_shortcut_scholarship(struct input_event *ev, int key_a_is_holding) {
    if (ev->code == B_KEY && ev->value == 1 && key_a_is_holding == 1) {
        FILE *output_file = fopen("ex1.txt", "a");
        if (output_file != NULL) {
            fprintf(output_file, "Great scholarship in this semester!\n");
            fclose(output_file);
        } else {
            perror("Error opening output file");
        }
    }
}

// Function to demonstrate all available shortcuts
void print_shortcuts() {
    printf("Available Shortcuts:\n");
    printf("1. E + X: The program will be terminated\n");
    printf("2. P + E: I passed the Exam!\n");
    printf("3. C + A + P: Get some cappuccino!\n");
    printf("4. A + B: Great scholarship in this semester!\n");
}
