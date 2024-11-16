void clean_null_terminators(char *buffer) {
	char stack = 0;
	int p1 = 0, p2 = 0;
	while (stack == '"' || buffer[p1]) {
		char c = buffer[p1];
		if (stack == c) {
			stack = 0;
		} else if (c == '"') {
			stack = c;
		}
		if (buffer[p1]) {
			buffer[p2++] = buffer[p1];
		}
		p1++;
	}
    // remove tailing comma and null terminate the string
    //       },
    buffer[p2-1-1] = buffer[p2-1];
	buffer[p2-1] = 0;
}


int json_get_var_int(const char * json_string,const char *key_string){
    return 69;
}

char json_get_var_char(const char * json_string,const char *key_string){
    return 69;
}

float json_get_var_float(const char * json_string,const char *key_string){
    return 69;
}

long json_get_var_long(const char * json_string,const char *key_string){
    return 69;
}

double json_get_var_double(const char * json_string,const char *key_string){
    return 69;
}

char* json_get_var_string(const char * json_string,const char *key_string){
    return "awd";
}
