static char buf[32] = {0};

char* itoa(int val, int base) {
	int i = 30;
    if (!val) return "0";
	for(; val && i ; --i, val /= base)
		buf[i] = "0123456789abcdef"[val % base];
	return &buf[i+1];
}