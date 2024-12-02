/* iaed-23 - project1 - ist1106340 - Francisco Uva - LEIC-T */
#include "proj1.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* GLOBAL VARIABLES */ 
int num_lines = 0; /* number of existing lines */
int num_stops = 0; /* number of existing stops */
int num_links = 0; /* number of existing links */
line lines[MAX_LINES]; /* array that stores the lines */
stop stops[MAX_STOPS]; /* array that stores the stops */
link links[MAX_LINKS]; /* array that stores the links */
char c; /* last character that has been read from the input */


/* Asks for a command until the stop command is inputed */
int main() {
	
	while (command()) {};
	
	return 0;
}


/* Handles the command that is inputed. Returns 1 if a new command should be 
inputed. Returns 0 if the programs should stop. */
int command() {
	
	c = getchar();

	switch (c){
		
		case 'q':
			/* stops the program */
			return 0;
		
		case 'c':
			lines_command();
			return 1;

		case 'p':
			stops_command();
			return 1;

		case 'l':
			links_command();
			return 1;

		case 'i':
			interlinks_command();
			return 1;

		case 'f':
			f_command();
			return 1;

		default:
			/* ignores unrecognized commands */
			return 1;
	}
}


/* Handles the 'c' command. Creates and lists stops. With arguments inputed, a 
new line is created if it doesn't already exist. If it already exists lists all 
the line stops by the itinerary sequence with the option of being listed in the 
reverse order. If the command is inputed without arguments, lists all lines by 
order of creation */
void lines_command() {
	
	char name[MAX_LINE_NAME]; 
	char inverted[8] = {'\0'}; 
	int len_name; /* Length of the line name */
	int len_inverted = 0; /* length of inverted parameter */

	len_name = read_string(name, MAX_LINE_NAME);
	
	if (len_name != -1) {
		/* if command isn't inputed without arguments */
		len_inverted = read_string(inverted, 8);
	}

	if (len_name == -1) {
		/* if command is inputed without arguments */
		print_all_lines();
	}
	else if (len_inverted != -1) {
		
		if (strcmp(inverted, "inv") != 0  && strcmp(inverted, "inve") != 0 
		&& strcmp(inverted, "inver") != 0 && strcmp(inverted, "invers") != 0
		&& strcmp(inverted, "inverso") != 0) {
		
			printf("%s", LINE_ERROR_INCORRECT_SORT_OPTION);
			return;
		}
		if (find_line_index_by_name(name) != -1) {
			
			print_line_stops(name, 1);
		}	
	}
	else if (len_name != 0 && find_line_index_by_name(name) != -1) {
		
		print_line_stops(name, 0);
	}
	else {
		
		add_line(name);
	}
}


/* Handles the 'p' command. Creates and lists stops. With no arguments, prints
all stops by order of creation. With coordinates arguments, creates a new stop.
Only with name argument, prints the coordinates of the stop. */
void stops_command() {
	
	char name[MAX_STOP_NAME];
	char lat_str[17];
	char lon_str[17];
	double lat;
	double lon;
	int len_name, len_lat;

	len_name = read_string(name, MAX_STOP_NAME);
	
	if (len_name != -1) {
		
		len_lat = read_string(lat_str, 17);
		
		if (len_lat != -1) {
			read_string(lon_str, 17);
			lat = atof(lat_str);
			lon = atof(lon_str);
		}
	}
	
	if (len_name == -1) {
		
		print_all_stops();
	}
	else if (len_lat == -1) {
		
		print_stop(name);
	}
	else {
		
		add_stop(name, lat, lon);
	}
}


/* Handles the 'l' command. Creates new links. It doesn't output anything. */
void links_command() {
	
	char line_name[MAX_LINE_NAME] = {'\0'};
	char stop_start[MAX_STOP_NAME] = {'\0'};
	char stop_end[MAX_STOP_NAME] = {'\0'};
	char cost_str[17] = {'\0'};
	char duration_str[17] = {'\0'};
	float cost, duration;

	if (read_string(line_name, MAX_LINE_NAME) == -1){
		return;
	}

	read_string(stop_start, MAX_STOP_NAME);
	read_string(stop_end, MAX_STOP_NAME);
	read_string(cost_str, 17);
	read_string(duration_str, 17);
	
	cost = atof(cost_str);
	duration = atof(duration_str);

	add_link(line_name, stop_start, stop_end, cost, duration);
}


