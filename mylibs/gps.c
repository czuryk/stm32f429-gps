#include "gps.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"

int sats_num;
int sat_packets;
int pack_cur;
int satsarr[50];
int data_ready;
int ready_to_accept = 0;

int cpy_left_from(const char* str, const char* substr, const int off, char* tstr) {
	char* pos;
	int tlen;	
	pos = strstr(str, substr);
	if (pos != NULL) {
		tlen = strlen(str) - (pos - str)-(off-1);
		strncpy(tstr, pos+off, tlen);
		tstr[tlen-1] = '\0';
		return 0;
	} else {
		return 1;
	}
}


int cpy_left_to(const char* str, const char* substr, const int off, char* tstr) {
	char* pos;
	int tlen;	
	pos = strstr(str, substr);
	if (pos != NULL) {
		tlen = (pos - str)-(off);
		strncpy(tstr, str, tlen);
		tstr[tlen] = '\0';
		return 0;
	} else {
		return 1;		
	}
}


void GPS_GetCoord(const char* str, char* lat, char* lon) {
	char* pos1;
	if (strstr(str, "GPRMC") != NULL) {	
		pos1 = strstr(str, ",A,");
		if (pos1 != NULL) {
			strncpy(lat, pos1+3, 9);
		} else {
			strncpy(lat, "0000.0000", 9);
		}
		lat[9] = '\0';
		pos1 = strstr(str, ",N,");
		if (pos1 != NULL) {
			strncpy(lon, pos1+4, 9);
		} else {
			strncpy(lon, "0000.0000", 9);		
		}
		lon[9] = '\0';
	}
}


void GPS_GetDateTime(const char* str, char* date, char* time) {
	char* pos1;
	if (strstr(str, "GPRMC") != NULL) {	
		pos1 = strstr(str, "*");
		strncpy(date, pos1-10, 6);
		date[6] = '\0';
		strncpy(time, str+7, 6);
		time[6] = '\0';
	}
}


void GPS_SatInfo(const char* str, int sats) {
	char tstr[80];
	char tres[20];
	int res;
	int satn;
	int signal;
	int i;
	
	if (strstr(str, "GPGSV") != NULL) {	
		cpy_left_from(str, "GSV", 4, tstr);
		cpy_left_to(tstr, ",", 0, tres);
		sat_packets = atoi(tres);
		cpy_left_from(tstr, ",", 1, tstr);
		cpy_left_to(tstr, ",", 0, tres);
		pack_cur = atoi(tres);
		cpy_left_from(tstr, ",", 1, tstr);
		cpy_left_to(tstr, ",", 0, tres);
		sats = atoi(tres);

		if (pack_cur == 1) {
			for (i = 0; i < 50; i++) {satsarr[i]=-1;};
			data_ready = 0;
			ready_to_accept = 1;
		}
		
		if (ready_to_accept == 0) {return;}
		
		res = 0;
		while (res == 0) {
			if (cpy_left_from(tstr, ",", 1, tstr) == 1) {res = 1; break;} // Skip
			cpy_left_to(tstr, ",", 0, tres);
			satn = atoi(tres);	//Get sat num;
			if (cpy_left_from(tstr, ",", 1, tstr) == 1) {res = 1; break;} // Skip
			if (cpy_left_from(tstr, ",", 1, tstr) == 1) {res = 1;  break;} // Skip
			if (cpy_left_from(tstr, ",", 1, tstr) == 1) {res = 1;  break;} // Skip
			if (cpy_left_to(tstr, ",", 0, tres) == 1) {cpy_left_to(tstr, "*", 0, tres);}
			signal = atoi(tres);	//Get sat signal;
			if (signal > 0) {satsarr[satn] = signal;}
		}
		if (sat_packets == pack_cur) {
			data_ready = 1;
			ready_to_accept = 0;
			//SatAdvData(); // Callback function;
		}
	}
}

