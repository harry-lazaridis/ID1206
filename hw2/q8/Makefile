CC      = gcc
CFLAGS  = -O0 -Wall -Wextra
TARGET  = hw2_mmap_faults
N       ?= 25600
HUGE    ?= 64
NS      ?= 4096 8192 16384
FILTER  = grep -E "Allocating|Elapsed|Minor|Major"

.PHONY: all run run-huge hugepages perf time q1 q2 q3 clean

all: $(TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) -o $@ $<

# Option 1: normal pages
run: $(TARGET)
	./$(TARGET) $(N)

# Option 2: huge pages (run "make hugepages" first)
run-huge: $(TARGET)
	./$(TARGET) $(N) huge

# Reserve huge pages (needs sudo, resets on reboot)
hugepages:
	sudo sysctl -w vm.nr_hugepages=$(HUGE)
	grep Huge /proc/meminfo

# Page faults with perf, both modes
perf: $(TARGET)
	perf stat -e page-faults,minor-faults,major-faults ./$(TARGET) $(N)
	perf stat -e page-faults,minor-faults,major-faults ./$(TARGET) $(N) huge

# Page faults with /usr/bin/time, both modes
time: $(TARGET)
	/usr/bin/time -f "normal: minor=%R major=%F" ./$(TARGET) $(N)
	/usr/bin/time -f "huge:   minor=%R major=%F" ./$(TARGET) $(N) huge

# Question 1: Option 1 (normal pages) for N = 4096, 8192, 16384
q1: $(TARGET)
	@echo "===== Q1: Option 1, normal pages ====="
	@for n in $(NS); do \
		echo "--- N=$$n ---"; \
		/usr/bin/time -v ./$(TARGET) $$n 2>&1 | $(FILTER); \
	done

# Question 2: Option 2 (huge pages) for N = 4096, 8192, 16384
q2: $(TARGET)
	@echo "===== Q2: Option 2, huge pages ====="
	@grep HugePages_Free /proc/meminfo
	@for n in $(NS); do \
		echo "--- N=$$n ---"; \
		/usr/bin/time -v ./$(TARGET) $$n huge 2>&1 | $(FILTER); \
	done

# Question 3: both options side by side for N = 16384
q3: $(TARGET)
	@echo "===== Q3: N=16384, Option 1 vs Option 2 ====="
	@echo "--- Option 1 (normal) ---"
	@/usr/bin/time -v ./$(TARGET) 16384 2>&1 | $(FILTER)
	@echo "--- Option 2 (huge) ---"
	@/usr/bin/time -v ./$(TARGET) 16384 huge 2>&1 | $(FILTER)

clean:
	rm -f $(TARGET)
