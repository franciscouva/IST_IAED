#ifndef MAIN_H
#define MAIN_H

/* LINES */

#define MAX_LINES 200
#define MAX_LINE_NAME 21
#define LINE_ERROR_INCORRECT_SORT_OPTION "incorrect sort option.\n"


/* STOPS */

#define MAX_STOPS 10000
#define MAX_STOP_NAME 51
#define STOP_ERROR_DUPLICATE_STOP "stop already exists.\n"
#define STOP_ERROR_NON_EXISTANT_STOP "no such stop.\n"
#define STOP_ERROR_INAVLID_LOCATION "invalid location.\n"




/* LINKS */

#define MAX_LINKS 30000
#define LINK_ERROR_NON_EXISTANT_LINE "no such line.\n"
#define LINK_ERROR_NON_EXISTANT_STOP "no such stop.\n"
#define LINK_ERROR_CANNOT_BE_ASSCOCIATED "link cannot be associated with bus line.\n"
#define LINK_ERROR_NEGATIVE_COST_OR_DURATION "negative cost or duration.\n"





typedef struct Line{
	char name[MAX_LINE_NAME];
	char stop_start[MAX_STOP_NAME];
	char stop_end[MAX_STOP_NAME];
	int number_of_stops;
	float total_cost;
	float total_duration;
	char line_stops[MAX_STOPS][MAX_STOP_NAME];
} line;

typedef struct Stop {
	char name[MAX_STOP_NAME];
	double latitude;
	double longitude;
	char stop_lines[MAX_LINES][MAX_LINE_NAME];
	int number_of_lines;
} stop;

typedef struct Link{
	char line_name[MAX_LINE_NAME];
	char stop_start[MAX_STOP_NAME];
	char stop_end[MAX_STOP_NAME];
	float cost;
	float duration;
} link;




/* LINES */
void lines_command();
void print_all_lines();
void add_line(char name[]);
void print_line_stops(char line_name[], int inv);
int find_line_index_by_name(char name[]);


/* STOPS */
void stops_command();
void print_all_stops();
void print_stop(char name[]);
void add_stop(char name[], double lat, double lon);
int find_stop_index_by_name(char name[]);


/* LINKS */
void links_command();
void add_link(char line_name[], char stop_start[], char stop_end[], 
				float cost, float duration);

/* INTERLINKS */
void interlinks_command();


/* F COMMAND */
void f_command();


/* AUXILIARIES */
int command();
int read_string(char *str, int size);
void print_array_strings(char arr[][MAX_LINE_NAME], int size);
void copy_array_strings(char copy[][MAX_LINE_NAME], 
						char arr[][MAX_LINE_NAME], int size);


#endif