/* Handles the 'i' command. Prints the interlinks between lines. Lines are
printed in alphabetical order. */
void interlinks_command() {
	
	int i, j, h;
	/* Array that is going to be changed so that the lines are in 
	alphabetical order */
	char copy_lines[MAX_LINES][MAX_LINE_NAME];
	/* Temporary variable that holds the line that is being copied */
	char temp[MAX_LINE_NAME];

	for (i = 0; i < num_stops; i++) {
		
		if (stops[i].number_of_lines < 2) {
			
			continue;
		}

		copy_array_strings(copy_lines, stops[i].stop_lines, 
		stops[i].number_of_lines);

		/* Puts the lines of the copied array in alphabetical order */
		for (j = 0; j < stops[i].number_of_lines-1; j++) {
		
			for (h = 0; h < stops[i].number_of_lines-j-1; h++) {
			
				if (strcmp(copy_lines[h], copy_lines[h+1]) > 0) {
				
					strcpy(temp, copy_lines[h]);
					strcpy(copy_lines[h], copy_lines[h+1]);
					strcpy(copy_lines[h+1], temp);
				}
			}
		}

		printf("%s %d: ", stops[i].name, stops[i].number_of_lines);
		print_array_strings(copy_lines, stops[i].number_of_lines);

	}

}


/* LINES */

/* Finds the index of a line on the vector 'lines' by the line name
and returns it. If the line with the given name does not exist, returns -1  */
int find_line_index_by_name(char name[]) {
	
	int i;

	for (i = 0; i < num_lines; i++) {
		
		if (strcmp(lines[i].name, name) == 0) {
			
			return i;
		}
	}
	
	return -1;
}


/* Prints all existant lines, with ther names, origin and destiny stop,
number of stops, total cost and total duration. */
void print_all_lines() {
	
	int i;

	for (i = 0; i < num_lines; i++){
		
		if (strlen(lines[i].name) < 6) {
		
			if (strcmp(lines[i].stop_start, "") == 0) {
			
				printf("%s %d %.2f %.2f\n",
				lines[i].name, lines[i].number_of_stops,
				lines[i].total_cost, lines[i].total_duration);
			}
			else {

				printf("%s %s %s %d %.2f %.2f\n",
				lines[i].name, lines[i].stop_start,
				lines[i].stop_end, lines[i].number_of_stops,
				lines[i].total_cost, lines[i].total_duration);
			}
		}
	}
}


/* Creates a new line with the given name. */
void add_line(char name[]) {
	
	line new_line;

	strcpy(new_line.name, name);
	strcpy(new_line.stop_start, "");
	strcpy(new_line.stop_end, "");
	new_line.number_of_stops = 0;
	new_line.total_cost = 0;
	new_line.total_duration = 0;

	lines[num_lines] = new_line;
	num_lines++;
}


/* Prints all the stops of a line by its itinerary order, with the option of
being printed in the reverse order. */
void print_line_stops(char name[], int inv) {

	int i;
	int ind_line = find_line_index_by_name(name);

	if (lines[ind_line].number_of_stops == 0) {
		
		return;
	}
	
	if (inv) {

		for (i = lines[ind_line].number_of_stops-1; i > 0; i--) {

			printf("%s, ", lines[ind_line].line_stops[i]);
		}
		
		printf("%s\n", lines[ind_line].line_stops[0]);
	}
	else {
		
		for (i = 0; i < lines[ind_line].number_of_stops-1; i++) {
		
			printf("%s, ", lines[ind_line].line_stops[i]);
		}

		printf("%s\n", 
			lines[ind_line].line_stops[lines[ind_line].number_of_stops-1]);
	}
}


/* Prints all stops by order of creation, with its coordinates. */
void print_all_stops() {
	
	int i;

	for (i = 0; i < num_stops; i++) {
		
		printf("%s: %16.12f %16.12f %d\n", 
			stops[i].name, stops[i].latitude, 
			stops[i].longitude, stops[i].number_of_lines);
	}
}


/* STOPS */

/* Finds the index of a stop on the vector 'stops' by the stop name
and returns it. If the stop with the given name does not exist, returns -1  */
int find_stop_index_by_name(char name[]) {
	
	int i;

	for (i = 0; i < num_stops; i++) {
		
		if (strcmp(stops[i].name, name) == 0) {
			
			return i;
		}
	}

	return -1;
}


