#include <stdio.h>
#include <utility>

struct code_ptr{
	int pos;
	char *code;
};

struct state{
	code_ptr ptr;
	short val;
	bool ok = true;
};

int arr[65536];

short get(short i){
	unsigned short j = i + 32768;
	return arr[j];
}

void set(short i, short v){
	unsigned short j = i + 32768;
	arr[j] = v;
}

state error(code_ptr ptr, char message[]){
	state s = {ptr,0};
	s.ok = false;
	printf("%s on %d\n", message, ptr.pos);
	return s;
}

state eval(code_ptr ptr, bool require_set = false){
	// either way we expect square brackets and possibly
	// something in between
	int depth = 0;
	int start = ptr.pos;
	if (ptr.code[ptr.pos] != '['){
		char message[100];
		sprintf(message, "expected '[', got '%c'", ptr.code[ptr.pos]);
		return error(ptr, message);
	}
	ptr.pos ++;
	while (true){
		if (ptr.code[ptr.pos] == '['){
			depth++;
		}
		if (ptr.code[ptr.pos] == ']'){
			if (depth == 0){
				break;
			}
			depth--;
		}
		if (ptr.code[ptr.pos] == '\0'){
			return error(ptr, "unexpected end of file");
		}
		ptr.pos ++;
	}
	bool io = start + 1 == ptr.pos;
	// printf("%d %d %d\n", start, ptr.pos, io);
	short index = 0;
	if (not io){
		if (
			start + 2 == ptr.pos and
			ptr.code[ptr.pos-1] == '-'
		){
			index = 0;
		}
		else{
			code_ptr inner = ptr;
			inner.pos = start + 1;
			state s = eval(inner);
			if (not s.ok){
				return s;
			}
			index = s.val;
		}
	}

	// printf("%c %c\n", ptr.code[ptr.pos] ,ptr.code[ptr.pos + 1]);

	if (
		   ptr.code[ptr.pos + 1] == '='
		or ptr.code[ptr.pos + 1] == '-'
	){
		ptr.pos += 2;
		state s = eval(ptr);
		if (not s.ok){
			return s;
		}
		if (ptr.code[ptr.pos - 1] == '-'){
			s.val -= get(index);
		}
		if (io){
			putc(s.val, stdout);
			return {s.ptr,s.val};
		}
		// printf("set\n");
		set(index, s.val);
		return {s.ptr,s.val};
	}
	else if (require_set)
	{
		return error(ptr, "expected = or -");
	}
	else if (ptr.code[ptr.pos + 1] == ':'){
		ptr.pos += 2;
		state s;
		// printf("v: %d\n", get(index));
		while (get(index)){
			s = eval(ptr);
			if (not s.ok){
				return s;
			}
		}
		return s;
	}
	else if (io)
	{
		ptr.pos ++;
		short v = getc(stdin);
		return {ptr,v};
	}
	else{
		ptr.pos ++;
		return {ptr, get(index)};
	}
}

int main(int argc, char *argv[]){
	if (argc != 2){
		printf("Usage: %s <file>\n", argv[0]);
		return 1;
	}
	FILE *f = fopen(argv[1], "r");
	if (f == NULL){
		printf("Could not open file %s\n", argv[1]);
		return 1;
	}
	size_t size;
	fseek(f, 0, SEEK_END);
	size = ftell(f);
	fseek(f, 0, SEEK_SET);
	char *code = new char[size + 1];
	fread(code, 1, size, f);
	fclose(f);
	code[size] = '\0';
	// ignore characters other than [, ], =, -, :
	size_t i = 0;
	for (int j = 0; j < size; j++){
		if (code[j] == '[' or code[j] == ']' or code[j] == '=' or code[j] == '-' or code[j] == ':'){
			code[i] = code[j];
			i++;
		}
	}

	for (int i = 0; i < 65536; i++){
		arr[i] = 1;
	}

	code[i] = '\0';
	code_ptr ptr = {0, code};
	state s = eval(ptr);
	if (s.ptr.pos != i){
		printf("Ignored after %d", s.ptr.pos);
		return 1;
	}
	return s.ok;
}