/* Prints the coordinates of an existant stop. */
void print_stop(char name[]) {
	
	int i = find_stop_index_by_name(name);

	if (i != -1) {
		
		printf("%16.12f %16.12f\n", stops[i].latitude, stops[i].longitude);
		return;
	}
	
	printf("%s: %s", name, STOP_ERROR_NON_EXISTANT_STOP);
}


/* Creates a new stop. */
void add_stop(char name[], double lat, double lon) {
	
	stop new_stop;

	if (find_stop_index_by_name(name) != -1) {
		
		printf("%s: %s", name, STOP_ERROR_DUPLICATE_STOP);
		return;
	}

	if (lat > 90 || lat < -90 || lon > 180 || lon < -180) {
		printf(STOP_ERROR_INAVLID_LOCATION);
		return;
	}
	

	strcpy(new_stop.name, name);
	new_stop.latitude = lat;
	new_stop.longitude = lon;
	new_stop.number_of_lines = 0;
	stops[num_stops] = new_stop;
	num_stops++;
}


/* LINKS */

/* Creates a new link. */
void add_link(char line_name[], char stop_start[], char stop_end[], 
float cost, float duration) { 
	
	link new_link;
	int ind_line = find_line_index_by_name(line_name);
	int ind_stop_start = find_stop_index_by_name(stop_start);
	int ind_stop_end = find_stop_index_by_name(stop_end);
	int i;
	/* state variables. 1 if it the start/end stop doesn't already have that 
	line, 0 if it already has. */
	int add_nline_start = 1;
	int add_nline_end = 1;

	if (ind_line == -1) {
		
		printf("%s: %s", line_name, LINK_ERROR_NON_EXISTANT_LINE);
		return;
	}
	
	if (ind_stop_start == -1) {
		
		printf("%s: %s", stop_start, LINK_ERROR_NON_EXISTANT_STOP);
		return;
	}

	if (ind_stop_end == -1) {
		
		printf("%s: %s", stop_end, LINK_ERROR_NON_EXISTANT_STOP);
		return;
	}

	/* If the line already has links. */
	if (strcmp(lines[ind_line].stop_start, "") != 0 &&
	(strcmp(lines[ind_line].stop_end, "") != 0)) {
		
		if ((strcmp(lines[ind_line].stop_start, stop_end) != 0
		&& strcmp(lines[ind_line].stop_end, stop_start) != 0)) {
		
			printf("%s", LINK_ERROR_CANNOT_BE_ASSCOCIATED);
			return;
		}
	}

	if (cost < 0 || duration < 0) {
		
		printf("%s", LINK_ERROR_NEGATIVE_COST_OR_DURATION);
		return;
	}
	
	strcpy(new_link.line_name, line_name);
	strcpy(new_link.stop_start, stop_start);
	strcpy(new_link.stop_end, stop_end);
	new_link.cost = cost;
	new_link.duration = duration;

	lines[ind_line].total_cost += cost;
	lines[ind_line].total_duration += duration;

	/* If it is going to be a circular line. */
	if (strcmp(lines[ind_line].stop_start, stop_end) == 0 &&
	(strcmp(lines[ind_line].stop_end, stop_start) == 0)) {

		strcpy(lines[ind_line].stop_end, stop_end);
		strcpy(lines[ind_line].line_stops[lines[ind_line].number_of_stops],
		stop_end);
		lines[ind_line].number_of_stops++;

		links[num_links] = new_link;
		num_links++;
		return;
	}

	/* Checks if the stops already have that line. */
	for (i = 0; i < stops[ind_stop_start].number_of_lines; i++) {
		
		if (strcmp(stops[ind_stop_start].stop_lines[i], line_name) == 0) {
			
			add_nline_start = 0;
			break;
		}
	}

	for (i = 0; i < stops[ind_stop_end].number_of_lines; i++) {
		
		if (strcmp(stops[ind_stop_end].stop_lines[i], line_name) == 0) {
			
			add_nline_end = 0;
			break;
		}
	}

	/* If if it should, adds 1 to the number of lines of the stops */
	if (add_nline_start) {
		
		strcpy(stops[ind_stop_start].stop_lines
		[stops[ind_stop_start].number_of_lines], line_name);
		
		stops[ind_stop_start].number_of_lines++;
	}
	
	if (add_nline_end) {
		
		strcpy(stops[ind_stop_end].stop_lines
		[stops[ind_stop_end].number_of_lines], line_name);
		
		stops[ind_stop_end].number_of_lines++;
	}
	
	/* If the line doesn't already have links. */
	if (strcmp(lines[ind_line].stop_start, "") == 0 &&
	(strcmp(lines[ind_line].stop_end, "") == 0)) {
		
		strcpy(lines[ind_line].stop_start, stop_start);
		strcpy(lines[ind_line].stop_end, stop_end);
		
		strcpy(lines[ind_line].line_stops[0], stop_start);
		strcpy(lines[ind_line].line_stops[1], stop_end);

		lines[ind_line].number_of_stops += 2;
	}
	/* Adds to the beginning of the line. */
	else if (strcmp(lines[ind_line].stop_start, stop_end) == 0) {
		
		for (i = lines[ind_line].number_of_stops-1; i >= 0; i--) {
			
			strcpy(lines[ind_line].line_stops[i+1], 
			lines[ind_line].line_stops[i]);
		};
		
		strcpy(lines[ind_line].stop_start, stop_start);
		strcpy(lines[ind_line].line_stops[0], stop_start);
		lines[ind_line].number_of_stops++;
	}
	/* Adds to the end of the line. */
	else {

		strcpy(lines[ind_line].line_stops[lines[ind_line].number_of_stops],
		stop_end);
		strcpy(lines[ind_line].stop_end, stop_end);
		lines[ind_line].number_of_stops++;
	}
	
	links[num_links] = new_link;
	num_links++;
}


/* AUXILIARIES */

/* Reads the input, character by character and populates the given string, until
it finds a space if it is not quoted, or until it finds a quote, if it is quoted
Returns the length of the string, or if it reaches the given size. If it doen't 
read any string, returns -1. */
int read_string(char *str, int size) {
	
	int length = 0;
	int quoted = 0; /* state variable. If quoted = 1. If not = 0. */

	if (c == '\n') {

		return -1; /*so it doesn't read any more*/
	}

	c = getchar();

	/* ignores leading spaces */
	while (c == ' ') {

		c = getchar();
	}

	if (c == '\n') {

		return -1; /*so it doesn't read any more*/
	}

	/* checks if it is quoted */
	if (c == '"') {
		
		quoted = 1;
	}

	if (quoted) {
		
		c = getchar();
		
		while (c != '\n' && c != EOF && c != '"' && length < size) {
		/* Read input until length is reached or quote is encountered */
			*str++ = c;
			length++;
			c = getchar();
		}
	}
	else {
		
		while (c != '\n' && c != EOF && c != ' ' && length < size) {
		/* Read input until length is reached or sapce is encountered */
			*str++ = c;
			length++;
			c = getchar();
		}
	}

	/* If it has reached its maximum size, discards the rest of the input. */
	if (size == length) {
		
		while (c != '\n'){
			
			c = getchar();
		}
	}

	*str = '\0'; /* Null-terminate the string */
	
	return length;
}


/* Copies an array of strings to another array. */
void copy_array_strings(char copy[][MAX_LINE_NAME], char arr[][MAX_LINE_NAME],
int size) {

	int i;

	for (i = 0; i < size; ++i) {
		
		strcpy(copy[i], arr[i]);
	}
}


/* Prints an array of strings */
void print_array_strings(char arr[][MAX_LINE_NAME], int size) {

	int i;

	for (i = 0; i < size-1; i++) {
		
		printf("%s ", arr[i]);
	}

	printf("%s\n", arr[size-1]);
}


void f_command() {
	char stop_name[MAX_STOP_NAME];
	int i;
	
	/* state variable, if it is 0 it has not printed, if it is 1 it has printed */
	int printed = 0; 

	read_string(stop_name, MAX_STOP_NAME);

	if (find_stop_index_by_name(stop_name) == -1) {
		
		printf("%s: %s", stop_name, STOP_ERROR_NON_EXISTANT_STOP);
		return;
	}

	for (i = 0; i < num_lines; i++) {
		
		if (strcmp(stop_name, lines[i].stop_end) == 0) {
			
			if (printed) {
				printf(" ");
			}
			
			printf("%s", lines[i].name);
			printed = 1;
		}
		
	}
	
	if (printed) {
		printf("\n");
	}
}